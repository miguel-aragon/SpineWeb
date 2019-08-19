/*

   NAME: 
       GRADIENT

   DESCRIPTION:
       Compute the approximate gradient of a scalar field

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

void derivate_x_write(double *Densi, struct Header Header, char *fname);
void derivate_y_write(double *Densi, struct Header Header, char *fname);
void derivate_z_write(double *Densi, struct Header Header, char *fname);


int main(int argc, char **argv)
{

  int i,j,k, I,J,K;
  //--- Data arrays
  double *Densi = NULL;
  //--- Headers
  struct Header Header_Densi;
  char   file_out[256];

  if (argc != 3)
    {
      printf("------------------------------------------ \n");
      printf("  Compute the gradient of a scalar field \n");
      printf("\n");
      printf(" Usage:\n");
      printf("     ./gradient file_in.dvol file_prefix \n");
      printf("\n");
      printf("------------------------------------------ \n"); 
      fflush(stdout);
      exit(1); 
    }

  //====================================
  //--- Read density field
  //====================================
  Densi = Read_DataCube_double(argv[1], &Header_Densi);

  
  //==============================================
  //--- Compute derivatives
  //==============================================

  //----------
  //--- f_x
  //----------
  printf(">>> Computing f_x... \n");
  sprintf(file_out, "%s_x.dvol", argv[2]);
  derivate_x_write(Densi, Header_Densi, file_out);

  
  //----------
  //--- f_y
  //----------
  printf(">>> Computing f_y... \n");
  sprintf(file_out, "%s_y.dvol", argv[2]);
  derivate_y_write(Densi, Header_Densi, file_out);


  //----------
  //--- f_z
  //----------
  printf(">>> Computing f_z... \n");
  sprintf(file_out, "%s_z.dvol", argv[2]);
  derivate_z_write(Densi, Header_Densi, file_out);



  //--- Free memory
  free(Densi);

  printf("\n\n>>> Clean exit from program!\n"); fflush(stdout);

  exit(0);

}


//==================================================
//
//==================================================
void derivate_x_write(double *Densi, struct Header Header, char *fname){
  int i,j,k;
  int i0,i1;
  int ind, ind0, ind1;
  FILE *file_pointer;
  double tempd;
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
	dump = fwrite(&tempd,sizeof(double),1, file_pointer);

      } //--- end for k
    } //--- end for j
  } //--- end for i

  fclose(file_pointer);

}


//==================================================
//
//==================================================
void derivate_y_write(double *Densi, struct Header Header, char *fname){
  int i,j,k;
  int j0,j1;
  int ind, ind0, ind1;
  FILE *file_pointer;
  double tempd;
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
	dump = fwrite(&tempd,sizeof(double),1, file_pointer);

      } //--- end for k
    } //--- end for j
  } //--- end for i

  fclose(file_pointer);

}


//==================================================
//
//==================================================
void derivate_z_write(double *Densi, struct Header Header, char *fname){
  int i,j,k;
  int k0,k1;
  int ind, ind0, ind1;
  FILE *file_pointer;
  double tempd;
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
	dump = fwrite(&tempd,sizeof(double),1, file_pointer);


      } //--- end for k
    } //--- end for j
  } //--- end for i
  
  fclose(file_pointer);

}

