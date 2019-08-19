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
char *count_adjacent_voids(int *voids, int sizeX, int sizeY, int sizeZ)
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

	//--- If pixel not in skeleton then skip
	if (voids[shell_ind[0]] != 0) continue;
	
	//--- Copy adjacent pixels to array
	for(w=0; w<27; w++) shell_array[w] = voids[shell_ind[w]];

	//--- Count number of different elements in neighbourhood
	region_count = count_different_int(shell_array, n_shell);
	
	//--- Store the number of adjacent regions. Substract 1 to
	//    account for the wall itself
	spine[shell_ind[0]] = region_count-1;

	//--- Special cases:
	//    We are already in the watershed boundaries so no need to check for that.
	//    If there are less than 3 regions (void1 - wall - void2) then
	//    we must be deep into the spine or boundary of thick wall. Set the count to a 
	//    large number as a flag
	//--- 1).- Pixels deep into the spine (only adjacent with 0's) 
	if (region_count == 1) spine[shell_ind[0]] = 254;
	//    2).- Pixels in thick walls (2 adjacent regions)
	if (region_count == 2) spine[shell_ind[0]] = 255;
	//--- Old criteria:
	//if (region_count < 3) spine[shell_ind[0]] = 255;

      }
    }
  }


  free(shell_ind);

  return spine;

}
