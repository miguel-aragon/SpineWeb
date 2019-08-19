/*

   NAME: 
       MERGE_VOIDS

   DESCRIPTION:
       Merge voids in scale space. This is the first step in the void hierarchy.

   OPTIONS:
       NOFIX -> Don't fix the region's IDs to be increasing starting at 0.

   COMPILE AS:


   HISTORY:
       2008-12-08 Ported from IDL code.
       01/2017 Major rewrite as part of version 2.2

   Written by: Miguel Angel Aragon Calvo. 
	       miguel.angel.aragon.calvo@gmail.com
	       maragon@astro.unam.mx

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../lib/header_struct.h"
#include "../lib/io_files.h"
#include "inc/water_hierarchy.h"


int main(int argc, char **argv)
{
  //--- Useful variables
  int i;
  //--- Data arrays
  int    *Water1      = NULL;
  int    *Water2      = NULL;
  int    *Water12     = NULL;
  //--- Headers
  struct Header Header_Wat1;
  struct Header Header_Wat2;
  struct Header Header_Wat12;
  //--- Timer
  time_t t1,t2;
  (void) time(&t1);
  //--- Optional parameters
  int    FLAG_FIX_ID = 1;

  if (argc <= 2)
    {
      printf("------------------------------------------ \n");
      printf("          Merge watershed basins of different scales \n");
      printf("\n");
      printf(" Usage:\n");
      printf("     merge_void_simple WaterHigh.ireg WaterLow.ireg Merged.imer [-NOFIX]");
      printf("\n");
      printf("     By default fixes the watershed regions ID's so they increase from 0. \n");
      printf("     Use -NOFIX flag to keep whatever index was selected when mergint the voids (not reccomended).\n");
      printf("\n");
      printf("------------------------------------------ \n"); 
      fflush(stdout);
      exit(1); 
    }

  //--- Optional input parameters
  for (i=1; i < argc; i++) 
    if (strcmp( "-NOFIX", argv[i]) == 0){
      FLAG_FIX_ID = 0;
      printf(">>> NOFIX flag set \n"); fflush(stdout);
    }

  if (FLAG_FIX_ID != 0) printf(">>> FIX REGIONS \n"); fflush(stdout);


  //====================================
  //--- Read watershed from files
  //====================================
  Water1 = Read_DataCube_int(argv[1], &Header_Wat1);
  Water2 = Read_DataCube_int(argv[2], &Header_Wat2);

  //====================================
  //--- Merge scales
  //====================================
  printf(">>> Mixing regions 1 and 2 \n"); fflush(stdout);
  Water12 = merge_regions_int(Water1, Water2, Header_Wat1.sizeX*Header_Wat1.sizeY*Header_Wat1.sizeZ);

  //====================================
  //--- Fix indexes of regions after merging
  //====================================
  if (FLAG_FIX_ID != 0) {
    printf(">>> Fixing ID's of regions \n"); fflush(stdout);
    fix_watershed_IDs(Water12,Header_Wat1.sizeX*Header_Wat1.sizeY*Header_Wat1.sizeZ);
  }

  //====================================
  //--- Write merged watershed to file
  //====================================
  Write_DataCube_int(argv[3], &Header_Wat1, Water12);

  //--- Free memory
  free(Water1);
  free(Water2);
  free(Water12);


  //--- Check time and print
  (void) time(&t2);
  printf("\n>>> Computing time = %d seconds\n", (int) t2 - (int) t1);

  printf("\n\n>>> Clean exit from program!\n"); fflush(stdout);

  exit(0);

}


