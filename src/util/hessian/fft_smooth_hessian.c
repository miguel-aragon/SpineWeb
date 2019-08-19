/*

NAME:    fft_convolution

DESCRIPTION:
   Convolves a 3D array with a gaussian function.

COMPILE:
   gcc -o fft_smooth_hessian fft_smooth_hessian.c fourn.c nrutil.c rlft3.c -lm

USAGE:
   fft_convolution input sigma output

HISTORY:
   18-01-2008 First version.
   24-01-2008 Minor bug fixed.

Written by Miguel Angel Aragon Calvo.
Write comments to: miguel@pha.jhu.edu, miguel@kapteyn.astro.rug.nl


*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "nrutil.h"

void read_file_header(char *fname, int *DataType, int *GridType, int* sizeG, int *sizeX, int *sizeY, int *sizeZ);
void read_file(char *fname, double ***data, int *DataType, int *GridType, 
	       int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ);
void drlft3(double ***data, double **speq, unsigned long nn1, unsigned long nn2, unsigned long nn3, int isign);
void gauss_derivatives(double ***data, double sigma, double gamma, int sizeX, int sizeY, int sizeZ, int axis);
void fft_convolution(double ***data1, double ***data2, int sizeX, int sizeY, int sizeZ);
void write_file(char *fname, double ***data, int DataType, int GridType, 
		int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);


int main(int argc, char **argv)
{
  int    i;
  double ***data1,***data2;
  int    sizeG, sizeX, sizeY, sizeZ;
  float  offsX,offsY,offsZ;
  int    DataType, GridType;
  double sigma;
  float  temp;
  int    pairs[] = {11,22,33,12,31,32};
  char   file_out[256];


  if (argc != 4){
    printf("\nUsage:\n");
    printf("   ./fft_smooth_hessian input_file sigma[pixels] output_file\n\n");
    exit(0);
  }

  //-- Gaussian width
  sigma = (double) atof(argv[2]);
  printf("Smooth with %f pixels\n", sigma);
  //--- Read file header
  read_file_header(argv[1], &DataType, &GridType, &sizeG, &sizeX, &sizeY, &sizeZ);

  //--- Allocate memory for density array
  printf(">>> Allocating arrays of %d %d %d\n", sizeX,sizeY,sizeZ); fflush(stdout);
  data1=d3tensor(1,sizeX, 1,sizeY, 1,sizeZ);
  data2=d3tensor(1,sizeX, 1,sizeY, 1,sizeZ); 

  for (i=0;i<6;i++){

    printf(">>> -------------------------------------------\n");
    printf(">>> Computing component %d of Hessian matrix...\n",pairs[i]);

    //--- Read denstiy file
    read_file(argv[1], data1, &DataType, &GridType, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

    //--- Create 3D gaussian
    printf(">>> Create derivative %d  of width %f...\n", pairs[i], sigma); fflush(stdout);
    gauss_derivatives(data2, sigma, 1.0, sizeX,sizeY,sizeZ,pairs[i]);

    //--- Convolution
    printf(">>> Convolution...\n"); fflush(stdout);
    fft_convolution(data1, data2, sizeX,sizeY,sizeZ);

    //--- Write smoothed density file
    printf(">>> Write to file...\n"); fflush(stdout);

    sprintf(file_out, "%s%d", argv[3], pairs[i]);
    write_file(file_out, data1, DataType, GridType, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  }

  //--- Free memory
  printf(">>> Deallocating memory...\n");
  fflush(stdout);
  free_d3tensor(data2,1,sizeX,1,sizeY,1,sizeZ);
  free_d3tensor(data1,1,sizeX,1,sizeY,1,sizeZ);

  return 0;
}


//=============================================================
//               FUNCTION DESCRIPTION
//=============================================================
void read_file_header(char *fname, int *DataType, int *GridType, int *sizeG, int *sizeX, int *sizeY, int *sizeZ){

  FILE   *file_pointer;
  char   buffer[200];

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) {
      printf("    Can't open file %s header, exiting program...\n",buffer); 
      exit(0);
  }

  //--- Read header
  fread(DataType, sizeof(int),   1, file_pointer);
  fread(GridType, sizeof(int),   1, file_pointer);
  fread(sizeG,    sizeof(int),   1, file_pointer);
  fread(sizeX,    sizeof(int),   1, file_pointer);
  fread(sizeY,    sizeof(int),   1, file_pointer);
  fread(sizeZ,    sizeof(int),   1, file_pointer);

  fclose(file_pointer);

}

void read_file(char *fname, double ***data, int *DataType, int *GridType, 
	       int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ)
{
  int    i,j,k,w,q, cont;
  FILE   *file_pointer;
  char   buffer[200];
  double temp;
  int    remaining_bytes;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(file_pointer=fopen(buffer,"r"))) {
      printf("    Can't open read file '%s'\n",buffer); 
      exit(0);}

  //--- Read header
  fread(DataType, sizeof(int),   1, file_pointer);
  fread(GridType, sizeof(int),   1, file_pointer);
  fread(sizeG,    sizeof(int),   1, file_pointer);
  fread(sizeX,    sizeof(int),   1, file_pointer);
  fread(sizeY,    sizeof(int),   1, file_pointer);
  fread(sizeZ,    sizeof(int),   1, file_pointer);
  fread(offsX,    sizeof(float), 1, file_pointer);
  fread(offsY,    sizeof(float), 1, file_pointer);
  fread(offsZ,    sizeof(float), 1, file_pointer);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++){
   fread(&dummy_byte,sizeof(char),1,file_pointer);
  }

  printf("    Reading grid of size [%dx%dx%d] ... \n\n", *sizeX,*sizeY,*sizeZ);
  fflush(stdout);
  //--- Read data from file
  for (i=1; i<= *sizeX; i++){
    for (j=1; j<= *sizeY; j++){
      for (k=1; k<= *sizeZ; k++) {
	fread(&temp, sizeof(double), 1, file_pointer);	
	data[i][j][k] = temp;
      }
    }
  }

  fclose(file_pointer);

}


void gauss_derivatives(double ***data, double sigma, double gamma, int sizeX, int sizeY, int sizeZ, int axis){

  int    i,j,k;
  double dista2;
  double sizeX_2,sizeY_2,sizeZ_2;
  double xg,yg,zg;
  const double PI = 3.14159265358979;
  double distX2,distY2,distZ2;
  double sigma2,sigma_2;
  double const1,const2,const3;

  //--- Some constants
  sizeX_2 = sizeX/2.0;
  sizeY_2 = sizeY/2.0;
  sizeZ_2 = sizeZ/2.0;
  sigma2  = 2.0*sigma*sigma;
  sigma_2 = sigma*sigma;
  const1  = pow(sigma,2.0*gamma);
  const2  = 1.0/(pow(2.0*PI,1.5) * pow(sigma,5));
  const3  = 1.0/(pow(2.0*PI,1.5) * pow(sigma,7));

  for(i=1; i<=sizeX; i++ ){
    for(j=1; j<=sizeY; j++ ){
      for(k=1; k<=sizeZ; k++ ){

	//--- Wrap-around order
	if (i <= sizeX/2)
	  xg = (double) i - 0.5;
	else 
	  xg = (double) i - sizeX - 0.5;
	if (j <= sizeY/2)
	  yg = (double) j - 0.5;
	else
	  yg = (double) j - sizeY - 0.5;
	if (k <= sizeZ/2)
	  zg = (double) k - 0.5;
	else
	  zg = (double) k - sizeZ - 0.5;

	//--- Square of distances from center
	dista2 = xg*xg + yg*yg + zg*zg;

	//--- Second derivatives of Gaussian
	switch( axis ) 
	  {
	  case 11:
	    data[i][j][k] = -const1*(xg*xg/sigma_2-1.0)*const2*exp(-dista2/sigma2);  //--- XX
	    break;
	  case 22 :
	    data[i][j][k] = -const1*(yg*yg/sigma_2-1.0)*const2*exp(-dista2/sigma2);  //--- YY
	    break;
	  case 33 :
	    data[i][j][k] = -const1*(zg*zg/sigma_2-1.0)*const2*exp(-dista2/sigma2);  //--- ZZ
	    break;
	  case 12 :
	    data[i][j][k] = -const1*(xg*yg)            *const3*exp(-dista2/sigma2);  //--- XY
	    break;
	  case 31 :
	    data[i][j][k] = -const1*(zg*xg)            *const3*exp(-dista2/sigma2);  //--- ZX
	    break;
	  case 32 :
	    data[i][j][k] = -const1*(zg*yg)            *const3*exp(-dista2/sigma2);  //--- ZY
	    break;
	  }

      }
    }
  }

}


void fft_convolution(double ***data1, double ***data2, int sizeX, int sizeY, int sizeZ){
  int i,j,k;
  double fac;
  double Re,Im;
  double **speq1,**speq2, *sp1,*sp2;

  //--- Initialize auxiliary arrays for FFT
  speq1=dmatrix(1,sizeX,1,2*sizeY);
  speq2=dmatrix(1,sizeX,1,2*sizeY);

  //--- FFT forward
  printf("    FFT forward data 1 \n"); fflush(stdout);
  drlft3(data1,speq1,sizeX,sizeY,sizeZ,1);
  printf("    FFT forward data 2 \n"); fflush(stdout);
  drlft3(data2,speq2,sizeX,sizeY,sizeZ,1);

  //---------------------------------------------
  //         CONVOLUTION
  //---------------------------------------------
  //--- Normalization
  fac = 2.0/(sizeX*sizeY*sizeZ);

  //--- Pointers to 3D arrays
  sp1 = &data1[1][1][1];
  sp2 = &data2[1][1][1];
  //--- By using the pointer we can avoid the three loops
  for (i=1;i<=sizeX*sizeY*sizeZ/2;i++){
    Re = sp1[0]*sp2[0] - sp1[1]*sp2[1];
    Im = sp1[0]*sp2[1] + sp1[1]*sp2[0];
    sp1[0] = fac*Re;
    sp1[1] = fac*Im;	  
    sp1 += 2;
    sp2 += 2;	    
  }

  //--- Pointers to 2D arrays
  sp1 = &speq1[1][1];
  sp2 = &speq2[1][1];
  //--- By using the pointer we can avoid the three loops
  for (i=1;i<=sizeX*sizeY;i++){
    Re = sp1[0]*sp2[0] - sp1[1]*sp2[1];
    Im = sp1[0]*sp2[1] + sp1[1]*sp2[0];
    sp1[0] = fac*Re;
    sp1[1] = fac*Im;	  
    sp1 += 2;
    sp2 += 2;	    
  }

  //--- FFT reverse
  printf("    FFT reverse data 1\n");
  drlft3(data1,speq1,sizeX,sizeY,sizeZ,-1);

  //--- Deallocate auxiliary arrays
  free_dmatrix(speq1,1,sizeX,1,2*sizeY);
  free_dmatrix(speq2,1,sizeX,1,2*sizeY);
}


void write_file(char *fname, double ***data, int DataType, int GridType, 
		int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){
  int    i,j,k;
  FILE   *fp;
  double temp;
  char   buffer[200];
  int    remaining_bytes=0;

  //--- Attempt to open file
  sprintf(buffer,"%s",fname);
  if(!(fp=fopen(buffer,"w"))) {
      printf("    Can't open write file '%s'\n",buffer);
      exit(0);
    }

  printf("    Writting grid of size [%d,%d,%d] to file %s\n",sizeX,sizeY,sizeZ, buffer);
  fflush(stdout);

  //--- Write header
  fwrite(&DataType,sizeof(int),  1,fp);
  fwrite(&GridType,sizeof(int),  1,fp);
  fwrite(&sizeG   ,sizeof(int),  1,fp);
  fwrite(&sizeX   ,sizeof(int),  1,fp);
  fwrite(&sizeY   ,sizeof(int),  1,fp);
  fwrite(&sizeZ   ,sizeof(int),  1,fp);
  fwrite(&offsX   ,sizeof(float),1,fp);
  fwrite(&offsY   ,sizeof(float),1,fp);
  fwrite(&offsZ   ,sizeof(float),1,fp);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,fp); 

  for (i=1; i<=sizeX; i++){
    for (j=1; j<=sizeY; j++){
      for (k=1; k<=sizeZ; k++) {
	temp = data[i][j][k];
	fwrite(&temp,sizeof(double),1, fp);
     }
    }
  }
  fclose(fp);
}

