/*


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "inc/water_hierarchy.h"
#include "../lib/pixel3.h"
#include "../lib/numeric.h"
#include "../lib/find_minima.h"

//--- The tree node
struct Void {
  int    ID;       //--- ID of node
  int    Level;    //--- Level in tree

  //--- Void properties
  float  Volume;
  float  Area;
  float  MinDen;
  float  MaxDen;
  float  MeanDen;
  float  MedianDen;

};

//=========================================================
//=========================================================
int *void_basic_properties(const double *density, const int *voids1, int n_pixels)
{
    int     i,j,k;
    int    *histo1=NULL;
    int    *histo1_index=NULL;
    int     histo1_index_off=0;
    int     n_histo1=0;
    //--- Void cells
    double *void_density=NULL;

    //--- Compute histogram and store indexes
    histo1       = histogram_int(voids1, n_pixels, &n_histo1);
    histo1_index = histogram_int_index(voids1, n_pixels);

    //=============================================
    //--- Loop over all regions and compute properties
    //=============================================
    histo1_index_off = histo1[0]; //--- Start at region 1, 0 is the watershed
    for (i=1;i<n_histo1;i++)
    {

      //--- Number of pixels in this void
      n_void_pix = histo1[i];
      
      //--- Skip empty bins
      if (n_void_pix == 0) continue;

      //--- Allocate memory for region
      void_density = (double *) malloc(sizeof(double)*n_void_pix);
      for (j=0;j<n_void_pix;j++) void_density[j] = density[histo1_index[histo1_index_off + j]];

      //================================
      //--- Fill void structure
      //================================
      void[i].ID      = i;

      //--- Fill void properties
      void[i].Volume  = (float) n_void_pix;

      //--- Compute mass
      for (j=0;j<histo1[i];j++) void[i].mass += void_density[j];
      
      //--- Mean density
      void[i].MeanDen = void[i].mass/void[i].volume;
      
      //--- Minimum density
      void[i].MinDen  = min_double(void_density, n_void_pix);

      //--- Maximum density
      void[i].MaxDen  = max_double(void_density, n_void_pix);




      //--- Increase offset of histo_index1
      histo1_index_off = histo1_index_off+n_void_pix;

      //--- Free array, otherwise bad memory leak...
      free(void_density);

    } //--- end for i
    
}


