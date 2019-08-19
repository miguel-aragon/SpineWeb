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
#include "../inc/get_wall_groups.h"

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
int *get_wall_groups(int *voids, char *spine, int sizeX, int sizeY, int sizeZ){
  long   i,j,k,w;
  //--- Shell
  int   n_shell = 27;
  int  *shell_ind = malloc(n_shell*sizeof(int));  
  int   shell_array[n_shell];

  //--- Wall pixels
  int   *wall_pix=NULL;
  long  n_wall_pix=0;
  //--- Wall cube
  int   *walls=NULL;

  //--- We are going to store the tuple here
  tuple *pix_tuple=NULL;

  //--- Loop over all pixels and count wall pixels
  long n_pix = sizeX * sizeY * sizeZ;
  for (i=0; i<n_pix; i++) if (spine[i] == 2) n_wall_pix++;

  //--- Allocate memory for tuple
  if ((pix_tuple = (tuple *) malloc(sizeof(tuple)*n_wall_pix )) == NULL){
      printf(">>> ERROR: malloc failed for walls pixels array. Exit program... \n\n");
      exit (1);
    }  

  //--- Allocate memory for wall pixels
  if ((wall_pix = (int *) malloc(sizeof(int)*n_wall_pix )) == NULL) {
      printf(">>> ERROR: malloc failed for walls pixels array. Exit program... \n\n");
      exit (1);
    }

  //--- Get indexes of pixels in walls
  long cont=0;
  for (i=0; i<n_pix; i++) {
    if (spine[i] == 2) {      
      wall_pix[cont] = i;
      cont++;
    }
  }

  printf(">>> %ld pixels in walls", n_wall_pix);
  
  double xg = (double) sizeX;
  double yg = (double) sizeY;
  double zg = (double) sizeZ;
  //--- Loop over wall pixels
  int ind_ijk[3];
  for (i=0; i<n_wall_pix; i++) {

    long ind_i = wall_pix[i];

    //--- Linear to three-dimensional indexes
    decomposed_index3d(ind_ijk, ind_i, xg,yg,zg);

    //--- Get shell around [xgrid,ygrid,zgrid] cell 
    shell_27(shell_ind, ind_ijk[0], ind_ijk[1], ind_ijk[2], sizeX,sizeY,sizeZ);

    //--- Copy adjacent pixels to array
    for(w=0; w<27; w++) shell_array[w] = voids[shell_ind[w]];

    long v0;
    long v1;
    //--- Give first non-zero value
    for(w=0; w<27; w++) {
      if(shell_array[w] != 0){
	v0 = shell_array[w];
	break;
      }
    }
    //--- Get second void
    for(w=0; w<27; w++){
      if (shell_array[w] != 0){
	if (shell_array[w] != v0){
	  v1 = shell_array[w];
	}
      }
    }
    
    long vv0=v0;
    long vv1=v1;
    if (v0 > v1){
      vv0 = v1;
      vv1 = v0;
    }

    pix_tuple[i].ind = ind_i;
    pix_tuple[i].v0 = vv0;
    pix_tuple[i].v1 = vv1;

    
  } //--- end for i
  free(shell_ind);

  //--- Sort tuples with their adjacent voids
  qsort(pix_tuple, n_wall_pix, sizeof(tuple), compmi);

  //--- Assign groups, track when the indexes change
  tuple pix_tuple_old = {-1, 0, pix_tuple[0].v0,pix_tuple[0].v1};
  int   cont_group=0;
  for (i=0; i<n_wall_pix; i++){

    if ( pix_tuple_old.v0 != pix_tuple[i].v0 || pix_tuple_old.v1 != pix_tuple[i].v1){
      cont_group++;
      pix_tuple[i].group = cont_group;
      pix_tuple_old.v0 = pix_tuple[i].v0;
      pix_tuple_old.v1 = pix_tuple[i].v1;
    } // if
    pix_tuple[i].group = cont_group;
    
  } // for i

  //--- Allocate walls cube and fill
  if ((walls = (int *) malloc(sizeof(int)*sizeX*sizeY*sizeZ )) == NULL) {
      printf(">>> ERROR: malloc failed for boundaries. Exit program... \n\n");
      exit (1);
    }
  for (i=0; i<(sizeX*sizeY*sizeZ); i++) walls[i] = -1;

  //--- Store wall grup picles in cube
  for (i=0; i<n_wall_pix; i++) walls[pix_tuple[i].ind] = pix_tuple[i].group;

  return walls;
}


//--------------------------------------------------------
//
//--------------------------------------------------------
static int compmi(const void *m1, const void *m2) {
  struct tuple_ *mi1 = (struct tuple_ *) m1;
  struct tuple_ *mi2 = (struct tuple_ *) m2;

  if (mi1->v0 <  mi2->v0) return -1;
  if (mi1->v0 >  mi2->v0) return  1;
  if (mi1->v0 == mi2->v0) {
    if (mi1->v1 <  mi2->v1) return -1;
    if (mi1->v1 >  mi2->v1) return  1;
    if (mi1->v1 == mi2->v1) return  0;
  }
}
