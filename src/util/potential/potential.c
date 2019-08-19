/*

gcc

*/


#include <stdio.h>
#include "nr.h"
#include "nrutil.h"


#define NDIM 3

void read_file_header(char *fname, int *DataType, int *GridType, int* sizeG, int *sizeX, int *sizeY, int *sizeZ);
void read_file(char *fname, double *data, int *DataType, int *GridType, 
	       int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ);
void green_function(double *data, unsigned long *nn);
void write_file(char *fname, double *data, int DataType, int GridType, 
		int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ);

int main(int argc, char **argv)
{
  int isign;
  unsigned long i,j,k,l,ndum=2,*nn;
  double *data1;

  //--- Header variables
  int    sizeG, sizeX, sizeY, sizeZ;
  float  offsX,offsY,offsZ;
  int    DataType, GridType;


  //--- Read file header
  read_file_header(argv[1], &DataType, &GridType, &sizeG, &sizeX, &sizeY, &sizeZ);
  printf("%d [%d %d %d]\n", sizeG, sizeX,sizeY,sizeZ);

  //--- Allocate memory for density array
  nn=lvector(1,NDIM);
  data1=dvector(1,sizeX*sizeY*sizeZ*2);
  nn[3] = sizeX;
  nn[2] = sizeY;
  nn[1] = sizeZ;

  //--- Read data from file
  printf("Reading data from file...\n"); fflush(stdout);
  read_file(argv[1], data1, &DataType, &GridType, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);


  //--- Forward FFT
  printf("Forward FFT...\n");
  isign=1;
  dfourn(data1,nn,NDIM,isign);
  
  //====================
  //--- Multiply by Green's function
  printf("Multiply by Greens function...\n");
  green_function(data1, nn);
  //====================


  //--- Inverse FFT
  printf("Inverse FFT...\n");
  isign = -1;
  dfourn(data1,nn,NDIM,isign);


  //--- Write smoothed density file
  printf(">>> Write to file...\n"); fflush(stdout);
  write_file(argv[2], data1, DataType, GridType, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);


  free_dvector(data1,1,sizeX*sizeY*sizeZ*2);
  free_lvector(nn,1,NDIM);
  return 0;
}


void green_function(double *data, unsigned long *nn)
{
  unsigned long i,j,k;
  int    l;
  double k_x,k_y,k_z;
  double pi2;
  double G_k;
  int    nn123 = nn[1]*nn[2]*nn[3];

  pi2 = 3.141592653589793238 * 2.0;

  for (i=1;i<=nn[3];i++)
    for (j=1;j<=nn[2];j++)
      for (k=1;k<=nn[1];k++) {

	if (i+j+k == 3) {
	  data[1] = 0;
	  data[2] = 0;
	  continue;
	}
	//--- Decompose into 3 vectors
	l=k+(j-1)*nn[1]+(i-1)*nn[2]*nn[1];
	//--- multiply by two (fast) 
	l=(l<<1)-1;

	//--- Wrap-around order
	if (i <= nn[3]/2) k_x = pi2*(i  -  1.0)/nn[3];
	else              k_x = pi2*(nn[3]-i+1)/nn[3];
	if (j <= nn[2]/2) k_y = pi2*(j  -  1.0)/nn[2];
	else              k_y = pi2*(nn[2]-j+1)/nn[2];
	if (k <= nn[1]/2) k_z = pi2*(k  -  1.0)/nn[1];
	else              k_z = pi2*(nn[1]-k+1)/nn[1];

	//--- Green's function. 
	G_k = -1.0 / (sin(k_x/2.0)*sin(k_x/2.0) + sin(k_y/2.0)*sin(k_y/2.0) + sin(k_z/2.0)*sin(k_z/2.0))/nn123;

	//--- Multiply G_x with REAL part of array
	data[l] = data[l] * G_k;
	//--- Multiply G_x with IMAGINARY part of array
	l++;
	data[l] = data[l] * G_k;
      }


}

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

void read_file(char *fname, double *data, int *DataType, int *GridType, 
	       int *sizeG, int *sizeX, int *sizeY, int *sizeZ, float *offsX, float *offsY, float *offsZ)
{
  unsigned long     i,j,k,w,q,l, cont;
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
  fread(sizeX,   sizeof(int),   1, file_pointer);
  fread(sizeY,   sizeof(int),   1, file_pointer);
  fread(sizeZ,   sizeof(int),   1, file_pointer);
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
  for (i=1;i<=(*sizeX);i++)
    for (j=1;j<=(*sizeY);j++)
      for (k=1;k<=(*sizeZ);k++) {
	l=k+(j-1)*(*sizeX)+(i-1)*(*sizeY)*(*sizeX);
	l=(l<<1)-1;
        fread(&temp, sizeof(double), 1, file_pointer);
        data[l] = temp;
      }

  fclose(file_pointer);

}

void write_file(char *fname, double *data, int DataType, int GridType, 
		int sizeG, int sizeX, int sizeY, int sizeZ, float offsX, float offsY, float offsZ){
  unsigned long     i,j,k,l;
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

  printf("    Writting grid of [%d,%d,%d]\n",sizeX,sizeY,sizeZ);
  fflush(stdout);

  //--- Write header
  fwrite(&DataType,sizeof(int),1,fp);
  fwrite(&GridType,sizeof(int),1,fp);
  fwrite(&sizeG   ,sizeof(int),1,fp);
  fwrite(&sizeX  ,sizeof(int),1,fp);
  fwrite(&sizeY  ,sizeof(int),1,fp);
  fwrite(&sizeZ  ,sizeof(int),1,fp);
  fwrite(&offsX   ,sizeof(float),1,fp);
  fwrite(&offsY   ,sizeof(float),1,fp);
  fwrite(&offsZ   ,sizeof(float),1,fp);

  //--- Fill rest of header with zeroes
  remaining_bytes = 256-36;
  char dummy_byte=0;
  for (i=0; i<remaining_bytes; i++) fwrite(&dummy_byte,sizeof(char),1,fp); 

  for (i=1;i<=sizeX;i++)
    for (j=1;j<=sizeY;j++)
      for (k=1;k<=sizeZ;k++) {
        l=k+(j-1)*sizeX+(i-1)*sizeY*sizeX;
        l=(l<<1)-1;
        temp =  data[l];
        fwrite(&temp,sizeof(double),1, fp);
      }

  fclose(fp);
}

