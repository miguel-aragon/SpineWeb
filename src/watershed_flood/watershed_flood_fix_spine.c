/*

    SPINEWEB code. Identify Structures in the Cosmic Web.
    Copyright (C) 2012 Miguel A. Aragon-Calvo

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
       miguel@pha.jhu.edu
       miguel.angel.aragon.calvo@gmail.com

   NAME: 

   DESCRIPTION:

   COMPILE AS:

   NOTES:

   HISTORY:

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
#include "inc/watershed_spine.h"


//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- General variables
  int     i,j,k;
  //--- Data arrays
  double *density=NULL;
  int    *water=NULL;
  char   *boundaries=NULL;
  //--- Headers
  struct Header Header_Den;
  struct Header Header_Wat;


  if (argc != 4) {
    printf("------------------------------------------ \n");
    printf("           watershed_flood_fix_spine \n");
    printf("\n");
    printf(" Usage:\n");
    printf("     ./watershed_3d  watershed_raw.ivol density.dvol watershed.ivol \n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }


  //====================================
  //--- Read watershed from files
  //====================================
  water = Read_DataCube_int(argv[1], &Header_Wat);


  //====================================
  //--- Read density field
  //====================================
  density = Read_DataCube_double(argv[2], &Header_Den);


  //====================================
  //--- Get boundaries between voids
  //====================================
  printf(">>> Getting boundaries between voids...\n"); fflush(stdout);
  boundaries = get_void_boundaries(water, Header_Wat.sizeX,Header_Wat.sizeY,Header_Wat.sizeZ);


  //====================================
  //--- Do local flooding watershed
  //====================================
  printf(">>> Apply flooding watershed locally on void boundaries...\n"); fflush(stdout);
  local_watershed_sort(density, water, boundaries, Header_Wat.sizeX,Header_Wat.sizeY,Header_Wat.sizeZ);


  //====================================
  //--- Write merged watershed to file
  //====================================
  Write_DataCube_int(argv[3], &Header_Wat, water);


  //====================================
  //--- Free allocated memory
  //====================================
  printf(">>> Free alocated memory...\n"); fflush(stdout);

  free(water);
  free(boundaries);
  free(density);
 
  exit(0);

}



