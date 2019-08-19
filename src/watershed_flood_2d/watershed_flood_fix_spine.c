/*

    SPINE code. Identify Structures in the Cosmic Web.
    Copyright (C) 2012-2017 Miguel Angel Aragon-Calvo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    If you wish to contact the author write to: 
        miguel.angel.aragon.calvo@gmail.com
	maragon@astro.unma.mx

   NAME: 

   DESCRIPTION:

   COMPILE AS:

   NOTES:

   HISTORY:
     12/2008: Ported from IDL code
     01/2017: Major rewrite version 2.0


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../lib/header_struct.h"
#include "../lib/io_files.h"
#include "inc/watershed_spine.h"


//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- General variables
  int     i;
  //--- Data arrays
  char   *boundaries=NULL;
  double *density_double=NULL;
  float  *density_float=NULL;

  //--- Headers
  struct Header Header_Den;
  struct Header Header_Wat;
  //--- Optional parameters
  int    FLAG_FLOAT = 0;


  if (argc != 4) {
    printf("------------------------------------------ \n");
    printf(" watershed_flood_fix_spine \n");
    printf("\n");
    printf("\n");
    printf("    Complete the watershed tranform from watershed regions and density field.\n");
    printf("    This new version gives watershed trasnform the index=-1 instead of 0 as before.\n");
    printf("\n");
    printf(" Usage:\n");
    printf("     ./watershed_flood_fix_spine  watershed_regions.ireg density.dvol watershed_transform.iwat [-FLOAT]\n");
    printf("     Use FLOAT flag for floating point reading/writing.\n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }

  //--- Optional input parameters
  for (i=1; i < argc; i++) 
    if (strcmp( "-NOFIX", argv[i]) == 0){
      FLAG_FLOAT = 1;
      printf(">>> NOFIX FLOAT flag set. Be sure to provide floating point input files.\n"); fflush(stdout);
    }

  //====================================
  //--- Read watershed from files
  //====================================
  int    *water=NULL;
  water = Read_DataCube_int(argv[1], &Header_Wat);


  //====================================
  //--- Read density field
  //====================================
  if (FLAG_FLOAT == 0) density_double = Read_DataCube_double(argv[2], &Header_Den);
  else                 density_float  = Read_DataCube_float( argv[2], &Header_Den);


  //====================================
  //--- Get boundaries between voids
  //====================================
  printf(">>> Getting boundaries between voids...\n"); fflush(stdout);
  boundaries = get_void_boundaries(water, Header_Wat.sizeX,Header_Wat.sizeY,Header_Wat.sizeZ);


  //====================================
  //--- Do local flooding watershed
  //====================================
  printf(">>> Apply flooding watershed locally on void boundaries...\n"); fflush(stdout);
  if (FLAG_FLOAT == 0) local_watershed_sort_double(density_double, water, boundaries, Header_Wat.sizeX,Header_Wat.sizeY,Header_Wat.sizeZ);
  else                 local_watershed_sort_float(density_float,   water, boundaries, Header_Wat.sizeX,Header_Wat.sizeY,Header_Wat.sizeZ);


  //====================================
  //--- Write merged watershed to file
  //====================================
  Write_DataCube_int(argv[3], &Header_Wat, water);


  //====================================
  //--- Free allocated memory
  //====================================
  printf(">>> Releasing alocated memory...\n"); fflush(stdout);
  free(water);
  free(boundaries);
  if (FLAG_FLOAT == 0) free(density_double);
  else                 free(density_float);
  
 
  exit(0);

}



