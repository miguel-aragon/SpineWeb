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
#include "inc/get_filament_groups.h"


//===============================================================
//                 MAIN FUNCTION
//===============================================================
int main(int argc, char **argv)
{
  //--- Data arrays
  int  *water=NULL;
  char *spine=NULL;
  int  *filas=NULL;
  //--- Headers
  struct Header Header_Wat;


  if (argc != 4) {
    printf("------------------------------------------ \n");
    printf(" spine_filament_catalogues \n");
    printf("\n");
    printf("    Compute filament groups by grouping filament pixels with the same\n");
    printf("      3 voids. Filament groups start at id=0, -1 are pixels not in filaments.\n");
    printf("\n");
    printf("    NOTES:\n");   
    printf("       Must run spine_wall_catalogues first!!!\n");
    printf(" Usage:\n");
    printf("     ./spine_filament_catalogues  watershed.iwat  spine.cspi filament_catalogues.dat \n");
    printf("\n");
    printf("------------------------------------------ \n");
    exit(0); }


  //====================================
  //--- Read watershed from files
  //====================================
  water = Read_DataCube_int(argv[1], &Header_Wat);

  //====================================
  //--- Read spine
  //====================================
  spine = Read_DataCube_char(argv[2], &Header_Wat);

  //====================================
  //--- Get wall catalogues
  //====================================
  printf(">>> ...\n"); fflush(stdout);
  filas = get_filament_groups(water, spine, Header_Wat.sizeX,Header_Wat.sizeY,Header_Wat.sizeZ);

  //====================================
  //--- Write spine to file
  //====================================
  Header_Wat.datatype = 5; //--- char type
  Write_DataCube_int(argv[3], &Header_Wat, filas);

  //====================================
  //--- Free allocated memory
  //====================================
  printf(">>> Free alocated memory...\n"); fflush(stdout);

  free(water);
  free(spine);
  free(filas);

  exit(0);

}



