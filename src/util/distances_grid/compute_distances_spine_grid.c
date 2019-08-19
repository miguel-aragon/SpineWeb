/*

   NAME: 


   DESCRIPTION:  Compute distances from the spine for all voxels in the image. 
                 The distances are not euclidean.
		 User can specify SPINE, WALLS or FILAMENTS.


   NOTES:


   COMPILE:
 
       gcc -O3 -o compute_distances_spine_grid compute_distances_spine_grid.c io_files.c get_distance.c pixel3.c numeric.c -lm

   HISTORY:
       2007      Created
       June/2009 Include distances from spine/wall/filaments

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl
	       
	   

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "io_files.h"
#include "get_distance.h"

//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- General variables
  int  i,j,k;
  //--- Data arrays
  char  *skeleton;
  char  *distances;
  int   sizeG,sizeX,sizeY,sizeZ;  //--- Grid size
  float offsX,offsY,offsZ;
  //--- Type of distance
  int   type_dist =0;
  int   type_dista=0;

  if (argc != 5) {
    printf("--------------------------------------------------------------- \n");
    printf("           Compute distances using a 3D kernel \n");
    printf("\n");
    printf(" Usage:\n");
    printf("     ./compute_distances [distance type] [type] <file.cspi> <file.cdis>\n");
    printf("\n");
    printf("     Where [distance type] indicates the type of distance:\n");
    printf("         0 = square         kernel\n");
    printf("         1 = semi-spherical kernel\n");
    printf("\n");
    printf("     Where [type] indicates the type of distance:\n");
    printf("         0 = Spine (filaments and walls)\n");
    printf("         1 = Walls\n");
    printf("         2 = Filaments\n");
    printf("\n");
    printf("--------------------------------------------------------------- \n");
    exit(0); }

  //--- Get the type of distance and validate
  type_dista = atoi(argv[1]);
  type_dist  = atoi(argv[2]);
  if (type_dist > 2) { 
    printf(">>> Invalid spine distance!!!\n");
    exit(0);
  }

  //--- Some info
  if (type_dista == 0) printf("Computing square distances\n");
  if (type_dista == 1) printf("Computing semi-spherical distances\n");

  if (type_dist == 0) printf("Computing distances from spine (walls+filaments)\n");
  if (type_dist == 1) printf("Computing distances from walls\n");
  if (type_dist == 2) printf("Computing distances from filaments\n");
  printf("\n");

  //--- Read header of file
  printf("Read header from file...\n"); fflush(stdout);
  read_volume_head(argv[3], &sizeG, &sizeX,&sizeY,&sizeZ, &offsX,&offsY,&offsZ);

  //--- Allocate memory for working arrays
  printf("Allocating memory...\n"); fflush(stdout);
  skeleton      = (char *)   malloc(sizeof(char) *sizeX*sizeY*sizeZ);
  distances     = (char *)   malloc(sizeof(char) *sizeX*sizeY*sizeZ);

  //--- Read denstiy file
  printf("Reading file...\n"); fflush(stdout);
  read_volume_char(argv[3], skeleton, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);

  //--- Compute distances from skeleton
  printf("Computing distances from skeleton...\n"); fflush(stdout);
  if (type_dista == 0) get_distance(skeleton, distances, type_dist, sizeX,sizeY,sizeZ);
  if (type_dista == 1) get_distance_sph(skeleton, distances, type_dist, sizeX,sizeY,sizeZ);

  //--- Write distances to file
  printf("Writting distances to file %s...\n", argv[4]); fflush(stdout);
  write_volume_char(argv[4], distances, sizeG, sizeX,sizeY,sizeZ, offsX,offsY,offsZ);


  //--- Free allocated memory
  printf("Free alocated memory...\n");fflush(stdout);
  fflush(stdout);
  free(skeleton);
  free(distances);

  printf("End of program\n");
  fflush(stdout);



}



