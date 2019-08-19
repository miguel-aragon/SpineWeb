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
#include "inc/water_hierarchy.h"

struct Flags {
  //---  Flags
  char   Sampling;           //--- Use sampling points instead of data points
  char   WindowType;         //--- Type of window: Tophat or Gaussian
  char   Weights;            //--- Read weights from file
  //---  Files
  char   file_input1  [256];
  char   file_input2  [256];
  char   file_merged  [256];
  char   file_weights [256];
 } flags;     //--- Pointer to structure



void command_line_parser(int argc, char **argv, struct Flags *flags);

int main(int argc, char **argv)
{
  //--- Data arrays
  double *Densi       =NULL;
  int    *Water1      =NULL;
  int    *Water2      =NULL;
  int    *Water12     =NULL;
  int    *Water_merged=NULL;
  //--- Headers
  struct Header Header_Den;
  struct Header Header_Wat1;
  struct Header Header_Wat2;
  struct Header Header_Wat12;
  struct Header Header_Mer;
  //--- Timer
  time_t t1,t2;
  (void) time(&t1);



  //====================================
  //--- Read watershed from files
  //====================================
  Water1 = Read_DataCube_int(argv[1], &Header_Wat1);
  Water2 = Read_DataCube_int(argv[2], &Header_Wat2);

  //====================================
  //--- Merge scales
  //====================================
  printf(">>> Mixing regions 1 and 2 \n"); fflush(stdout);
  //Water12 = merge_regions_int(Water1, Water2, Header_Wat1.sizeX*Header_Wat1.sizeY*Header_Wat1.sizeZ);
  Water12 = merge_regions_grid_int(Water1, Water2, Header_Wat1.sizeX*Header_Wat1.sizeY*Header_Wat1.sizeZ,Header_Wat1.sizeX,Header_Wat1.sizeY,Header_Wat1.sizeZ);

  //====================================
  //--- Read density field
  //====================================
  Densi = Read_DataCube_double(argv[3], &Header_Den);

  //====================================
  //--- Compute watershed with markers
  //====================================
  printf(">>> Compute watershed with markers \n"); fflush(stdout);
  Water_merged = watershed_sorted_int_27_marked(Densi, Water12, Header_Wat1.sizeX, Header_Wat1.sizeY, Header_Wat1.sizeZ);

  //====================================
  //--- Fix indexes of regions after merging
  //====================================
  printf(">>> Fixing ID's of regions \n"); fflush(stdout);
  fix_watershed_IDs(Water_merged,Header_Wat1.sizeX*Header_Wat1.sizeY*Header_Wat1.sizeZ);

  //====================================
  //--- Write merged watershed to file
  //====================================
  Write_DataCube_int(argv[4], &Header_Wat1, Water12);
  Write_DataCube_int(argv[5], &Header_Wat1, Water_merged);


  //--- Free memory
  free(Water1);
  free(Water2);
  free(Water12);
  free(Densi);
  free(Water_merged);


  //--- Check time and print
  (void) time(&t2);
  printf("\n>>> Computing time = %d seconds\n", (int) t2 - (int) t1);

  printf("\n\n>>> Clean exit from program!\n"); fflush(stdout);

  exit(0);

}



//========================================
//--- 
//========================================
void command_line_parser(int argc, char **argv, struct Flags *flags)
{

  int i;

  //-------------------------------
  //--- Default options
  //-------------------------------
  flags->N_Threads = 1;

  //-------------------------------
  //--- Read command line options
  //-------------------------------
  printf("\n-------------------------------------------------\n");
  for (i = 1; i < argc; i++) { 
    //--- OUTPUT FILE
    if (strcmp( "-o", argv[i]) == 0){
      sscanf(argv[i+1],"%s",&flags->file_output);
      printf(">>>     Output file = %s \n", flags->file_output);
    }
    //--- INPUT POSITIONS
    if (strcmp( "-i", argv[i]) == 0){
      sscanf(argv[i+1],"%s",&flags->file_input);
      printf(">>>     Input file = %s \n", flags->file_input);
    }
    //--- READ WINDOW SIZE
    else if      (strcmp( "-w", argv[i]) == 0){
      sscanf(argv[i+1],"%lf",&flags->WindowSize);
      printf(">>>     Window size = %lf\n", flags->WindowSize);
    }
    //--- READ GRID SIZE FOR INDEXING
    else if (strcmp( "-g", argv[i]) == 0){
      sscanf(argv[i+1],"%d",&flags->GridSize);  
      printf(">>>     Grid   size = %d\n", flags->GridSize);
    }
    //--- WINDOW TYPE
    else if (strcmp( "-t", argv[i]) == 0){
      flags->WindowType = 1;
      printf(">>> [i] TopHat window\n");
    }
    //--- SAMPLING FILE
    else if (strcmp( "-s", argv[i]) == 0){
      flags->Sampling = 1;
      sscanf(argv[i+1],"%s",&flags->file_sampling);  
      printf(">>>     Sampling file: '%s'\n",flags->file_sampling);
    }
    //--- WEIGHTS
    else if (strcmp( "-p", argv[i]) == 0){
      flags->Weights = 1;
      sscanf(argv[i+1],"%s",&flags->file_weights);
      printf(">>>     Weights file: '%s'\n",flags->file_weights);
    }
  }


  if (argc != 6)
    {
      printf("------------------------------------------ \n");
      printf("          Merge watershed basins of different scales \n");
      printf("\n");
      printf(" Usage:\n");
      printf("     ./watershed_merge WaterHigh.ivol WaterLow.ivol DensiHigh.dvol Mixed.ivol Merged.ivol");
      printf("                                -i [Input file]    \n");
      printf("                                -o [Output file]   \n");
      printf("                                -w [Window size]   \n");
      printf("\n");
      printf("------------------------------------------ \n"); 
      fflush(stdout);
      exit(1); 
    }


}
