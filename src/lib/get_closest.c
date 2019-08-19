/*



*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "get_closest.h"
#include "pixel3.h"
#include "pixel2.h"

//==================================
//--- GET_CLOSEST
//    Find the neighbouring pixel with lower density. The index of
//    this pixel is stored in closest_minima. The current implementation
//    stores the absolute index of the pixel in an int array. For lower
//    memory one could use the relative index in the 27 neighbourhood and
//    store in a char array.
//==================================
void get_closest(double *density, int *closest_minima, int x_grid, int y_grid, int z_grid)
{
  int   i,j,k,w,q, cont;
  int  min_ID;        //--- Id of minimum cell
  //--- Shell
  int  Ind_Shell[27];
  int  *shell_ind = Ind_Shell;
  //--- Decompose3D
  int  xyz_temp[3];
  int  *xyz = xyz_temp;

  printf("\t Finding local minima and most underdense neighbour...\n\n");
  fflush(stdout);

  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//----------------------------------
	//--- Find least dense cell in shell
	//----------------------------------
	min_ID = shell_ind[0];                     //--- first guess
	for(w=0; w<27; w++){
	  //--- Check if minimum
	  if (density[min_ID] > density[shell_ind[w]]) 
	    { 
	      min_ID = shell_ind[w]; 
	    }
	  //--- DEBUGGING. Replicated values may introduce errors.
	  if ((w > 0) && (density[shell_ind[0]] == density[shell_ind[w]])) 
	    {
	      printf(">>> WARNING: replicated density value. ");
	      printf("    den[%d %d %d] \t= den[shell[%d]]  \t(%f = %f)\n",i,j,k, w, density[shell_ind[0]],density[shell_ind[w]]);
	    }
	}
	
	//--- Save least dense neighbour
	closest_minima[shell_ind[0]] = min_ID;

      } //--- end for k
    } //--- end for j
  } //--- end for i


}

//==================================
//--- GET_CLOSEST
//    [FLOAT]
//    Find the neighbouring pixel with lower density. The index of
//    this pixel is stored in closest_minima. The current implementation
//    stores the absolute index of the pixel in an int array. For lower
//    memory one could use the relative index in the 27 neighbourhood and
//    store in a char array.
//==================================
void get_closest_float(float *density, int *closest_minima, int x_grid, int y_grid, int z_grid)
{
  int   i,j,k,w,q, cont;
  int  min_ID;        //--- Id of minimum cell
  //--- Shell
  int  Ind_Shell[27];
  int  *shell_ind = Ind_Shell;
  //--- Decompose3D
  int  xyz_temp[3];
  int  *xyz = xyz_temp;

  printf("\t Finding local minima and most underdense neighbour...\n\n");
  fflush(stdout);

  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//----------------------------------
	//--- Find least dense cell in shell
	//----------------------------------
	min_ID = shell_ind[0];                     //--- first guess
	for(w=0; w<27; w++){
	  //--- Check if minimum
	  if (density[min_ID] > density[shell_ind[w]]) 
	    { 
	      min_ID = shell_ind[w]; 
	    }
	  //--- DEBUGGING. Replicated values may introduce errors.
	  //if ((w > 0) && (density[shell_ind[0]] == density[shell_ind[w]])) {
	  //printf(">>> WARNING: replicated density value. ");
	  //printf("    den[%d %d %d] \t= den[shell[%d]]  \t(%f = %f)\n",i,j,k, w, density[shell_ind[0]],density[shell_ind[w]]);
	  //}
	}
	
	//--- Save least dense neighbour
	closest_minima[shell_ind[0]] = min_ID;

      } //--- end for k
    } //--- end for j
  } //--- end for i


}


//==================================
//--- GET_CLOSEST_2D
//    [FLOAT]
//    Find the neighbouring pixel with lower density. The index of
//    this pixel is stored in closest_minima. The current implementation
//    stores the absolute index of the pixel in an int array. For lower
//    memory one could use the relative index in the 27 neighbourhood and
//    store in a char array.
//==================================
void get_closest_2d_float(float *density, int *closest_minima, int x_grid, int y_grid)
{
  int   i,j,k,w,q, cont;
  int  min_ID;        //--- Id of minimum cell
  //--- Shell
  int  Ind_Shell[9];
  int  *shell_ind = Ind_Shell;
  //--- Decompose2D
  int  xy_temp[2];
  int  *xy = xy_temp;

  printf("\t Finding local minima and most underdense neighbour...\n\n");
  fflush(stdout);

  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      
      //--- Get shell around [xgrid,ygrid,zgrid] cell
      shell_09(shell_ind, i,j, x_grid,y_grid);
      
      //----------------------------------
      //--- Find least dense cell in shell
      //----------------------------------
      min_ID = shell_ind[0];                     //--- first guess
      for(w=0; w<9; w++){
	//--- Check if minimum
	if (density[min_ID] > density[shell_ind[w]]) min_ID = shell_ind[w]; 
      }
      
      //--- Save least dense neighbour
      closest_minima[shell_ind[0]] = min_ID;
      
    } //--- end for j
  } //--- end for i

}


