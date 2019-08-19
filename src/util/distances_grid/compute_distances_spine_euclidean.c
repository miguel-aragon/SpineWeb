/*

   NAME: 


   DESCRIPTION:


   COMPILE AS:


   NOTES:


   COMPILE:
 
       icc -o compute_distances_spine_euclidean compute_distances_spine_euclidean.c io_files.c get_euclidean_distance.c pixel3.c numeric.c -lm

   HISTORY:
 

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "io_files.h"
#include "get_euclidean_distance.h"

//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- General variables
  int  i,j,k;
  //--- Data arrays
  char  *skeleton;
  float *distances;
  int   sizeG,sizeX,sizeY,sizeZ;  //--- Grid size
  float offsX,offsY,offsZ;
  int   type_spine;

  if (argc != 4) {
    printf("--------------------------------------------------------------- \n");
    printf("           Watershed 3D \n");
    printf("\n");
    printf(" Usage:\n");
    printf("     ./compute_distances_spine_euclidean  spine.cspi spine_type distances.fvol \n");
    printf("\n");
    printf("      Where type is: wall = 0, filament = 1, wall+spine >= 2, filament+spine >= 3 \n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("--------------------------------------------------------------- \n");
    exit(0); }

  //--- Read header of file
  printf("Read header from file...\n"); fflush(stdout);
  read_volume_head(argv[1], &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Allocate memory for working arrays
  printf("Allocating memory...\n"); fflush(stdout);
  skeleton  = (char *)  malloc(sizeof(char)  *sizeX*sizeY*sizeZ);
  distances = (float *) malloc(sizeof(float) *sizeX*sizeY*sizeZ);

  //--- Read denstiy file
  printf("Reading file...\n"); fflush(stdout);
  read_volume_char(argv[1], skeleton, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Compute distances from skeleton
  printf("Computing distances from skeleton...\n"); fflush(stdout);
  type_spine = atoi(argv[2]);
  get_euclidean_distance_type(skeleton, distances, type_spine, sizeX,sizeY,sizeZ);

  //--- Write distances to file
  printf("Writting distances to file %s...\n", argv[3]); fflush(stdout);
  write_volume_float(argv[3], distances, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);


  //--- Free allocated memory
  printf("Free alocated memory...\n");fflush(stdout);
  fflush(stdout);
  free(skeleton);
  free(distances);

  printf("End of program\n");
  fflush(stdout);



}



