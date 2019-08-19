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

#include "../../lib/pixel2.h"
#include "../../lib/numeric.h"
#include "../../lib/find_minima.h"
#include "../../lib/get_closest.h"
#include "../inc/water_functions_uphill.h"


//==========================================
//   NAME:
//       watershed_uphill_27_float
//   DESCRIPTION:
//       [FLOAT] This is just a nice wraper for the watershed
//       functions.
//   NOTES:
//==========================================
int *watershed_uphill_09_float(float *density, int x_grid, int y_grid)
{
  //--- Local minima
  int *local_minima=NULL;
  int  n_local_minima=0;
  //--- Closest minima
  int *closest_minima=NULL;
  //--- Voids
  int *voids=NULL;
  int  n_pixels = x_grid*y_grid;

  //==================================
  //--- Find local minima
  //==================================
  printf(">>> Finding local minima"); fflush(stdout);
  local_minima = find_minima_int_09_float(density, x_grid,y_grid, &n_local_minima);
  printf(">>> Ready finding minima [%d]\n", n_local_minima); fflush(stdout);
  
  //--- Allocate memory for voids
  if ((voids = (int *) malloc(sizeof(int)*n_pixels )) == NULL)
    {
      printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n"); fflush(stdout);
      exit (1);
    }

  //--- Allocate memory for closest minima
  if ((closest_minima = (int *) malloc(sizeof(int)*n_pixels )) == NULL)
    {
      printf(">>> ERROR: malloc failed allocating memory for closest_minima. \n    Exit program... \n\n"); fflush(stdout);
      exit (1);
    }

  //--- Get closest (lower) pixel in neighbourhood
  get_closest_2d_float(density, closest_minima, x_grid, y_grid);

  //--- Flood basins bu going uphill from the local minima.
  flood_basin_uphill_2d(closest_minima, voids, x_grid, y_grid, n_local_minima, local_minima);

  free(closest_minima);

  return voids;
}




//==========================================
//   NAME:
//       flood_basin_uphill
//   DESCRIPTION:
//       Flood basins starting from each local minima and going uphill.
//   NOTES:
//==========================================
void flood_basin_uphill_2d(int *closest_minima, int *voids, int x_grid, int y_grid, int total_minima, int *local_minima)
{
  int    i,j,k,w,q;
  //--- Shell
  int    Ind_Shell[9];
  int   *shell_ind = Ind_Shell;
  //--- Decompose3D
  int    xy_temp[2];
  int   *xy = xy_temp;
  int    xg,yg;      //--- Cheap bug fix
  //--- Where array
  int    where_temp[10];
  int   *where_array = where_temp;
  int    Void_Id, Void_Index;
  int    cont,temp; 
  //--- Double versions 
  double xd_grid,yd_grid,zd_grid;
  int    n_cell;   //--- Total cell elements

  //--- Double versions
  xd_grid = (float) x_grid;
  yd_grid = (float) y_grid;
  

  //--- Inicialize "voids" array
  n_cell = x_grid*y_grid;
  for (i=0; i<n_cell; i++) voids[i] = 0;

  printf("Start loop over %d minima...\n",total_minima);
  //--- Loop over all local minima
  for(i=0; i<total_minima; i++){
 
    //--- All cells in void share this Id, voids Id go 1 -> total_minima
    Void_Id = i+1;

    //--- Index of void center
    Void_Index  = local_minima[i];

    //--- Get the 3d indexes of local minima
    decomposed_index2d(xy, Void_Index, xd_grid,yd_grid);
    //--- OJO: Fix bug, check later...
    xg = xy[0]; yg = xy[1];

    //--- Get shell around void center
    shell_09(shell_ind, xg,yg, x_grid,y_grid);

    if (Void_Index != shell_ind[0]) {
      printf(">>> ERROR!!! %d == %d ?\n", Void_Index, shell_ind[0]);
      printf(">>> %d [%d %d] [%f %f]\n", i, xg,yg, xd_grid,yd_grid);
      fflush(stdout);
      exit(0);
    }

    //--- This is a simple implementation of the where() function.
    //    The array where_array contains the valid elements.
    //    The first element of where_array is the number of valid
    //    elements.
    cont = 0;
    for (w=0; w<9;w++) {
      //--- Find all neighbours that belong to the void. This is: all
      //    the cells that point to the center cell via closest_minima
      if (closest_minima[shell_ind[w]] == Void_Index) {
	where_array[cont+1] = w;
	cont++;
      }
      where_array[0] = cont; //--- Number of valid cells
    }

    //--- Fill valid cells with their corresponding Void ID
    for (w=0;w<where_array[0];w++) voids[shell_ind[where_array[w+1]]] = Void_Id;

    //--- Continue if the only valid cell is the void center itself
    if (where_array[0] == 1) continue;
    
    //--- Re-use where_array to store the valid shell cells, Ignore the
    //    first data element of where_array since it is the void center itself.
    for (w=0;w<where_array[0]-1;w++) where_array[w+1] = shell_ind[where_array[w+2]];
    //--- Update number of elements since we ignored the first cell in where_array.
    where_array[0] = where_array[0]-1;

    //-----------------------------------------
    //--- Call recursive function
    //-----------------------------------------
    recursive_flood_uphill_2d(where_array, closest_minima, voids, Void_Id, x_grid, y_grid);

    if(fmod((float)i,1000.0) == 0) {printf("%d voids of %d\n", i+1, total_minima); fflush(stdout);}

  }

  printf("================================================\n");
  fflush(stdout);

}



//==========================================
//   NAME:
//       recursive_flood_uphill
//   DESCRIPTION:
//       recursive flooding function. Starts from
//       local minima.
//   NOTES:
//==========================================
void recursive_flood_uphill_2d(int *children, int *closest_minima, int *voids, int ID, int x_grid, int y_grid){

  int    i,j,w,q, cont;
  int    index_i;
  //--- Shell
  int    Ind_Shell[9];
  int   *shell_ind = Ind_Shell;
  //--- Decompose2D
  int    xy_temp[2];
  int   *xy = xy_temp;
  int    xg,yg;      //--- Cheap bug fix
  //--- Where array
  int    where_temp[10];
  int   *where_array = where_temp;
  //--- Double versions 
  double xd_grid,yd_grid;


  //--- Double versions
  xd_grid = (float) x_grid;
  yd_grid = (float) y_grid;


  //--- If the pixel was already flooded skip. This step can be easily avoided.
  //    it is supposed to make the algorithm faster. Remove it if you prefer.
  if (voids[children[1]] == 0) return;

  //--- Loop over valid children cells 
  for (i=0;i<children[0];i++){

    index_i = children[i+1];

    //--- Get the 3d indexes of local minima
    decomposed_index2d(xy, index_i, xd_grid,yd_grid);
    //--- OJO: Fix bug, check later...
    xg = xy[0]; yg = xy[1];

    //--- Get shell around void center
    shell_09(shell_ind, xg,yg, x_grid,y_grid);
   
    //--- This is a simple implementation of the where() function.
    //    The array children contains the valid elements.
    //    The first element of children is the number of valid
    //    elements.
    cont = 0;
    for (w=0; w<9;w++) {
      //--- Find all neighbours that belong to the void. This is: all
      //    the cells that point to the center cell via closest_minima
      if (closest_minima[shell_ind[w]] == index_i) {
	where_array[cont+1] = w;
	cont++;
      }
      where_array[0] = cont; //--- Number of valid cells
    }

    //----------------------------------------
    //--- Continue loop if no more children.
    //    Here is where we stop recursion.
    //----------------------------------------
    if (cont == 0) continue;

    //--- Fill valid cells with their corresponding Void ID
    for (w=0;w<where_array[0];w++) voids[shell_ind[where_array[w+1]]] = ID;

    //--- Re-use where_array to store the valid shell cells
    for (w=0;w<where_array[0];w++) where_array[w+1] = shell_ind[where_array[w+1]];
    
    //-----------------------------------------
    //--- Call recursively
    //-----------------------------------------
    recursive_flood_uphill_2d(where_array, closest_minima, voids, ID, x_grid, y_grid);

  }

}

