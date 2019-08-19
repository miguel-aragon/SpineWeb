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
#include "../inc/get_filament_groups.h"

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
int *get_filament_groups(int *voids, char *spine, int sizeX, int sizeY, int sizeZ){
  long   i,j,k,w;
  //--- Shell
  int   n_shell = 27;
  int  *shell_ind = malloc(n_shell*sizeof(int));  
  int   shell_array[n_shell];

  //--- Fila pixels
  int   *fila_pix=NULL;
  long  n_fila_pix=0;
  //--- Fila cube
  int   *filas=NULL;

  //--- We are going to store the triplet here
  triplet *pix_triplet=NULL;

  //--- Loop over all pixels and count filament pixels
  long n_pix = sizeX * sizeY * sizeZ;
  for (i=0; i<n_pix; i++) if (spine[i] == 3) n_fila_pix++;

  //--- Allocate memory for triplet
  if ((pix_triplet = (triplet *) malloc(sizeof(triplet)*n_fila_pix )) == NULL){
      printf(">>> ERROR: malloc failed for filas pixels array. Exit program... \n\n");
      exit (1);
    }  

  //--- Allocate memory for filament pixels
  if ((fila_pix = (int *) malloc(sizeof(int)*n_fila_pix )) == NULL) {
      printf(">>> ERROR: malloc failed for filas pixels array. Exit program... \n\n");
      exit (1);
    }

  //--- Get indexes of pixels in filas
  long cont=0;
  for (i=0; i<n_pix; i++) {
    if (spine[i] == 3) {      
      fila_pix[cont] = i;
      cont++;
    }
  }

  printf(">>> %ld pixels in filaments", n_fila_pix);
  
  double xg = (double) sizeX;
  double yg = (double) sizeY;
  double zg = (double) sizeZ;
  //--- Loop over fila pixels
  int ind_ijk[3];
  for (i=0; i<n_fila_pix; i++) {

    long ind_i = fila_pix[i];

    //--- Linear to three-dimensional indexes
    decomposed_index3d(ind_ijk, ind_i, xg,yg,zg);

    //--- Get shell around [xgrid,ygrid,zgrid] cell 
    shell_27(shell_ind, ind_ijk[0], ind_ijk[1], ind_ijk[2], sizeX,sizeY,sizeZ);

    //--- Copy adjacent pixels to array
    for(w=0; w<27; w++) shell_array[w] = voids[shell_ind[w]];

    //--- Get unique void id's. This can be optimized but for few pixels it is ok
    long v0, v1, v2;
    //--- Give first non-zero value
    for(w=0; w<27; w++)
      if(shell_array[w] != 0){
	v0 = shell_array[w];
	break;
      }
    //--- Get second void
    for(w=0; w<27; w++)
      if (shell_array[w] != 0)
	if (shell_array[w] != v0){
	  v1 = shell_array[w];
	  break;
	}
    //--- Get third void
    for(w=0; w<27; w++)
      if (shell_array[w] != 0)
	if (shell_array[w] != v0 && shell_array[w] != v1){
	  v2 = shell_array[w];
	  break;
	}   

    //--- Sort void id's
    long *vv0,*vv1,*vv2;
    vv0 = &v0;
    vv1 = &v1;
    vv2 = &v2;
    sort_3long(vv0,vv1,vv2);    
    pix_triplet[i].ind = ind_i;
    pix_triplet[i].v0 = *vv0;
    pix_triplet[i].v1 = *vv1;
    pix_triplet[i].v2 = *vv2;
    
  } //--- end for i
  free(shell_ind);

  //--- Sort triplets with their adjacent voids
  qsort(pix_triplet, n_fila_pix, sizeof(triplet), comp_3);

  //--- Assign groups, track when the indexes change
  triplet pix_triplet_old = {-1, 0, pix_triplet[0].v0,pix_triplet[0].v1};
  int   cont_group=0;
  for (i=0; i<n_fila_pix; i++){

    if ( pix_triplet_old.v0 != pix_triplet[i].v0 || pix_triplet_old.v1 != pix_triplet[i].v1){
      cont_group++;
      pix_triplet[i].group = cont_group;
      pix_triplet_old.v0 = pix_triplet[i].v0;
      pix_triplet_old.v1 = pix_triplet[i].v1;
    } // if
    pix_triplet[i].group = cont_group;
    
  } // for i

  //--- Allocate filas cube and fill
  if ((filas = (int *) malloc(sizeof(int)*sizeX*sizeY*sizeZ )) == NULL) {
      printf(">>> ERROR: malloc failed for boundaries. Exit program... \n\n");
      exit (1);
    }
  for (i=0; i<(sizeX*sizeY*sizeZ); i++) filas[i] = -1;

  //--- Store fila group particles in cube
  for (i=0; i<n_fila_pix; i++) filas[pix_triplet[i].ind] = pix_triplet[i].group;

  return filas;
}


//--------------------------------------------------------
//
//--------------------------------------------------------
static int comp_3(const void *m1, const void *m2) {
  struct triplet_ *mi1 = (struct triplet_ *) m1;
  struct triplet_ *mi2 = (struct triplet_ *) m2;

  //--- Simple cases
  if (mi1->v0 <  mi2->v0) return -1;
  if (mi1->v0 >  mi2->v0) return  1;

  if (mi1->v0 == mi2->v0) {
    //--- Simple cases
    if (mi1->v1 <  mi2->v1) return -1;
    if (mi1->v1 >  mi2->v1) return  1;

    if (mi1->v1 == mi2->v1) {
      //--- Simple cases
      if (mi1->v2 <  mi2->v2) return -1;
      if (mi1->v2 >  mi2->v2) return  1;
      if (mi1->v2 == mi2->v2) return  0;
    }
  }

}  // comp_3()


//--------------------------------------------------------
//
//--------------------------------------------------------
void compare_and_swap(long *a, long *b) {
    long temp;
    if (*a > *b) {
        temp = *a;
        *a   = *b;
        *b   = temp;
    }
}

//--------------------------------------------------------
//
//--------------------------------------------------------
void sort_3long(long *a, long *b, long *c) {
    compare_and_swap(a, b);
    compare_and_swap(a, c);
    compare_and_swap(b, c);
}
