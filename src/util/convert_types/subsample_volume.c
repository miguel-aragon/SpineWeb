/*

   NAME: 
      SUBSAMPLE_VOLUME


   DESCRIPTION:
      Subsamples a given interval inside a volume


   COMPILE AS:

       icc -o subsample_volume subsample_volume.c io_files.c

   NOTES:

   HISTORY:
       2008-07-16
 

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
  int    i,j,k;
  int    x0,x1,y0,y1,z0,z1;
  FILE   *file_inp;
  FILE   *file_out;
  //--- Header
  int    datatype,gridtype;
  int    sizeG,sizeX,sizeY,sizeZ;  
  int    sizeX2,sizeY2,sizeZ2;
  float  offsX,offsY,offsZ;
  float  offsX2,offsY2,offsZ2;
  int    remaining_bytes;
  char   dummy_byte;
  double *data_buffer;
  int    n_buffer;
  float  tempf;
  double tempd;

  if (argc < 9) {
    printf("------------------------------------------ \n");
    printf(" Usage:\n");
    printf("     ./subsample_volume input_file[double] x0 x1 y0 y1 z0 z1 output_file[float] \n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }

  x0 = atoi(argv[2]);
  x1 = atoi(argv[3]);
  y0 = atoi(argv[4]);
  y1 = atoi(argv[5]);
  z0 = atoi(argv[6]);
  z1 = atoi(argv[7]);

  sizeX2 = x1-x0+1;
  sizeY2 = x1-x0+1;
  sizeZ2 = x1-x0+1;
  offsX2 = x0;
  offsY2 = y0;
  offsZ2 = z0;


  //--- Read header of file
  printf("Read header from file...\n"); fflush(stdout);
  read_volume_head(argv[1], &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Open files
  if(!(file_inp=fopen(argv[1],"r"))) { printf("Can't open file '%s'\n",argv[1]); exit(0); }
  if(!(file_out=fopen(argv[8],"w"))) { printf("Can't open file '%s'\n",argv[2]); exit(0); }

  //--- Copy header
  read_head_variables(file_inp, &datatype, &gridtype, &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);
  write_head_variables(file_out, 10, 0, sizeG, sizeX2,sizeY2,sizeZ2, offsX2,offsY2,offsZ2);

  //--- Copy rest of header
  remaining_bytes = 256-36;
  for (i=0; i<remaining_bytes; i++) {
    fread( &dummy_byte,sizeof(char),1,file_inp);
    fwrite(&dummy_byte,sizeof(char),1,file_out); 
  }

  //--- Copy data in planes (data buffer)
  for (i=0; i<sizeX; i++) {
    for (j=0; j<sizeY; j++) {
      for (k=0; k<sizeZ; k++) {

	//--- Read pixel

	fread(&tempd, sizeof(double), 1, file_inp);
	//--- Only voxels inside interval
	if(i >= x0 && i <= x1 && j >= y0 && j <= y1 && k >= z0 && k <= z1) {
	  tempf = (float) tempd;
	  fwrite(&tempf,sizeof(float),1, file_out);
	}
	
	}
      }
    }

  //--- Close files
  fclose(file_inp);
  fclose(file_out);

  printf("Finish copying file!\n");

}



