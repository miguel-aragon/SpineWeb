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

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../lib/pixel3.h"
#include "../../lib/numeric.h"


//==========================================
//   NAME:
//       count_adjacent_voids
//   DESCRIPTION:
//       This is the main SpineWeb idea. ID the pixel is
//       part of the watershed transform then count the
//       number of adjacent voids. 
//
//       UPDATE: I replaced the notion of counting voids
//       to identify filaments for the more logical one of
//       counting adjacent walls. Still the spine gives a
//       good first indication of filaments.
//
//   NOTES:
//       The watershed transform has ID = 0.
//==========================================
char *count_adjacent_regions(int *voids, int sizeX, int sizeY, int sizeZ)
{
  int   i,j,k,w;
  //--- Shell
  int   n_shell = 27;
  int  *shell_ind = malloc(n_shell*sizeof(int));  
  int   shell_array[n_shell];
  //--- Spine
  char *spine=NULL;
  int   region_count;

  //--- Allocate memory for spine
  if ((spine = (char *) malloc(sizeof(char)*sizeX*sizeY*sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed for boundaries. Exit program... \n\n");
      exit (1);
    }

  //--- Loop over grid cells and count adjacent voids around pixel...
  for(i=0; i<sizeX; i++){
    for(j=0; j<sizeY; j++){
      for(k=0; k<sizeZ; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, sizeX,sizeY,sizeZ);	
	
	//--- Copy adjacent pixels to array
	for(w=0; w<27; w++) shell_array[w] = voids[shell_ind[w]];

	//--- Count number of different elements in neighbourhood
	region_count = count_different_int(shell_array, n_shell);
	
	//--- Store the number of adjacent regions
	spine[shell_ind[0]] = region_count;

      }
    }
  }


  free(shell_ind);

  return spine;

}
