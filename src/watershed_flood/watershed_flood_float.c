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
       watershed_flood

   DESCRIPTION:

   COMPILE AS:


   HISTORY:
       2008-12-08 Ported from IDL code.
       2107-01-24 Made floating point version

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl
	       miguel.angel.aragon.calvo@gmail.com

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "../lib/header_struct.h"
#include "../lib/io_files.h"
#include "../lib/numeric.h"
#include "../lib/get_closest.h"
#include "inc/water_functions_uphill.h"


int main(int argc, char **argv)
{
  //--- Data arrays
  float        *density=NULL;
  int          *voids  =NULL;
  //-- Header
  struct Header Header;

  if (argc != 3)
    {
      printf("------------------------------------------ \n");
      printf("          watershed uphill algorithm. \n");
      printf("\n");
      printf(" USAGE:\n");
      printf("     ./watershed_uphill density.fvol watershed.ivol \n");
      printf("\n");
      printf(" NOTES: \n");
      printf("    - Input file is type is float\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("\n");
      printf("------------------------------------------ \n"); 
      fflush(stdout);
      exit(1); 
    }


  //====================================
  //--- Read density field
  //====================================
  printf(">>> Reading density file\n"); fflush(stdout);
  density = Read_DataCube_float(argv[1],&Header);


  //====================================
  //--- Perform watershed transform
  //====================================
  printf(">>> Watershed transform\n"); fflush(stdout);
  voids = watershed_uphill_27_float(density, Header.sizeX, Header.sizeY, Header.sizeZ);


  //====================================
  //--- Write watershed to file
  //====================================
  //--- Fix header
  printf(">>> Writing watershed to file\n"); fflush(stdout);
  Header.datatype = 5;
  Header.gridtype = 3;
  Write_DataCube_int(argv[2], &Header, voids);


  free(density);
  free(voids);

  printf(">>> Clean exit from program!\n"); fflush(stdout);

  exit(0);

}


