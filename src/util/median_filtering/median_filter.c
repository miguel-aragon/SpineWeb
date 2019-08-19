/*

NAME:    Median_filter

DESCRIPTION:
   Median filter

COMPILE:
   gcc -o median_filter median_filter.c shell.c remove_peaks.c io_files.c  -lm

USAGE:
   median_filter input_file threshold_value output_file

HISTORY:
   27-01-2008 Created

Written by Miguel Angel Aragon Calvo.
Write comments to: miguel@pha.jhu.edu, miguel@kapteyn.astro.rug.nl
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "remove_peaks.h"
#include "io_files.h"


//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  
  int    i,j,k;                      //--- General variables
  double *density;                   //--- Data arrays
  int    sizeG, sizeX,sizeY,sizeZ;   //--- Grid size
  float  offsX,offsY,offsZ;          //--- Offset on grid
  int    n_cell;
  int    threshold;


  if (argc != 4){
    printf("\nUsage:\n");
    printf(" median_filter input_file threshold_value output_file\n\n");
    exit(0);
  }

  //-- Read threshold from command line
  threshold = (int) atoi(argv[2]);

  //--- Read denstiy file and allocate memory
  printf("Reading from file...\n"); fflush(stdout);
  read_volume_double(argv[1], &density, &sizeG, &sizeX, &sizeY, &sizeZ, &offsX, &offsY, &offsZ);

  //--- Remove peaks
  printf("Remove peaks...\n"); fflush(stdout);
  remove_peaks(density, sizeX,sizeY,sizeZ, threshold);

  //--- Write to file
  printf("Write to file...\n"); fflush(stdout);
  write_volume_double(argv[3], density, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Free allocated memory
  printf("Free alocated memory...\n"); fflush(stdout);
  free(density);

  printf("End of program\n"); fflush(stdout);


}



