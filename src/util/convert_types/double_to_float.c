/*

   NAME: 
      DOUBLE_TO_FLOAT


   DESCRIPTION:
      Convert datacube of DUOBLE type to FLOAT


   COMPILE AS:

       gcc -O3 -o double_to_float double_to_float.c io_files.c

   NOTES:

   HISTORY:
       2008-03-06
 

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_files.h"

//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- General variables
  int    i,j;
  FILE   *file_inp;
  FILE   *file_out;
  //--- Header
  int    datatype,gridtype;
  int    sizeG,sizeX,sizeY,sizeZ;  
  float  offsX,offsY,offsZ;
  int    remaining_bytes;
  char   dummy_byte;
  float  *data_buffer;
  int    n_buffer;
  float  tempf;
  double tempd;

  if (argc < 2) {
    printf("------------------------------------------ \n");
    printf(" Usage:\n");
    printf("     ./double_to_float file_double file_float \n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }

  //--- Read header of file
  printf("Read header from file...\n"); fflush(stdout);
  read_volume_head(argv[1], &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Open files
  if(!(file_inp=fopen(argv[1],"r"))) { printf("Can't open file '%s'\n",argv[1]); exit(0); }
  if(!(file_out=fopen(argv[2],"w"))) { printf("Can't open file '%s'\n",argv[2]); exit(0); }

  //--- Copy header
  read_head_variables(file_inp, &datatype, &gridtype, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);
  write_head_variables(file_out, 2, 0, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Copy rest of header
  remaining_bytes = 256-36;
  for (i=0; i<remaining_bytes; i++) {
    fread( &dummy_byte,sizeof(char),1,file_inp);
    fwrite(&dummy_byte,sizeof(char),1,file_out); 
  }

  //--- Allocate data buffer. This is to avoid excessive load on
  //    the hard disk. Maybe irrelevant...
  n_buffer = sizeY*sizeZ;
  if ((data_buffer = (float *) malloc(sizeof(float)*n_buffer)) == NULL) {
    printf("Error allocating data buffer, exiting now...\n");
    exit ( 1 );}


  //--- Copy data in planes (data buffer)
  for (i=0; i<sizeX; i++) {
    //--- Read data to buffer
    for (j=0; j<n_buffer; j++) {
      fread(&tempd, sizeof(double), 1, file_inp);
      data_buffer[j] = (float) tempd;
    }
    //--- Write data from buffer
    for (j=0; j<n_buffer; j++) {
      tempf = data_buffer[j];
      fwrite(&tempf,sizeof(float),1, file_out);
    }
  }

  //--- Close files
  fclose(file_inp);
  fclose(file_out);

  printf("Finish copying file!\n");
 
}



