/*



*/


#include <stdio.h>
#include <stdlib.h>
#include "get_distance.h"
#include "numeric.h"
#include "pixel3.h"

void get_euclidean_distance(char *voids, float *distances, int sizeX, int sizeY, int sizeZ){
  int   i,j,k;
  //--- For index decomposing
  int    *xyz_pixel=(int *) malloc(sizeof(int) * 3);
  int    *xyz_spine=(int *) malloc(sizeof(int) * 3);
  double sizeX2=(double) sizeX;
  double sizeY2=(double) sizeY;
  double sizeZ2=(double) sizeZ;
  int    n_pixels = sizeX*sizeY*sizeZ;
  int    *where_spine, n_where_spine;
  int    dist_temp, dist_min;

  printf("Prepare voids and distances arrays \n"); fflush(stdout);

  //--- Skeleton set to 1 and the rest set to 0
  for(i=0; i<sizeX*sizeY*sizeZ; i++) if(voids[i] >= 2) voids[i] = 1;

  //--- Clean distances array.
  for(i=0; i<sizeX*sizeY*sizeZ; i++) distances[i] = 0;

  //--- Find indexes of skeleton
  n_where_spine = sizeX*sizeY*sizeZ; //--- Number of cells, this value gets overwritten
  where_spine = where_char(1,"=",voids,&n_where_spine);

  printf(" %d \n", n_where_spine); fflush(stdout);
 
  //==========================
  //   Main iteration loop. Compute the shortest distance from point to spine
  //==========================
  printf("Enter main loop \n"); fflush(stdout);
  for(i=0; i<n_pixels; i++){

    //--- Set minimum distance to a large unlikely value
    dist_min=sizeX*sizeX;

    
    //--- Decompose index of current pixel
    decomposed_index3d(xyz_pixel, i, sizeX2,sizeY2,sizeZ2);
   
    //--- Compute distances from point to spine and keep minimum
    for(j=0;j<n_where_spine;j++) {

      //--- Decompose indexes of current point and spine
      decomposed_index3d(xyz_spine, where_spine[j], sizeX2,sizeY2,sizeZ2);

      //--- Get distance from pixel to element of spine
      dist_temp = pow((xyz_pixel[0]-xyz_spine[0]),2)+pow((xyz_pixel[1]-xyz_spine[0]),1)+pow((xyz_pixel[2]-xyz_spine[2]),2);

      //--- Keep the smalles value
      if (dist_temp < dist_min) dist_min=dist_temp;

      //printf("[%d,%d] %d %d \n",i,j,dist_min, dist_temp); fflush(stdout);

    }//--- end for j

    //--- Now fill distance array with the minimum distance
    distances[i] = pow((double)dist_min,0.5);
    
    //printf(" %d %d %f \n",i, voids[i], distances[i]);

  }

  printf("  ready main loop \n"); fflush(stdout);

  free(where_spine);
  free(xyz_pixel);
  free(xyz_spine);

}



//================================
void get_euclidean_distance_type(char *voids, float *distances, int spine_type, int sizeX, int sizeY, int sizeZ){
  int   i,j,k;
  //--- For index decomposing
  int    *xyz_pixel=(int *) malloc(sizeof(int) * 3);
  int    *xyz_spine=(int *) malloc(sizeof(int) * 3);
  double sizeX2=(double) sizeX;
  double sizeY2=(double) sizeY;
  double sizeZ2=(double) sizeZ;
  int    n_pixels = sizeX*sizeY*sizeZ;
  int    *where_spine, n_where_spine;
  int    dist_temp, dist_min;

  printf("Prepare voids and distances arrays \n"); fflush(stdout);

  //--- Spine set to 1 depending of type and the rest set to 0
  if (spine_type == 0) {
    for(i=0; i<sizeX*sizeY*sizeZ; i++) {      
      if (voids[i] == 2) voids[i] = 1;      
      else               voids[i] = 0; 
    }
  } else if (spine_type == 1){
    for(i=0; i<sizeX*sizeY*sizeZ; i++) {
      if (voids[i] == 3) voids[i] = 1;
      else               voids[i] = 0;
    }
  } else if (spine_type == 2){
    for(i=0; i<sizeX*sizeY*sizeZ; i++) {
      if (voids[i] >= 2) voids[i] = 1;
      else               voids[i] = 0;
    }
  } else if (spine_type == 3){
    for(i=0; i<sizeX*sizeY*sizeZ; i++) {
      if (voids[i] >= 3) voids[i] = 1;
      else               voids[i] = 0;
    }
  }
  
  //--- Clean distances array.
  for(i=0; i<sizeX*sizeY*sizeZ; i++) distances[i] = 0;

  //--- Find indexes of skeleton
  n_where_spine = sizeX*sizeY*sizeZ; //--- Number of cells, this value gets overwritten
  where_spine = where_char(1,"=",voids,&n_where_spine);

  printf(" %d \n", n_where_spine); fflush(stdout);
 
  //==========================
  //   Main iteration loop. Compute the shortest distance from point to spine
  //==========================
  printf("Enter main loop \n"); fflush(stdout);
  for(i=0; i<n_pixels; i++){

    //--- Set minimum distance to a large unlikely value
    dist_min=sizeX*sizeX;

    
    //--- Decompose index of current pixel
    decomposed_index3d(xyz_pixel, i, sizeX2,sizeY2,sizeZ2);
   
    //--- Compute distances from point to spine and keep minimum
    for(j=0;j<n_where_spine;j++) {

      //--- Decompose indexes of current point and spine
      decomposed_index3d(xyz_spine, where_spine[j], sizeX2,sizeY2,sizeZ2);

      //--- Get distance from pixel to element of spine
      dist_temp = pow((xyz_pixel[0]-xyz_spine[0]),2)+pow((xyz_pixel[1]-xyz_spine[0]),1)+pow((xyz_pixel[2]-xyz_spine[2]),2);

      //--- Keep the smalles value
      if (dist_temp < dist_min) dist_min=dist_temp;

      //printf("[%d,%d] %d %d \n",i,j,dist_min, dist_temp); fflush(stdout);

    }//--- end for j

    //--- Now fill distance array with the minimum distance
    distances[i] = pow((double)dist_min,0.5);
    
    //printf(" %d %d %f \n",i, voids[i], distances[i]);

  }

  printf("  ready main loop \n"); fflush(stdout);

  free(where_spine);
  free(xyz_pixel);
  free(xyz_spine);

}

