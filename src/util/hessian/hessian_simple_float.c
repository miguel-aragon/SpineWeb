/*

   NAME: 
       HESSIAN_SIMPLE

   DESCRIPTION:
       Compute the approximate hessian of a smooth field

   COMPILE AS:


   HISTORY:
       2009  Created

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "../../lib/header_struct.h"
#include "../../lib/io_files.h"

void derivate_x(float *Densi, float *Deriv, int sizeX, int sizeY, int sizeZ);
void derivate_y(float *Densi, float *Deriv, int sizeX, int sizeY, int sizeZ);
void derivate_z(float *Densi, float *Deriv, int sizeX, int sizeY, int sizeZ);
void derivate_x_write(float *Densi, struct Header Header, char *fname);
void derivate_y_write(float *Densi, struct Header Header, char *fname);
void derivate_z_write(float *Densi, struct Header Header, char *fname);


int main(int argc, char **argv)
{

  int i,j,k, I,J,K;
  //--- Data arrays
  float *Densi = NULL;
  float *Deriv = NULL;
  //--- Headers
  struct Header Header_Densi;
  struct Header Header_Deriv;
  char   file_out[256];

  if (argc != 3)
    {
      printf("------------------------------------------ \n");
      printf("  Compute the Hessian of a smooth field \n");
      printf("\n");
      printf(" Usage:\n");
      printf("     ./hessian_simple_float file_in.fvol file_prefix \n");
      printf("\n");
      printf("      Same as hessian_simple but in floating point.");
      printf("      The code will generate the six independent elements of the Hessian matrix\n");
      printf("       |  f_xx, f_yx, f_yz |\n");
      printf("   H = {  f_xy, f_yy, f_yy }\n");
      printf("       |  f_xz, f_yz, f_zz |\n");
      printf("------------------------------------------ \n"); 
      fflush(stdout);
      exit(1); 
    }

  //====================================
  //--- Read density field
  //====================================
  Densi = Read_DataCube_float(argv[1], &Header_Densi);

  //====================================
  //--- Allocate memory for derivatices
  //====================================
  if ((Deriv = (float *) malloc(sizeof(float)*Header_Densi.sizeX*Header_Densi.sizeY*Header_Densi.sizeZ )) == NULL) {
    printf(">>> ERROR: malloc failed for derivatives. \n    Exit program... \n\n");
    exit (1);
  }
  
  //==============================================
  //--- Compute derivatives
  //==============================================

  //----------
  //--- f_xx
  //----------
  printf(">>> Computing f_xx... \n");
  derivate_x(Densi, Deriv, Header_Densi.sizeX, Header_Densi.sizeY, Header_Densi.sizeZ);
  //--- Take derivative again and save to file
  sprintf(file_out, "%s_xx.fvol", argv[2]);
  derivate_x_write(Deriv, Header_Densi, file_out);

  //----------
  //--- f_xy
  //----------
  printf(">>> Computing f_xy... \n");
  derivate_x(Densi, Deriv, Header_Densi.sizeX, Header_Densi.sizeY, Header_Densi.sizeZ);
  //--- Take derivative again and save to file
  sprintf(file_out, "%s_xy.fvol", argv[2]);
  derivate_y_write(Deriv, Header_Densi, file_out);
  
  //----------
  //--- f_yy
  //----------
  printf(">>> Computing f_yy... \n");
  derivate_y(Densi, Deriv, Header_Densi.sizeX, Header_Densi.sizeY, Header_Densi.sizeZ);
  //--- Take derivative again and save to file
  sprintf(file_out, "%s_yy.fvol", argv[2]);
  derivate_y_write(Deriv, Header_Densi, file_out);

  //----------
  //--- f_xz
  //----------
  printf(">>> Computing f_xz... \n");
  derivate_x(Densi, Deriv, Header_Densi.sizeX, Header_Densi.sizeY, Header_Densi.sizeZ);
  //--- Take derivative again and save to file
  sprintf(file_out, "%s_xz.fvol", argv[2]);
  derivate_z_write(Deriv, Header_Densi, file_out);

  //----------
  //--- f_yz
  //----------
  printf(">>> Computing f_yz... \n");
  derivate_y(Densi, Deriv, Header_Densi.sizeX, Header_Densi.sizeY, Header_Densi.sizeZ);
  //--- Take derivative again and save to file
  sprintf(file_out, "%s_yz.fvol", argv[2]);
  derivate_z_write(Deriv, Header_Densi, file_out);

  //----------
  //--- f_zz
  //----------
  printf(">>> Computing f_zz... \n");
  derivate_z(Densi, Deriv, Header_Densi.sizeX, Header_Densi.sizeY, Header_Densi.sizeZ);
  //--- Take derivative again and save to file
  sprintf(file_out, "%s_zz.fvol", argv[2]);
  derivate_z_write(Deriv, Header_Densi, file_out);



  //--- Free memory
  free(Densi);
  free(Deriv);

  printf("\n\n>>> Clean exit from program!\n"); fflush(stdout);

  exit(0);

}



//==================================================
//
//==================================================
void derivate_x(float *Densi, float *Deriv, int sizeX, int sizeY, int sizeZ){
  long int i,j,k;
  long int i0,i1;
  long int ind, ind0, ind1;


  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	//--- Delta
	i0=i-1;
	i1=i+1;

	//--- Validate indexes
	if (i == 0)       i0 = sizeX-1;
	if (i == sizeX-1) i1 = 0;
	
	//--- f_x
	ind0 = k + j*sizeZ + i0*sizeZ*sizeY;
	ind1 = k + j*sizeZ + i1*sizeZ*sizeY;
	ind  = k + j*sizeZ + i *sizeZ*sizeY;
	Deriv[ind] = (Densi[ind1] - Densi[ind0] ) / 2.0;
	

      } //--- end for k
    } //--- end for j
  } //--- end for i

}


//==================================================
//
//==================================================
void derivate_x_write(float *Densi, struct Header Header, char *fname){
  int i,j,k;
  int i0,i1;
  int ind, ind0, ind1;
  FILE *file_pointer;
  float tempd;
  size_t dump;
  int  sizeX = Header.sizeX;
  int  sizeY = Header.sizeY;
  int  sizeZ = Header.sizeZ;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w"))){ 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Write header of file
  dump = fwrite(&Header, sizeof(Header), 1, file_pointer);


  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	//--- Delta
	i0=i-1;
	i1=i+1;

	//--- Validate indexes
	if (i == 0) i0 = sizeX-1;
	if (i == sizeX-1) i1 = 0;

	//--- f_x
	ind0 = k + j*sizeZ + i0*sizeZ*sizeY;
	ind1 = k + j*sizeZ + i1*sizeZ*sizeY;
	ind  = k + j*sizeZ + i *sizeZ*sizeY;

	//--- Derivative
	tempd = (Densi[ind1] - Densi[ind0] ) / 2.0;
	
	//--- Write to file
	dump = fwrite(&tempd,sizeof(float),1, file_pointer);

      } //--- end for k
    } //--- end for j
  } //--- end for i

  fclose(file_pointer);

}


//==================================================
//
//==================================================
void derivate_y(float *Densi, float *Deriv, int sizeX, int sizeY, int sizeZ){
  int i,j,k;
  int j0,j1;
  int ind, ind0, ind1;


  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	j0=j-1;
	j1=j+1;

	//--- Validate indexes
	if (j == 0) j0 = sizeY-1;
	if (j == sizeY-1) j1 = 0;

	//--- f_y
	ind0 = k + j0*sizeZ + i*sizeZ*sizeY;
	ind1 = k + j1*sizeZ + i*sizeZ*sizeY;
	ind  = k + j *sizeZ + i*sizeZ*sizeY;
	Deriv[ind] = (Densi[ind1] - Densi[ind0] ) / 2.0;
	

      } //--- end for k
    } //--- end for j
  } //--- end for i

}

//==================================================
//
//==================================================
void derivate_y_write(float *Densi, struct Header Header, char *fname){
  int i,j,k;
  int j0,j1;
  int ind, ind0, ind1;
  FILE *file_pointer;
  float tempd;
  size_t dump;
  int  sizeX = Header.sizeX;
  int  sizeY = Header.sizeY;
  int  sizeZ = Header.sizeZ;
 
  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w"))){ 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Write header of file
  dump = fwrite(&Header, sizeof(Header), 1, file_pointer);

  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	j0=j-1;
	j1=j+1;

	//--- Validate indexes
	if (j == 0) j0 = sizeY-1;
	if (j == sizeY-1) j1 = 0;

	//--- f_y
	ind0 = k + j0*sizeZ + i*sizeZ*sizeY;
	ind1 = k + j1*sizeZ + i*sizeZ*sizeY;
	ind  = k + j *sizeZ + i*sizeZ*sizeY;
	
	//--- Derivative
	tempd = (Densi[ind1] - Densi[ind0] ) / 2.0;
	
	//--- Write to file
	dump = fwrite(&tempd,sizeof(float),1, file_pointer);

      } //--- end for k
    } //--- end for j
  } //--- end for i

  fclose(file_pointer);

}

//==================================================
//
//==================================================
void derivate_z(float *Densi, float *Deriv, int sizeX, int sizeY, int sizeZ){
  int i,j,k;
  int k0,k1;
  int ind, ind0, ind1;


  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	k0=k-1;
	k1=k+1;

	//--- Validate indexes
	if (k == 0) k0 = sizeZ-1;
	if (k == sizeZ-1) k1 = 0;

	//--- f_xx
	ind0 = k0 + j*sizeZ + i*sizeZ*sizeY;
	ind1 = k1 + j*sizeZ + i*sizeZ*sizeY;
	ind  = k  + j*sizeZ + i*sizeZ*sizeY;
	Deriv[ind] = (Densi[ind1] - Densi[ind0] ) / 2.0;
	

      } //--- end for k
    } //--- end for j
  } //--- end for i
  
}

//==================================================
//
//==================================================
void derivate_z_write(float *Densi, struct Header Header, char *fname){
  int i,j,k;
  int k0,k1;
  int ind, ind0, ind1;
  FILE *file_pointer;
  float tempd;
  size_t dump;
  int  sizeX = Header.sizeX;
  int  sizeY = Header.sizeY;
  int  sizeZ = Header.sizeZ;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w"))){ 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Write header of file
  dump = fwrite(&Header, sizeof(Header), 1, file_pointer);

  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	k0=k-1;
	k1=k+1;

	//--- Validate indexes
	if (k == 0) k0 = sizeZ-1;
	if (k == sizeZ-1) k1 = 0;

	//--- f_xx
	ind0 = k0 + j*sizeZ + i*sizeZ*sizeY;
	ind1 = k1 + j*sizeZ + i*sizeZ*sizeY;
	ind  = k  + j*sizeZ + i*sizeZ*sizeY;

	//--- Derivative
	tempd = (Densi[ind1] - Densi[ind0] ) / 2.0;
	
	//--- Write to file
	dump = fwrite(&tempd,sizeof(float),1, file_pointer);


      } //--- end for k
    } //--- end for j
  } //--- end for i
  
  fclose(file_pointer);

}

