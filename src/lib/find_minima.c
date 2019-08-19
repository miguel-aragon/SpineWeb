/*

   NAME: 
       FUNCTION FIND_MINIMA


   DESCRIPTION:
       Find all the local minima in a rectangular neighbourhood
       (27 pixels). It returns a list with the indexes of the minima
       and the total number of local minima. It warns the user if
       there are ambiguities in the density field.


   INPUT:
       density          Density field.
       x_grid           Number of elements in the x direction. Same for y,z

   OUTPUT:
       total_minima     Total number of local minima.
       local_minima     Array with the indexes of all local minima.
       closest_minima   Each pixel contains the index of its least dense neighbour.


   NOTES:
       The function outputs a warning if there are replicated adjacent 
       density values. The algorithm still will work fine.
   

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pixel3.h"
#include "pixel2.h"
#include "find_minima.h"


//=========================================================
//
//  [DOUBLE] Find the local minima and return the ID indexes.
//
//=========================================================
int *find_minima_int_27(double *density, int x_grid, int y_grid, int z_grid, int *total_minima)
{

  //--- Pointer to output (local minima) array
  int      *local_minima;
  //--- Number of pixels in neighbourhood
  int       n_pixels_shell = 27;
  //--- General variables
  int       i,j,k,w,q, cont;
  int       min_ID;        //--- Id of minimum cell
  int       n_denser=0;    //--- Counter of minimum neighbours
  //--- Shell
  int       Ind_Shell[n_pixels_shell];
  int      *shell_ind = Ind_Shell;
  int       total=0;
  //--- Allowed number of voids
  //const int  n_allowed_voids = 65535;
  const int  n_allowed_voids = 10000000;

  //--- Allocate memory for DataCube
  if ((local_minima = (int *) malloc(sizeof(int)*n_allowed_voids)) == NULL) 
    {
      printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
      exit (1);
    }


  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//----------------------------------
	//--- Find least dense cell in shell
	//----------------------------------
	n_denser  = 0;                             //--- Start less dense counter
	min_ID = shell_ind[0];                     //--- first guess
	for(w=0; w<n_pixels_shell; w++){
	  //--- Check if minimum
	  if (density[min_ID] > density[shell_ind[w]]) { min_ID = shell_ind[w]; }

	  //--- DEBUGGING
	  if ((w > 0) && (density[shell_ind[0]] == density[shell_ind[w]])) {
	    //printf("WARNING: replicated density value. den[%d %d %d] \t= den[shell[%d]]  \t(%f = %f)\n",i,j,k, w, density[shell_ind[0]],density[shell_ind[w]]);
	  }

	  //--- Count how many denser neighbours
	  if (density[shell_ind[0]] < density[shell_ind[w]]) ++n_denser;
	}
		
	//--- Identify local minima and store its ID
	if (n_denser == n_pixels_shell-1) {
	  local_minima[*total_minima] = shell_ind[0];
	  ++(*total_minima);	  //--- Increase value pointed by total_minima
	  
	  //--- If more voids that the allowed the stop
	  if ((*total_minima) >= n_allowed_voids) {
	    printf("Total number of voids larger than the allowed %d [hard coded] \n", n_allowed_voids);
	    printf("      Stoped at grid position [%d,%d,%d]\n", i,j,k);
	    printf("      Exiting program...");
	    exit(1);
	  }
	}

      }
    }
  }

  return local_minima;

}

//=========================================================
//
//  [FLOAT] Find the local minima and return the ID indexes.
//
//=========================================================
int *find_minima_int_27_float(float *density, int x_grid, int y_grid, int z_grid, int *total_minima)
{

  //--- Pointer to output (local minima) array
  int      *local_minima;
  //--- Number of pixels in neighbourhood
  int       n_pixels_shell = 27;
  //--- General variables
  int       i,j,k,w,q, cont;
  int       min_ID;        //--- Id of minimum cell
  int       n_denser=0;    //--- Counter of minimum neighbours
  //--- Shell
  int       Ind_Shell[n_pixels_shell];
  int      *shell_ind = Ind_Shell;
  int       total=0;
  //--- Allowed number of voids
  //const int  n_allowed_voids = 65535;
  const int  n_allowed_voids = 10000000;

  //--- Allocate memory for DataCube
  if ((local_minima = (int *) malloc(sizeof(int)*n_allowed_voids)) == NULL) 
    {
      printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
      exit (1);
    }


  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_27(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//----------------------------------
	//--- Find least dense cell in shell
	//----------------------------------
	n_denser  = 0;                             //--- Start less dense counter
	min_ID = shell_ind[0];                     //--- first guess
	for(w=0; w<n_pixels_shell; w++){
	  //--- Check if minimum
	  if (density[min_ID] > density[shell_ind[w]]) { min_ID = shell_ind[w]; }

	  //--- DEBUGGING
	  //if ((w > 0) && (density[shell_ind[0]] == density[shell_ind[w]])) {
	    //printf("WARNING: replicated density value. den[%d %d %d] \t= den[shell[%d]]  \t(%f = %f)\n",i,j,k, w, density[shell_ind[0]],density[shell_ind[w]]);
	  //}

	  //--- Count how many denser neighbours
	  if (density[shell_ind[0]] < density[shell_ind[w]]) ++n_denser;
	}
	
	//--- Identify local minima and store its ID
	if (n_denser == n_pixels_shell-1) {
	  local_minima[*total_minima] = shell_ind[0];
	  ++(*total_minima);  //--- Increase value pointed by total_minima
	    
	  //--- If more voids that the allowed the stop
	  if ((*total_minima) >= n_allowed_voids) {
	    printf("Total number of voids larger than the allowed %d [hard coded] \n", n_allowed_voids);
	    printf("      Stoped at grid position [%d,%d,%d]\n", i,j,k);
	    printf("      Exiting program...");
	    exit(1);
	  }
	}

      }
    }
  }

  return local_minima;

}




//=========================================================
//
//  [DOUBLE] Find the local minima and return the ID indexes.
//
//=========================================================
int *find_minima_int_07(double *density, int x_grid, int y_grid, int z_grid, int *total_minima)
{

  //--- Pointer to output (local minima) array
  int      *local_minima;
  //--- Number of pixels in neighbourhood
  int       n_pixels_shell = 7;
  //--- General variables
  int       i,j,k,w,q, cont;
  //--- Id of minimum cell
  int       min_ID;        
  //--- Counter of minimum neighbours
  int       n_denser=0;    
  //--- Shell
  int       Ind_Shell[n_pixels_shell];
  int      *shell_ind = Ind_Shell;
  int       total=0;
  //--- Allowed number of voids
  //const int  n_allowed_voids = 65535;
  const int  n_allowed_voids = 10000000;

  //--- Allocate memory for DataCube
  if ((local_minima = (int *) malloc(sizeof(int)*n_allowed_voids)) == NULL) 
    {
      printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
      exit (1);
    }


  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_07(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//----------------------------------
	//--- Find least dense cell in shell
	//----------------------------------
	n_denser  = 0;                             //--- Start less dense counter
	min_ID = shell_ind[0];                     //--- first guess
	for(w=0; w<n_pixels_shell; w++){
	  //--- Check if minimum
	  if (density[min_ID] > density[shell_ind[w]]) { min_ID = shell_ind[w]; }

	  //--- DEBUGGING
	  if ((w > 0) && (density[shell_ind[0]] == density[shell_ind[w]])) {
	    //printf("WARNING: replicated density value. den[%d %d %d] \t= den[shell[%d]]  \t(%f = %f)\n",i,j,k, w, density[shell_ind[0]],density[shell_ind[w]]);
	  }

	  //--- Count how many denser neighbours
	  if (density[shell_ind[0]] < density[shell_ind[w]]) ++n_denser;
	}
		
	//--- Identify local minima and store its ID
	if (n_denser == n_pixels_shell-1) {
	  local_minima[*total_minima] = shell_ind[0];
	  ++(*total_minima);	  //--- Increase value pointed by total_minima
	  
	  //--- If more voids that the allowed the stop
	  if ((*total_minima) >= n_allowed_voids) {
	    printf("Total number of voids larger than the allowed %d [hard coded] \n", n_allowed_voids);
	    printf("      Stoped at grid position [%d,%d,%d]\n", i,j,k);
	    printf("      Exiting program...");
	    exit(1);
	  }
	}

      }
    }
  }

  return local_minima;

}


//=========================================================
//
//  Find the local minima and return the ID indexes.
//
//=========================================================
int *find_minima_int_07_float(float *density, int x_grid, int y_grid, int z_grid, int *total_minima)
{

  //--- Pointer to output (local minima) array
  int      *local_minima;
  //--- Number of pixels in neighbourhood
  int       n_pixels_shell = 7;
  //--- General variables
  int       i,j,k,w,q, cont;
  //--- Id of minimum cell
  int       min_ID;        
  //--- Counter of minimum neighbours
  int       n_denser=0;    
  //--- Shell
  int       Ind_Shell[n_pixels_shell];
  int      *shell_ind = Ind_Shell;
  int       total=0;
  //--- Allowed number of voids
  //const int  n_allowed_voids = 65535;
  const int  n_allowed_voids = 10000000;

  //--- Allocate memory for DataCube
  if ((local_minima = (int *) malloc(sizeof(int)*n_allowed_voids)) == NULL) 
    {
      printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
      exit (1);
    }


  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell_07(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//----------------------------------
	//--- Find least dense cell in shell
	//----------------------------------
	n_denser  = 0;                             //--- Start less dense counter
	min_ID = shell_ind[0];                     //--- first guess
	for(w=0; w<n_pixels_shell; w++){
	  //--- Check if minimum
	  if (density[min_ID] > density[shell_ind[w]]) { min_ID = shell_ind[w]; }

	  //--- DEBUGGING
	  //if ((w > 0) && (density[shell_ind[0]] == density[shell_ind[w]])) {
	    //printf("WARNING: replicated density value. den[%d %d %d] \t= den[shell[%d]]  \t(%f = %f)\n",i,j,k, w, density[shell_ind[0]],density[shell_ind[w]]);
	  //}

	  //--- Count how many denser neighbours
	  if (density[shell_ind[0]] < density[shell_ind[w]]) ++n_denser;
	}
	
	//--- Identify local minima and store its ID
	if (n_denser == n_pixels_shell-1) {
	  local_minima[*total_minima] = shell_ind[0];
	  ++(*total_minima);  //--- Increase value pointed by total_minima
	    
	  //--- If more voids that the allowed the stop
	  if ((*total_minima) >= n_allowed_voids) {
	    printf("Total number of voids larger than the allowed %d [hard coded] \n", n_allowed_voids);
	    printf("      Stoped at grid position [%d,%d,%d]\n", i,j,k);
	    printf("      Exiting program...");
	    exit(1);
	  }
	}

      }
    }
  }

  return local_minima;

}


//=========================================================
//
//  Find the local minima and return the ID indexes. This is a 2D function
//
//=========================================================
int *find_minima_int_09_float(float *density, int x_grid, int y_grid, int *total_minima)
{

  //--- Pointer to output (local minima) array
  int      *local_minima;
  //--- Number of pixels in neighbourhood
  int       n_pixels_shell = 9;
  //--- General variables
  int       i,j,w,q, cont;
  //--- Id of minimum cell
  int       min_ID;        
  //--- Counter of minimum neighbours
  int       n_denser=0;    
  //--- Shell
  int       Ind_Shell[n_pixels_shell];
  int      *shell_ind = Ind_Shell;
  int       total=0;
  //--- Allowed number of voids
  const int  n_allowed_voids = 10000000;

  //--- Allocate memory for DataCube
  if ((local_minima = (int *) malloc(sizeof(int)*n_allowed_voids)) == NULL) {
      printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
      exit (1);
    }


  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
 
      //--- Get shell around [xgrid,ygrid,zgrid] cell
      shell_09(shell_ind, i,j, x_grid,y_grid);

      //----------------------------------
      //--- Find least dense cell in shell
      //----------------------------------
      n_denser  = 0;                             //--- Start less dense counter
      min_ID = shell_ind[0];                     //--- first guess
      for(w=0; w<n_pixels_shell; w++){
	//--- Check if minimum
	if (density[min_ID] > density[shell_ind[w]]) { min_ID = shell_ind[w]; }
	//--- Count how many denser neighbours
	if (density[shell_ind[0]] < density[shell_ind[w]]) ++n_denser;
      }
      
      //--- Identify local minima and store its ID
      if (n_denser == n_pixels_shell-1) {
	local_minima[*total_minima] = shell_ind[0];
	++(*total_minima);  //--- Increase value pointed by total_minima
	
	//--- If more voids that the allowed the stop
	if ((*total_minima) >= n_allowed_voids) {
	  printf("Total number of voids larger than the allowed %d [hard coded] \n", n_allowed_voids);
	  printf("      Stopped at grid position [%d,%d]\n", i,j);
	  printf("      Exiting program...");
	  exit(1);
	}
      }
      
      
    }
  }
  
  return local_minima;
  
}



