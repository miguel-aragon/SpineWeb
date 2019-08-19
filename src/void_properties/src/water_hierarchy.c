/*


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../inc/water_hierarchy.h"
#include "../../lib/pixel3.h"
#include "../../lib/numeric.h"
#include "../../lib/find_minima.h"

//=========================================================
//--- This is a variation of watershed_sorted_int_27().
//    Here we pass an array with regions Id's (voids12) and use
//    this array to define the ID's of the local minima.
//    This acts as a marked watershed where we put marks at
//    the position of local minima.
//=========================================================
int *watershed_sorted_int_27_marked(double *density, int *voids12, int x_grid, int y_grid, int z_grid)
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
    //--- Debugging
    char   wait_char;
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

    //==========================================
    //--- OJO: Here is where we "mark" the local minima.
    //    The local minima indexes are given by the Id's in the
    //    input array voids12.
    //==========================================
    for (i=0; i<n_local_minima; i++) voids[local_minima[i]] = voids12[local_minima[i]];

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
//   Merge regions. This function does not depends on the
//   arrangement of the pixels but only on their Id's.
//   This means it can be used for delaunay watersheds. 
//   However, in the case of orphan regions (regions whose
//   parent is the watershed transform) one has to introduce
//   an adopt_orphan() function. This may involve the use
//   of local neighbourhood (shell or umbrella).
//=========================================================
int *merge_regions_int(const int *voids1, const int *voids2, int n_pixels)
{
    int  i,j,k;
    int *histo1=NULL;
    int *histo1_index=NULL;
    int  histo1_index_off=0;
    int  n_histo1=0;
    //--- Output array
    int *voids12=NULL;
    //--- Intersection
    int *inter_array=NULL;
    int *histo_inter_array=NULL;
    int  n_histo_inter_array=0;
    //--- Trick to get the maximum element that is not 0
    int *histo_inter_array_off=NULL;
    //--- maximum contribution
    int *where_max=NULL;
    int  n_where_max;
    int  max_inter_ID;

    //--- Allocate memory for merge-scale array
    if ((voids12 = (int *) malloc(sizeof(int)*n_pixels )) == NULL)
    {
        printf(">>> ERROR: malloc failed. Exit program... \n\n");
        exit (1);
    }

    //--- Clean array. This can be done only for the watershed pixels.
    for (i=0;i<n_pixels;i++) voids12[i] = 0;

    histo1       = histogram_int(voids1, n_pixels, &n_histo1);
    histo1_index = histogram_int_index(voids1, n_pixels);

    //=============================================
    //--- Loop over all regions in level 1 and find the intersection regions in
    //    level 2.
    //=============================================
    histo1_index_off = histo1[0]; //--- Start at region 1, 0 is the watershed
    for (i=1;i<n_histo1;i++)
    {

        //--- Skip empty bins
        if (histo1[i] == 0) continue;

        //--- get the intersection between voids 1 and voids2. Check which regions
        //    in voids2 are covered by region with Id i in voids1
        //--- Allocate memory for intersection
        inter_array = (int *) malloc(sizeof(int)*histo1[i]);
        for (j=0;j<histo1[i];j++) inter_array[j] = voids2[histo1_index[histo1_index_off + j]];

        //--- Histogram of indexes inside intersection
        histo_inter_array = histogram_int(inter_array, histo1[i], &n_histo_inter_array);

        //--- Get the region in voids2 to which region i in voids1 contributes
        //    the most. There may be an degeneracy in the case of two or more
        //    maximum. In this case just take one. One could improve this by using
        //    density to choose between masimum or something like that.
        //    In the case of the delaunay if one takes the volume this should not be
        //    a problem. Here arises from the fix size of the pixels.
        n_where_max = n_histo_inter_array;
        where_max   = where_int(max_int(histo_inter_array,n_histo_inter_array),"=",histo_inter_array, &n_where_max);

        //--- Add offset in case 0 is not the minimim Id in inter_array. Not sure if this
        //    actually works. The second line seems to make more sense...
        //max_inter_ID = min_int(inter_array, histo1[i]) + where_max[0];
        max_inter_ID = where_max[0];


        //--- If the parent is the watershed (Id=0) then choose the second largest. In the case of using the
	//    incomplete watershed (only flooding) this should be redundant since there are no ceroes.
        if (max_inter_ID == 0)
        {
            //--- Find the largest element in histo_inter_array that is not 0. In this case it is easy since
            //    we know that cero is ALWAYS the first element. It may be that inter_array does not contain
            //    ceroes but that case is excluded by the if(max_inter_ID == 0).
            n_where_max = n_histo_inter_array-1;
            histo_inter_array_off = histo_inter_array;
            histo_inter_array_off++;
            where_max   = where_int(max_int(histo_inter_array_off,n_histo_inter_array-1),"=",histo_inter_array_off, &n_where_max);
            max_inter_ID = where_max[0]+1;
        }

        //--- Fill region with its parent's ID
        for (j=0;j<histo1[i];j++) voids12[histo1_index[histo1_index_off + j]] = max_inter_ID;

        //--- Increase offset of histo_index1
        histo1_index_off = histo1_index_off+histo1[i];
        //--- Free array, otherwise bad memory leak...
        free(inter_array);
    }

    return voids12;
}

//=========================================================
//   Merge regions in a regular grid. Here we introduce an
//   adopt_orphan() function to reassign orphan regions
//   (regions whose parent is the watershed transform itself).
//=========================================================
int *merge_regions_grid_int(const int *voids1, const int *voids2, int n_pixels, int x_grid, int y_grid, int z_grid)
{
    int  i,j,k;
    int *histo1=NULL;
    int *histo1_index=NULL;
    int  histo1_index_off=0;
    int  n_histo1=0;
    //--- Output array
    int  *voids12=NULL;
    //--- Intersection
    int  *inter_array=NULL;
    int  *histo_inter_array=NULL;
    int  n_histo_inter_array=0;
    //--- Trick to get the maximum element that is not 0
    int *histo_inter_array_off=NULL;
    //--- maximum contribution
    int *where_max=NULL;
    int  n_where_max;
    int  max_inter_ID;

    //--- Allocate memory for merge-scale array
    if ((voids12 = (int *) malloc(sizeof(int)*n_pixels )) == NULL)
    {
        printf(">>> ERROR: malloc failed. Exit program... \n\n");
        exit (1);
    }

    //--- Clean array. This can be done only for the watershed pixels.
    for (i=0;i<n_pixels;i++) voids12[i] = 0;

    fflush(stdout);
    histo1       = histogram_int(voids1, n_pixels, &n_histo1);
    fflush(stdout);
    histo1_index = histogram_int_index(voids1, n_pixels);
    fflush(stdout);

    //=============================================
    //--- Loop over all regions in level 1 and find the intersection regions in
    //    level 2.
    //=============================================
    histo1_index_off = histo1[0]; //--- Start at region 1, 0 is the watershed
    for (i=1;i<n_histo1;i++)
    {

        //--- Skip empty bins
        if (histo1[i] == 0) continue;

        //--- get the intersection between voids 1 and voids2. Check which regions
        //    in voids2 are covered by region with Id i in voids1
        //--- Allocate memory for intersection
        inter_array = (int *) malloc(sizeof(int)*histo1[i]);
        for (j=0;j<histo1[i];j++) inter_array[j] = voids2[histo1_index[histo1_index_off + j]];

        //--- Histogram of indexes inside intersection
        histo_inter_array = histogram_int(inter_array, histo1[i], &n_histo_inter_array);

        //--- Get the region in voids2 to which region i in voids1 contributes
        //    the most. There may be an degeneracy in the case of two or more
        //    maximum. In this case just take one. One could improve this by using
        //    density to choose between masimum or something like that.
        //    In the case of the delaunay if one takes the volume this should not be
        //    a problem. Here arises from the fix size of the pixels.
        n_where_max = n_histo_inter_array;
        where_max   = where_int(max_int(histo_inter_array,n_histo_inter_array),"=",histo_inter_array, &n_where_max);

        //--- Add offset in case 0 is not the minimum Id in inter_array. Not sure if this
        //    actually works. The second line seems to make more sense...
        //max_inter_ID = min_int(inter_array, histo1[i]) + where_max[0];
        max_inter_ID = where_max[0];
	
        //--- If the parent is the watershed (Id=0) then choose the second largest
        if (max_inter_ID == 0)
	  {
	    //--- First check the special case where ALL the intersecting cells are ceroes.
	    //    This corresponds to orphan cells whose parent is the watershed tranform.
	    //    Loop over all cell in intersection. If all are ceroes the adopt orphan.
	    //    The present implementation gives a random adoption. Since the orphan cell
	    //    is (should) be small this random assignment should not have impact in the
	    //    final tree.
	    
	    //--- If the watershed is the maximum contributing region AND is the ONLY contributor then...
	    if (n_where_max == 1) 
	      {
		max_inter_ID = adopt_orphan_int(voids1,voids2, i, histo1[i], x_grid, y_grid, z_grid);
	      }
	    //--- Otherwise get the second largest contributor
	    else 
	      {
		//--- Find the largest element in histo_inter_array that is not 0. In this case it is easy since
		//    we know that cero is ALWAYS the first element. It may be that inter_array does not contain
		//    ceroes but that case is excluded by the if(max_inter_ID == 0).
		n_where_max = n_histo_inter_array-1;
		histo_inter_array_off = histo_inter_array;
		histo_inter_array_off++;
		where_max    = where_int(max_int(histo_inter_array_off,n_histo_inter_array-1),"=",histo_inter_array_off, &n_where_max);
		max_inter_ID = where_max[0]+1;
	      }
	  }
	
        //--- Fill region with its parent's ID
        for (j=0;j<histo1[i];j++) voids12[histo1_index[histo1_index_off + j]] = max_inter_ID;
	
        //--- Increase offset of histo_index1. This step is very important!!!
        histo1_index_off = histo1_index_off+histo1[i];

        //--- Free array, otherwise bad memory leak...
        free(inter_array);
    }

    return voids12;
}


//=========================================================
//
//=========================================================
int adopt_orphan_int(const int *voids1, const int *voids2, int orphan_ID, int n_orphan, int x_grid, int y_grid, int z_grid)
{
  int i,j;
  //--- Shell
  int    n_pixels_shell = 27;
  int    Ind_Shell[n_pixels_shell];
  int   *shell_ind = Ind_Shell;
  //--- Decompose3D
  int    xyz_temp[3];
  int   *xyz = xyz_temp;
  //--- Double versions
  double xd_grid=(double) x_grid,yd_grid=(double) y_grid,zd_grid=(double) z_grid;
  //--- Adjacent pixels
  int    Adj_Pixels[n_pixels_shell];
  //--- Find cells of orphan region
  int   *where_orphan=NULL;
  int    n_where_orphan;

  n_where_orphan = x_grid*y_grid*z_grid;
  where_orphan   = where_int(orphan_ID,"=", voids1, &n_where_orphan);

  for (i=0;i<n_where_orphan;i++){ 

    //--- Get the 3d indexes of local minima
    decomposed_index3d(xyz, where_orphan[i], xd_grid,yd_grid,zd_grid);
    //--- Get shell around pixel
    shell_27(shell_ind, xyz[0],xyz[1],xyz[2], x_grid,y_grid,z_grid);
    
    //--- Get ID of pixels inside shell
    for (j=0;j<n_pixels_shell;j++) {
      //--- OJO: This is the simples solution. Just look in the outer shell of the orphan region
      //    and assign the first adjacent region that comes. Since the orphan region should be
      //    very small < 100 cells this "random" adoption should not make a difference in the final
      //    region/void tree.
      if ((voids2[shell_ind[j]] != 0) && (Adj_Pixels[j] != voids1[shell_ind[j]])) return  voids2[shell_ind[j]];
    } // end for j 
  } // end for i

  //--- Default return value.  If no adoptive parents found. This should not happen unless
  //    the orphan region is embedded in a watershed transform which is several cell-thick.
  //    Very unlikely but still possible.
  return -1;
}


//=========================================================
//   Fix indexes after merging
//=========================================================
void fix_watershed_IDs(int *voids1, int n_pixels)
{
    int  i,j,k;
    int *histo1=NULL;
    int *histo1_index=NULL;
    int  histo1_index_off=0;
    int  n_histo1=0;
    //--- Intersection
    int *inter_array=NULL;
    int *histo_inter_array=NULL;
    int  n_histo_inter_array=0;
    //--- Region counter
    int  cont_region=1;

    //--- Compute hitogram of void regions
    fflush(stdout);
    histo1       = histogram_int(voids1, n_pixels, &n_histo1);
    fflush(stdout);
    histo1_index = histogram_int_index(voids1, n_pixels);
    fflush(stdout);

    //=============================================
    //--- Loop over all regions in level 1 and find the intersection regions in
    //    level 2.
    //=============================================
    histo1_index_off = histo1[0]; //--- Start at region 1, 0 is the watershed
    cont_region = 1;
    for (i=1;i<n_histo1;i++)
    {

        //--- Skip empty bins
        if (histo1[i] == 0) continue;

        //--- Reassing Id's to regions in place
        for (j=0;j<histo1[i];j++) voids1[histo1_index[histo1_index_off + j]] = cont_region;

        //--- Increase offset of histo_index1. This step is very important!!!
        histo1_index_off = histo1_index_off+histo1[i];
	
	cont_region++;

    } // end for i
    
    printf(">>> Reassigned Id's for %d voids\n", cont_region-1);

}


