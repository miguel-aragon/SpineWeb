/*

   NAME: 
       MERGE_VOIDS

   DESCRIPTION:
       Merge voids in scale space. This is the first step in the void hierarchy.

   COMPILE AS:


   HISTORY:
       2008-12-08 Ported from IDL code.

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "../lib/header_struct.h"
#include "../lib/io_files.h"
#include "inc/voids_tree.h"


int main(int argc, char **argv)
{
  //--- Data arrays
  double *Densi       =NULL;
  int    *Water1      =NULL;
  int    *Water2      =NULL;
  int    *Water3      =NULL;
  //--- Headers
  struct Header Header_Den;
  struct Header Header_Wat1;
  struct Header Header_Wat2;
  struct Header Header_Wat3;

  //--- Timer
  time_t t1,t2;
  (void) time(&t1);


  if (argc != 4)
    {
      printf("------------------------------------------ \n");
      printf("          Merge watershed basins of different scales \n");
      printf("\n");
      printf(" Usage:\n");
      printf("     ./watershed_merge Water_Scl2.ivol WaterScl1.ivol WaterScl0.ivol");
      printf("\n");
      printf("      where the scale goes as: Scl0 > Scl1 > Scl2 \n");
      printf("\n");
      printf("------------------------------------------ \n"); 
      fflush(stdout);
      exit(1); 
    }

  //====================================
  //--- Read watershed from files
  //====================================
  Water1 = Read_DataCube_int(argv[1], &Header_Wat1);
  Water2 = Read_DataCube_int(argv[2], &Header_Wat2);
  Water3 = Read_DataCube_int(argv[3], &Header_Wat3);

  //====================================
  //--- Fill tree
  //====================================
  fill_void_tree(Water1, Water2, Water3, Header_Wat1.sizeX*Header_Wat1.sizeY*Header_Wat1.sizeZ);



  //--- Free original watersheds from memory
  free(Water1);
  free(Water2);
  free(Water3);


  printf("\n\n>>> Clean exit from program!\n"); fflush(stdout);

  //--- Check time and print
  (void) time(&t2);
  printf("\n>>> Computing time = %d seconds\n", (int) t2 - (int) t1);


  exit(0);

}

