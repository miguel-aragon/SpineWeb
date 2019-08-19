/*



*/


#include <stdio.h>
#include <stdlib.h>
#include "get_distance.h"
#include "numeric.h"
#include "pixel3.h"

//===========================================================
//
//===========================================================
void get_distance(char *voids, char *boundaries, int type_distance, int sizeX, int sizeY, int sizeZ){
  int  i,j,k,w,q;
  int  *shell_ind = malloc(27*sizeof(int));  
  char shell_array[27];
  int  region_count;
  int  *where_new, n_where_new;
  int  cont_new=0;
  int  n_iter=sizeX;  //--- Set to a big number

  //--- Clean boundaries array.
  for(i=0; i<sizeX*sizeY*sizeZ; i++) boundaries[i] = 0;
  
  printf("Start iterations, maximum: %d\n", n_iter); fflush(stdout);

  //==========================
  //   Main iteration loop
  //==========================
  for (q=0;q<n_iter;q++){

    cont_new = 0;

    //--- Loop over grid cells...
    for(i=0; i<sizeX; i++){
      for(j=0; j<sizeY; j++){
	for(k=0; k<sizeZ; k++){
	  
	  //--- Get shell around [xgrid,ygrid,zgrid] cell
	  shell(shell_ind, i,j,k, sizeX,sizeY,sizeZ);

	  //--- Only not processed points
	  if (voids[shell_ind[0]] < 2) continue;
	  
	  
	  //--- Spine
	  if (type_distance == 0) {
	    if(voids[shell_ind[0]] <  2) continue;
	    //--- shell_array contains the values of void in the shell
	    for (w=0;w<27;w++) {
	      if (voids[shell_ind[w]] <  2) shell_array[w] = 0;
	      else                          shell_array[w] = 1;
	    }
	  }
	  
	  //--- Walls
	  if (type_distance == 1) {
	    if(voids[shell_ind[0]] != 2) continue;
	    //--- shell_array contains the values of void in the shell
	    for (w=0;w<27;w++) {
	      if (voids[shell_ind[w]] != 2) shell_array[w] = 0;
	      else                          shell_array[w] = 1;
	    }
	  }
	  
	  //--- Filaments
	  if (type_distance == 2) {
	    if(voids[shell_ind[0]] <= 2) continue;
	    //--- shell_array contains the values of void in the shell
	    for (w=0;w<27;w++) {
	      if (voids[shell_ind[w]] <= 2) shell_array[w] = 0;
	      else                          shell_array[w] = 1;
	    }
	  }	  


	  //--- Find indexes of non-processed pixels
	  n_where_new = 27;
	  where_new = where_char(0,"=",shell_array,&n_where_new);


	  //--- If there are new pixels
	  if (n_where_new != 0) 
	    for (w=0;w<n_where_new;w++)  boundaries[shell_ind[where_new[w]]] = q+1;


	  //--- Count new pixels in iteration
	  cont_new = cont_new+n_where_new;

	  //--- Free array
	  free(where_new);

	}
      }
      //printf("%d ", cont_new); fflush(stdout);
    }

    //--- Update voids array. Dilate...
    if (type_distance == 0) for(w=0; w<sizeX*sizeY*sizeZ; w++) if(boundaries[w] != 0) voids[w] = 2;
    if (type_distance == 1) for(w=0; w<sizeX*sizeY*sizeZ; w++) if(boundaries[w] != 0) voids[w] = 2;
    if (type_distance == 2) for(w=0; w<sizeX*sizeY*sizeZ; w++) if(boundaries[w] != 0) voids[w] = 3;

    printf("new[%d] = \t %d\n", q, cont_new); fflush(stdout);
    //--- Leave if there a no more new pixels
    if (cont_new == 0) break;
  }

}



//===========================================================
//
//===========================================================
void get_distance_sph(char *voids, char *boundaries, int type_distance, int sizeX, int sizeY, int sizeZ){
  int  i,j,k,w,q;
  int  n_shell = 19;
  int  *shell_ind = malloc(n_shell*sizeof(int));  
  char shell_array[n_shell];
  int  region_count;
  int  *where_new, n_where_new;
  int  cont_new=0;
  int  n_iter=sizeX;  //--- Set to a big number

  //--- Clean boundaries array.
  for(i=0; i<sizeX*sizeY*sizeZ; i++) boundaries[i] = 0;
  
  printf("Start iterations, maximum: %d\n", n_iter); fflush(stdout);

  //==========================
  //   Main iteration loop
  //==========================
  for (q=0;q<n_iter;q++){

    cont_new = 0;

    //--- Loop over grid cells...
    for(i=0; i<sizeX; i++){
      for(j=0; j<sizeY; j++){
	for(k=0; k<sizeZ; k++){
	  
	  //--- Get shell around [xgrid,ygrid,zgrid] cell
	  shell_sph(shell_ind, i,j,k, sizeX,sizeY,sizeZ);

	  //--- Only not processed points
	  if (voids[shell_ind[0]] < 2) continue;
	  
	  
	  //--- Spine
	  if (type_distance == 0) {
	    if(voids[shell_ind[0]] <  2) continue;
	    //--- shell_array contains the values of void in the shell
	    for (w=0;w<n_shell;w++) {
	      if (voids[shell_ind[w]] <  2) shell_array[w] = 0;
	      else                          shell_array[w] = 1;
	    }
	  }
	  
	  //--- Walls
	  if (type_distance == 1) {
	    if(voids[shell_ind[0]] != 2) continue;
	    //--- shell_array contains the values of void in the shell
	    for (w=0;w<n_shell;w++) {
	      if (voids[shell_ind[w]] != 2) shell_array[w] = 0;
	      else                          shell_array[w] = 1;
	    }
	  }
	  
	  //--- Filaments
	  if (type_distance == 2) {
	    if(voids[shell_ind[0]] <= 2) continue;
	    //--- shell_array contains the values of void in the shell
	    for (w=0;w<n_shell;w++) {
	      if (voids[shell_ind[w]] <= 2) shell_array[w] = 0;
	      else                          shell_array[w] = 1;
	    }
	  }	  


	  //--- Find indexes of non-processed pixels
	  n_where_new = n_shell;
	  where_new = where_char(0,"=",shell_array,&n_where_new);


	  //--- If there are new pixels
	  if (n_where_new != 0) 
	    for (w=0;w<n_where_new;w++)  boundaries[shell_ind[where_new[w]]] = q+1;


	  //--- Count new pixels in iteration
	  cont_new = cont_new+n_where_new;

	  //--- Free array
	  free(where_new);

	}
      }
      //printf("%d ", cont_new); fflush(stdout);
    }

    //--- Update voids array. Dilate...
    if (type_distance == 0) for(w=0; w<sizeX*sizeY*sizeZ; w++) if(boundaries[w] != 0) voids[w] = 2;
    if (type_distance == 1) for(w=0; w<sizeX*sizeY*sizeZ; w++) if(boundaries[w] != 0) voids[w] = 2;
    if (type_distance == 2) for(w=0; w<sizeX*sizeY*sizeZ; w++) if(boundaries[w] != 0) voids[w] = 3;

    printf("new[%d] = \t %d\n", q, cont_new); fflush(stdout);
    //--- Leave if there a no more new pixels
    if (cont_new == 0) break;
  }

}
