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

#include "../inc/water_functions.h"
#include "../../lib/pixel3.h"
#include "../../lib/numeric.h"
#include "../../lib/find_minima.h"

//=========================================================
//
//=========================================================
int *watershed_sorted_int_27(double *density, int x_grid, int y_grid, int z_grid)
{

    //--- General variables
    int    i,j,k,w;
    //--- Shell
    int    n_pixels_shell = 27;
    int    Ind_Shell[n_pixels_shell];
    int   *shell_ind = Ind_Shell;
    //--- Decompose3D
    int    xyz_temp[3];
    int   *xyz = xyz_temp;
    //--- Double versions
    double xd_grid,yd_grid,zd_grid;
    int    n_pixels = x_grid*y_grid*z_grid;
    //--- Pointer to voids array
    int   *voids;
    //--- Permutation indexes after sorting in density
    int   *sorted_indexes;
    //--- Adjacent pixels
    int    Adj_Pixels[n_pixels_shell];
    //--- Number of unique pixels ID in shell
    int    n_unique = 0;
    //--- Array to store result from where_usint()
    int   *valid_unique;
    int    n_valid_unique;

    //--- Double versions of grid size
    xd_grid = (double) x_grid;
    yd_grid = (double) y_grid;
    zd_grid = (double) z_grid;

    //==================================
    //--- Find local minima
    //==================================
    //--- Local minima
    int *local_minima;
    int  n_local_minima=0;
    local_minima = find_minima_int_27(density, x_grid,y_grid,z_grid, &n_local_minima);
    printf("Ready finding minima [%d]\n", n_local_minima);
    fflush(stdout);

    //--- Allocate memory for voids
    if ((voids = (int *) malloc(sizeof(int)*n_pixels )) == NULL)
    {
        printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
        fflush(stdout);
        exit (1);
    }

    //--- Initialize voids array. Set all elements to -1, then set local minima
    for (i=0; i<n_pixels; i++) voids[i] = -1;
    for (i=0; i<n_local_minima; i++) voids[local_minima[i]] = i+1;

    //==================================
    //--- Sort density field. Get permutation indexes
    //==================================
    printf(">>> Sorting densities. This may take a while...\n");
    fflush(stdout);
    sorted_indexes = sort_index_double(density, n_pixels);

    //--- Loop over all pixels. This is the core of the watershed transform.
    //    Start with the less dense pixel that is not a minimim and assign it
    //    to its neighbouring minima. If there is at least one neighbouring
    //    pixel assigned to a minima then mark the current pixel as watershed.
    for (i=0; i<n_pixels; i++)
    {

        //--- Get the 3d indexes of local minima
        decomposed_index3d(xyz, sorted_indexes[i], xd_grid,yd_grid,zd_grid);
        //--- Get shell around pixel
        shell_27(shell_ind, xyz[0],xyz[1],xyz[2], x_grid,y_grid,z_grid);

        //--- Get ID of pixels inside shell
        for (w=0;w<n_pixels_shell;w++) Adj_Pixels[w] = voids[shell_ind[w]];

        //--- Find the unique indexes in shell. This function destroys
        //    the input array (Adj_Pixels).
        n_unique = get_unique_int(Adj_Pixels, n_pixels_shell);

        //--- From the unique elements select only those which are not -1, or 0.
        //    Free memory each loop.
        n_valid_unique = n_unique;
        valid_unique = where_int(0, "<" , Adj_Pixels, &n_valid_unique);

        //-------------------------------------
        //--- Now based on the number of different pixels assign ID of watershed
        //-------------------------------------
        switch (n_valid_unique)
	  {
	  case 0:
	    //--- Catch error. Cero neighbouring regions means that the flooding pixel
	    //    is embedded inside the watershed transform. This would be a one-pixel
	    //    region and therefore should have been identified as a local minima.
	    //    Probably this arises from the discrete nature of the watershed. 
	    //    Need some more serious testing of the algorithm. I don't think this
	    //    produces a topologically correct watershed but it seems to be an
	    //    uncommon case...
	    printf(">>> ERROR: No regions in shell!!!. Set pixel[%d] to cero.\n", sorted_indexes[i]);
	    voids[sorted_indexes[i]] = 0;
	    break;
	  case 1:
	    voids[sorted_indexes[i]] = Adj_Pixels[valid_unique[0]];
	    break;
	  default:
	    //--- More than two regions then this is a watershed line
	    voids[sorted_indexes[i]] = 0;
	  } //--- end switch
	
        //--- Free memory allocated by where_int() function
        if (n_valid_unique > 0) free(valid_unique);
	
    } //--- end for

    return voids;
}


//=========================================================
//
//=========================================================
int *watershed_sorted_int_07(double *density, int x_grid, int y_grid, int z_grid)
{

    //--- General variables
    int    i,j,k,w;
    //--- Shell
    int   n_pixels_shell = 7;
    int    Ind_Shell[n_pixels_shell];
    int   *shell_ind = Ind_Shell;
    //--- Decompose3D
    int    xyz_temp[3];
    int   *xyz = xyz_temp;
    //--- Double versions
    double xd_grid,yd_grid,zd_grid;
    int    n_pixels = x_grid*y_grid*z_grid;
    //--- Pointer to voids array
    int   *voids;
    //--- Permutation indexes after sorting in density
    int   *sorted_indexes;
    //--- Adjacent pixels
    int    Adj_Pixels[n_pixels_shell];
    //--- Number of unique pixels ID in shell
    int    n_unique = 0;
    //--- Array to store result from where_usint()
    int   *valid_unique;
    int    n_valid_unique;

    //--- Double versions of grid size
    xd_grid = (double) x_grid;
    yd_grid = (double) y_grid;
    zd_grid = (double) z_grid;

    //==================================
    //--- Find local minima
    //==================================
    //--- Local minima
    int *local_minima;
    int  n_local_minima=0;
    local_minima = find_minima_int_07(density, x_grid,y_grid,z_grid, &n_local_minima);
    printf("Ready finding minima [%d]\n", n_local_minima);
    fflush(stdout);

    //--- Allocate memory for voids
    if ((voids = (int *) malloc(sizeof(int)*n_pixels )) == NULL)
    {
        printf(">>> ERROR: malloc failed allocating memory for voids. \n    Exit program... \n\n");
        fflush(stdout);
        exit (1);
    }

    //--- Initialize voids array. Set all elements to -1, then set local minima
    for (i=0; i<n_pixels; i++) voids[i] = -1;
    for (i=0; i<n_local_minima; i++) voids[local_minima[i]] = i+1;

    //==================================
    //--- Sort density field. Get permutation indexes
    //==================================
    printf(">>> Sorting densities. This may take a while...\n");
    fflush(stdout);
    sorted_indexes = sort_index_double(density, n_pixels);

    //--- Loop over all pixels. This is the core of the watershed transform.
    //    Start with the less dense pixel that is not a minimim and assign it
    //    to its neighbouring minima. If there is at least one neighbouring
    //    pixel assigned to a minima then mark the current pixel as watershed.
    for (i=0; i<n_pixels; i++)
    {

        //--- Get the 3d indexes of local minima
        decomposed_index3d(xyz, sorted_indexes[i], xd_grid,yd_grid,zd_grid);
        //--- Get shell around pixel
        shell_07(shell_ind, xyz[0],xyz[1],xyz[2], x_grid,y_grid,z_grid);

        //--- Get ID of pixels inside shell
        for (w=0;w<n_pixels_shell;w++) Adj_Pixels[w] = voids[shell_ind[w]];

        //--- Find the unique indexes in shell. This function destroys
        //    the input array (Adj_Pixels).
        n_unique = get_unique_int(Adj_Pixels, n_pixels_shell);

        //--- From the unique elements select only those which are not -1, of 0.
        //    Free memory each loop.
        n_valid_unique = n_unique;
        valid_unique = where_int(0, "<" , Adj_Pixels, &n_valid_unique);

        //-------------------------------------
        //--- Now based on the number of different pixels assign ID of watershed
        //-------------------------------------
        switch (n_valid_unique)
	  {
	  case 0:
	    //--- Catch error. Cero neighbouring regions means that the flooding pixel
	    //    is embedded inside the watershed transform. This would be a one-pixel
	    //    region and therefore should have been identified as a local minima.
	    //    Probably this arises from the discrete nature of the watershed. 
	    //    Need some more serious testing of the algorithm. I don't think this
	    //    produces a topologically correct watershed but it seems to be an
	    //    uncommon case...
	    //printf(">>> ERROR: No regions in shell!!!. Set pixel[%d] to cero.\n", sorted_indexes[i]); //--- OJO: commneted this warning
	    voids[sorted_indexes[i]] = 0;
	    break;
	  case 1:
	    voids[sorted_indexes[i]] = Adj_Pixels[valid_unique[0]];
	    break;
	  default:
	    //--- More than two regions then this is a watershed line
	    voids[sorted_indexes[i]] = 0;
	  } //--- end switch
	
        //--- Free memory allocated by where_int() function
        if (n_valid_unique > 0) free(valid_unique);

    } //--- end for

    return voids;
}





