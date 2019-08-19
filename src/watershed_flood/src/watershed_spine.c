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
#include "../inc/watershed_spine.h"


//==========================================
//   NAME:
//       get_void_boundaries
//   DESCRIPTION:
//       Find the pixels that lie between two or more
//       voids. This is like a thick Spine but the topology 
//       is all wrong.
//==========================================
char *get_void_boundaries(int *voids, int sizeX, int sizeY, int sizeZ)
{
  int  i,j,k,w;
  int  n_shell=27;
  int  *shell_ind = malloc(n_shell*sizeof(int));
  int  shell_array[n_shell];
  int  region_count;
  char *boundaries=NULL;


  //--- Allocate memory for DataCube
  if ((boundaries = (char *) malloc(sizeof(char)*sizeX*sizeY*sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed for boundaries. Exit program... \n\n");
      exit (1);
    }

  //--- Loop over grid cells...
  for(i=0; i<sizeX; i++){
    for(j=0; j<sizeY; j++){
      for(k=0; k<sizeZ; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, sizeX,sizeY,sizeZ);
	
	//--- Copy neighbourhood to array
	for(w=0; w<27; w++) shell_array[w] = voids[shell_ind[w]];
	
	//--- Count number of different elements in neighbourhood
	boundaries[shell_ind[0]] = count_different_int(shell_array, n_shell);
      }
    }
  }

  free(shell_ind);

  return boundaries;

}


//==========================================
//   NAME:
//      local_watershed_flood
//   DESCRIPTION:
//      This is the tricky part of the code. Once the
//      watershed basins have been defined we must
//      find the skeleton. In get_void_boundaries() we identified
//      the boundary pixels. Here we perform a local
//      flooding watershed ONLY on the boundary pixels.
//   NOTES:
//      OJO: This function is destructive for *den!!!
//==========================================
void local_watershed_sort(double *den, int *voids, char *boundaries, int sizeX, int sizeY, int sizeZ)
{
  int     i,j;
  int     n_shell=27;
  int    *shell_ind   = malloc(n_shell*sizeof(int));
  double *slope_shell = malloc(n_shell*sizeof(double));
  int     shell_array[n_shell];
  int    *other_void;
  int     n_other_void=n_shell;
  double  max_den;              //--- maximum density of cube
  int     *where_boundaries;    //--- Indexes of boundary pixels
  int     *temp_array;          //--- Auxiliary array
  int     n_where_boundaries;   //--- Number of boundary pixels
  double *den_subarray;        //--- Densities in bounday cells
  int    *sorted_indexes;      //--- Array to store permutation indexes of sorting
  double  sizeXd=(double)sizeX,sizeYd=(double)sizeY,sizeZd=(double)sizeZ; //--- double versions
  int    *xyz = malloc(3*sizeof(int));  //--- To be used with decompose_3d
  int     xg,yg,zg;             //--- Cheap bug fix
  double *slope_shell_sub = (double *) malloc(sizeof(double)*n_shell);
  int     n_overflood;
  int    *overflood;
  //--- 
  int     n_pixels = sizeX*sizeY*sizeZ;

  //--- maximum height
  max_den = max_double(den, n_pixels);

  //--- Get indexes of boundaries
  n_where_boundaries = sizeX*sizeY*sizeZ; //--- This will contain the number of elements in where_boundaries
  where_boundaries = where_char(1, "<" , boundaries, &n_where_boundaries);

  //--- Fill array with densities at the boundaries
  den_subarray  = (double *) malloc(sizeof(double) *n_where_boundaries);
  for (i=0;i<n_where_boundaries;i++) den_subarray[i] = den[where_boundaries[i]];

  //--- Get permutation indexes of sorted density in boundaries
  printf(">>> Sorting %d pixels in boundaries between voids...\n",n_where_boundaries); fflush(stdout);
  sorted_indexes = sort_index_double(den_subarray, n_where_boundaries);

  //--- Allocate temporary array and fill with density-sorted indexes
  temp_array  = (int *) malloc(sizeof(int) *n_where_boundaries);
  for (i=0;i<n_where_boundaries;i++) temp_array[i] = where_boundaries[sorted_indexes[i]];

  //--- Fill where_boundaries with temporary array
  for (i=0;i<n_where_boundaries;i++) where_boundaries[i] = temp_array[i];

  //--- Loop over pixels in boundaries
  for (j=0;j<n_where_boundaries;j++) {

    //--- 1D to 3D index decomposition
    decomposed_index3d(xyz, where_boundaries[j], sizeXd,sizeYd,sizeZd);
    //--- Fix bug, check later...
    xg = xyz[0]; yg = xyz[1]; zg = xyz[2]; 
    //--- Get shell around pixel
    shell_27(shell_ind, xg,yg,zg, sizeX,sizeY,sizeZ);
    
    //--- ID's of voids inside shell
    for (i=0;i<27;i++) shell_array[i]=voids[shell_ind[i]];

    //--- Which other voids are inside shell?
    n_other_void=27;
    other_void = where_int(shell_array[0],"#",shell_array,&n_other_void);

    //--- Compute slopes from center pixel
    for (i=0;i<27;i++) slope_shell[i] = den[shell_ind[0]] - den[shell_ind[i]];

    //--- Only slopes that corespond to another void
    for (i=0;i<n_other_void;i++) slope_shell_sub[i] = slope_shell[other_void[i]];

    //--- If pixels from another void are lower then
    //    there is overflood and we are in a critical point
    n_overflood=0;
    for (i=0;i<n_other_void;i++) if(slope_shell_sub[i]>=0) ++n_overflood;

    //--- Build wall. IMPORTANT: it should be GT but there may be
    //    identical contiguous values due to float limitations <- Now changed to double...
    if(n_overflood > 0) den[shell_ind[0]] = max_den;

    //---free array used in where_usint()
    free(other_void);

  }

  //--- Add skeleton to voids
  max_den = max_double(den, n_pixels);
  for (i=0; i<n_pixels;i++)
    {
      if (den[i] == max_den) 
	{
	voids[i] = 0;
	}
      else
	{
	  voids[i] = voids[i]+1; //--- Add one to void counter
	}
    }

  free(shell_ind);
  free(slope_shell);
  free(xyz);
  free(slope_shell_sub);

}



//==========================================
//   NAME:
//      local_watershed_flood_float
//   DESCRIPTION:
//      [FLOAT] This is the tricky part of the code. Once the
//      watershed basins have been defined we must
//      find the skeleton. In get_void_boundaries() we identified
//      the boundary pixels. Here we perform a local
//      flooding watershed ONLY on the boundary pixels.
//   NOTES:
//      OJO: This function is destructive for *den!!!
//==========================================
void local_watershed_sort_float(float *den, int *voids, char *boundaries, int sizeX, int sizeY, int sizeZ)
{
  int     i,j;
  int     n_shell=27;
  int    *shell_ind   = malloc(n_shell*sizeof(int));
  float  *slope_shell = malloc(n_shell*sizeof(float));
  int     shell_array[n_shell];
  int    *other_void;
  int     n_other_void=n_shell;
  float   max_den;              //--- maximum density of cube
  int     *where_boundaries;    //--- Indexes of boundary pixels
  int     *temp_array;          //--- Auxiliary array
  int     n_where_boundaries;   //--- Number of boundary pixels
  float  *den_subarray;        //--- Densities in bounday cells
  int    *sorted_indexes;      //--- Array to store permutation indexes of sorting
  float  sizeXd=(double)sizeX,sizeYd=(double)sizeY,sizeZd=(double)sizeZ; //--- double versions
  int    *xyz = malloc(3*sizeof(int));  //--- To be used with decompose_3d
  int     xg,yg,zg;             //--- Cheap bug fix
  float  *slope_shell_sub = (float *) malloc(sizeof(float)*n_shell);
  int     n_overflood;
  int    *overflood;
  //--- 
  int     n_pixels = sizeX*sizeY*sizeZ;

  //--- maximum height
  max_den = max_float(den, n_pixels);

  //--- Get indexes of boundaries
  n_where_boundaries = sizeX*sizeY*sizeZ; //--- This will contain the number of elements in where_boundaries
  where_boundaries = where_char(1, "<" , boundaries, &n_where_boundaries);

  //--- Fill array with densities at the boundaries
  den_subarray  = (float *) malloc(sizeof(float) *n_where_boundaries);
  for (i=0;i<n_where_boundaries;i++) den_subarray[i] = den[where_boundaries[i]];

  //--- Get permutation indexes of sorted density in boundaries
  printf(">>> Sorting %d pixels in boundaries between voids...\n",n_where_boundaries); fflush(stdout);
  sorted_indexes = sort_index_float(den_subarray, n_where_boundaries);

  //--- Allocate temporary array and fill with density-sorted indexes
  temp_array  = (int *) malloc(sizeof(int) *n_where_boundaries);
  for (i=0;i<n_where_boundaries;i++) temp_array[i] = where_boundaries[sorted_indexes[i]];

  //--- Fill where_boundaries with temporary array
  for (i=0;i<n_where_boundaries;i++) where_boundaries[i] = temp_array[i];

  //--- Loop over pixels in boundaries
  for (j=0;j<n_where_boundaries;j++) {

    //--- 1D to 3D index decomposition
    decomposed_index3d(xyz, where_boundaries[j], sizeXd,sizeYd,sizeZd);
    //--- Fix bug, check later...
    xg = xyz[0]; yg = xyz[1]; zg = xyz[2]; 
    //--- Get shell around pixel
    shell_27(shell_ind, xg,yg,zg, sizeX,sizeY,sizeZ);
    
    //--- ID's of voids inside shell
    for (i=0;i<27;i++) shell_array[i]=voids[shell_ind[i]];

    //--- Which other voids are inside shell?
    n_other_void=27;
    other_void = where_int(shell_array[0],"#",shell_array,&n_other_void);

    //--- Compute slopes from center pixel
    for (i=0;i<27;i++) slope_shell[i] = den[shell_ind[0]] - den[shell_ind[i]];

    //--- Only slopes that corespond to another void
    for (i=0;i<n_other_void;i++) slope_shell_sub[i] = slope_shell[other_void[i]];

    //--- If pixels from another void are lower then
    //    there is overflood and we are in a critical point
    n_overflood=0;
    for (i=0;i<n_other_void;i++) if(slope_shell_sub[i]>=0) ++n_overflood;

    //--- Build wall. IMPORTANT: it should be GT but there may be
    //    identical contiguous values due to float limitations <- Now changed to double...
    if(n_overflood > 0) den[shell_ind[0]] = max_den;

    //---free array used in where_usint()
    free(other_void);

  }

  //--- Add skeleton to voids
  max_den = max_float(den, n_pixels);
  for (i=0; i<n_pixels;i++)
    {
      if (den[i] == max_den) 
	{
	  voids[i] = 0;
	}
      else
	{
	  voids[i] = voids[i]+1; //--- Add one to void counter
	}
    }

  free(shell_ind);
  free(slope_shell);
  free(xyz);
  free(slope_shell_sub);

}


//==========================================
//   NAME:
//       count_adjacent_voids
//   DESCRIPTION:
//       This is the main SpineWeb idea. ID the pixel is
//       part of the watershed transform then count the
//       number of adjacent voids. 
//   NOTES:
//       The watershed transform has ID = 0.
//==========================================
void count_adjacent_voids(int *voids, char *boundaries, int sizeX, int sizeY, int sizeZ)
{
  int  i,j,k,w;
  int  n_shell = 27;
  int  *shell_ind = malloc(n_shell*sizeof(int));  
  int  shell_array[n_shell];
  int  region_count;
  
  //--- Loop over grid cells...
  for(i=0; i<sizeX; i++){
    for(j=0; j<sizeY; j++){
      for(k=0; k<sizeZ; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, sizeX,sizeY,sizeZ);
	
	//--- If pixel of the skeleton...
	if (voids[shell_ind[0]] == 0) {
	  //--- Copy neighbourhood to array
	  for(w=0; w<27; w++) shell_array[w] = voids[shell_ind[w]];
	  
	  //--- Count number of different elements in neighbourhood
	  region_count = count_different_int(shell_array, n_shell);

	  //--- Fill array
	  boundaries[shell_ind[0]] = region_count;
	  //--- Account for pixels deep into the skeleton (clusters) 
	  if (region_count ==1) if (shell_array[0] == 0) boundaries[shell_ind[0]] = 9;

	}

      }
    }
  }

  free(shell_ind);

}
