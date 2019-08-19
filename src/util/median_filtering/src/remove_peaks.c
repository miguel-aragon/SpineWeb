/*

   NAME: 
       REMOVE PEAKS


   DESCRIPTION:
       Remove pixels above some threshold times the mean/median
       of its neighbous.

   INPUT:
       density          Density field.
       n_mean           Threshols given in terms of the mean of the
                        neighbours.

   OUTPUT:
       density          The cleaned density field


   NOTES:
       There is a "bug" in the parameter secion of the function. If I change
       the variable "threshold" to "float" then the value can not get passed.
       With "int" seems to be ok...

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../inc/remove_peaks.h"

//--- Comparison function
int compare_double(const void *v1, const void *v2);
int compare_float(const void *v1, const void *v2);


void remove_peaks(double *density, int x_grid, int y_grid, int z_grid, int threshold)
{
  int    i,j,k,w,q;
  int    n_peaks=0;
  //--- Shell
  int    Ind_Shell[27];
  double Den_Shell[27];
  int    *shell_ind = Ind_Shell;

  printf("Removing peaks above %d times the local median...\n\n", threshold);
  fflush(stdout);

  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//--- Fill array with shell
	for(w=0; w<27; w++)  Den_Shell[w] = density[shell_ind[w]];

	//--- Sort elements in shell
	qsort(Den_Shell, 27, sizeof(Den_Shell[0]), compare_double);

	//--- If above threshold then replace with median
	if (density[shell_ind[0]] > threshold*Den_Shell[13]) {
	  //density[shell_ind[0]] = Den_Shell[13];
	  density[shell_ind[0]] = (Den_Shell[ 5] + Den_Shell[ 6] + Den_Shell[ 7] + Den_Shell[ 8] + 
	                           Den_Shell[ 9] + Den_Shell[10] + Den_Shell[11] + Den_Shell[12] + 
	                           Den_Shell[13] + 
	                           Den_Shell[14] + Den_Shell[15] + Den_Shell[16] + Den_Shell[17] + 
	                           Den_Shell[18] + Den_Shell[19] + Den_Shell[20] + Den_Shell[21] )/17.0*2.0;
	  ++n_peaks;
	}

      }
    }
  }

  printf("%d peaks %d times above the local median removed",n_peaks, threshold);


}


//-------------------------------------------------------------------------------
void remove_peaks_float(float *density, int x_grid, int y_grid, int z_grid, int threshold)
{
  int    i,j,k,w,q;
  int    n_peaks=0;
  //--- Shell
  int    Ind_Shell[27];
  float  Den_Shell[27];
  int    *shell_ind = Ind_Shell;

  printf("Removing peaks above %d times the local median...\n\n", threshold);
  fflush(stdout);

  //--- Loop over grid cells...
  for(i=0; i<x_grid; i++){
    for(j=0; j<y_grid; j++){
      for(k=0; k<z_grid; k++){

	//--- Get shell around [xgrid,ygrid,zgrid] cell
	shell(shell_ind, i,j,k, x_grid,y_grid,z_grid);

	//--- Fill array with shell
	for(w=0; w<27; w++)  Den_Shell[w] = density[shell_ind[w]];

	//--- Sort elements in shell
	qsort(Den_Shell, 27, sizeof(Den_Shell[0]), compare_float);

	//--- If above threshold then replace with median
	if (density[shell_ind[0]] > threshold*Den_Shell[13]) {
	  //--- Simple median
	  //density[shell_ind[0]] = Den_Shell[13];
	  //--- Centered mean
	  density[shell_ind[0]] = (Den_Shell[ 5] + Den_Shell[ 6] + Den_Shell[ 7] + Den_Shell[ 8] +
				   Den_Shell[ 9] + Den_Shell[10] + Den_Shell[11] + Den_Shell[12] +
				   Den_Shell[13] +
				   Den_Shell[14] + Den_Shell[15] + Den_Shell[16] + Den_Shell[17] +
				   Den_Shell[18] + Den_Shell[19] + Den_Shell[20] + Den_Shell[21] )/17.0*2.0;
	  ++n_peaks;
	}

      }
    }
  }

  printf("%d peaks %d times above the local median removed",n_peaks, threshold);


}


int compare_double(const void *v1, const void *v2)
{
  if ((*(double *)v1 - *(double *)v2) < 0) {
    return (int) -1;
  }
  else{
    return (int) 1;
  }
}



int compare_float(const void *v1, const void *v2)
{
  if ((*(float *)v1 - *(float *)v2) < 0) {
    return (int) -1;
  }
  else{
    return (int) 1;
  }
}


