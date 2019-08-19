/*

   

   Written by: Miguel Angel Aragon Calvo. 
	       miguel@pha.jhu.edu
               miguel@astro.rug.nl


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "numeric.h"


/*===============================================================
  NAME
   where_float

 DESCRIPTION
   Simpliffied c version of the where() function. 

 INPUT
   compare       Float indicating the number to compare
   *arreglo      The array being compared
   *n_arreglo    Number of elements in array to be compared
   *option       Choose one relationship <, >, ==, !=

 OUTPUT
   *where_array  Return a pointer to an array containing the 
                 indexes of valid elements in *arreglo. Valid 
                 elements are those who follow the *option relationship
   *n_arreglo    IMPORTANT: the value of n_arreglo is changed to the
                 number of elements in *where_array
*/
int *where_float(float compare, char *option, float *arreglo, int *n_arreglo){
  int  i,cont;
  int  *where_array;

  switch( *option ) 
    {
    case '>':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare > arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
	if(compare > arreglo[i]) {
	  where_array[cont] = i;
	  cont++;
	}
      }
      break;
    case '.':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare >= arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
	if(compare >= arreglo[i]) {
	  where_array[cont] = i;
	  cont++;
	}
      }
      break;
    case '<':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare < arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
	if(compare < arreglo[i]) {
	  where_array[cont] = i;
	  cont++;
	}
      }
      break;
    case ',':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare <= arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
	if(compare <= arreglo[i]) {
	  where_array[cont] = i;
	  cont++;
	}
      }
      break;
    case '=':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare == arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
	if(compare == arreglo[i]) {
	  where_array[cont] = i;
	  cont++;
	}
      }
      break;
    case '#':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare != arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
	if(compare != arreglo[i]) {
	  where_array[cont] = i;
	  cont++;
	}
      }
      break;
    }

  *n_arreglo = cont;

  return where_array;

}


/*===============================================================      

*/
int *where_char(char compare, char *option, char *arreglo, int *n_arreglo){
  int   i,cont;
  int *where_array;

  switch( *option )
    {
    case '>':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare > arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare > arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '.':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare >= arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare >= arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '<':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare < arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare < arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case ',':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare <= arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare <= arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '=':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare == arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare == arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '#':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare != arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare != arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    }

  *n_arreglo = cont;

  return where_array;

}


/*===============================================================      

*/
int *where_usint(unsigned short int compare, char *option, unsigned short int *arreglo, int *n_arreglo){
  int   i,cont;
  int *where_array;

  switch( *option )
    {
    case '>':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare > arreglo[i]) ++cont;
      //--- Allocate array for where   
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements  
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare > arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '.':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare >= arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare >= arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '<':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare < arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare < arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case ',':
      cont = 0;
      //--- Count valid elements
      for(i=0; i<(*n_arreglo); i++) if(compare <= arreglo[i]) ++cont;
      //--- Allocate array for where
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare <= arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '=':
      cont = 0;
      //--- Count valid elements        
      for(i=0; i<(*n_arreglo); i++) if(compare == arreglo[i]) ++cont;
      //--- Allocate array for where           
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements      
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare == arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    case '#':
      cont = 0;
      //--- Count valid elements      
      for(i=0; i<(*n_arreglo); i++) if(compare != arreglo[i]) ++cont;
      //--- Allocate array for where           
      where_array = (int *)malloc(sizeof(int)*cont);
      //--- Fill array with valid elements      
      cont = 0;
      for(i=0; i<(*n_arreglo); i++) {
        if(compare != arreglo[i]) {
          where_array[cont] = i;
          cont++;
        }
      }
      break;
    }

  *n_arreglo = cont;

  return where_array;

}



/*===============================================================
  NAME
   max_float() 

 DESCRIPTION
   Find maximum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Maximum value in the array

*/
float max_float(float *arreglo, int n_arreglo){
  int i;
  float max_number;

  max_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (max_number < arreglo[i]) max_number = arreglo[i];

  return max_number;			      
}

/*===============================================================
  NAME
   max_double()

 DESCRIPTION
   Find maximum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Maximum value in the array

*/
float max_double(double *arreglo, int n_arreglo){
  int i;
  double max_number;

  max_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (max_number < arreglo[i]) max_number = arreglo[i];

  return max_number;			      
}



/*===============================================================
  NAME
   max_int() 

 DESCRIPTION
   Find maximum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Maximum value in the array

*/
int max_int(int *arreglo, int n_arreglo){
  int i;
  int max_number;

  max_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (max_number < arreglo[i]) max_number = arreglo[i];

  return max_number;			      
}

/*===============================================================
  NAME
   min_float() 

 DESCRIPTION
   Find minimum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Minimum value in the array

*/
float min_float(float *arreglo, int n_arreglo){
  int i;
  float min_number;

  min_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (min_number > arreglo[i]) min_number = arreglo[i]; 

  return min_number;			      
}

/*===============================================================
  NAME
   min_double() 

 DESCRIPTION
   Find minimum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Minimum value in the array

*/
float min_double(double *arreglo, int n_arreglo){
  int i;
  double min_number;

  min_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (min_number > arreglo[i]) min_number = arreglo[i]; 

  return min_number;			      
}


/*===============================================================
  NAME
   min_int() 

 DESCRIPTION
   Find minimum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Minimum value in the array

*/
int min_int(int *arreglo, int n_arreglo){
  int i;
  int min_number;

  min_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (min_number > arreglo[i]) min_number = arreglo[i]; 

  return min_number;			      
}


/*===============================================================
  NAME
   total_float() 

 DESCRIPTION
   Sum of all elements in array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Minimum value in the array

*/
float total_float(float *data, int n_data){
  int i;
  float total_data=0;

  for (i=0;i<n_data;i++) total_data = total_data + data[i];

  return total_data;			      
}

/*===============================================================
  NAME
   total_int() 

 DESCRIPTION
   Sum of all elements in array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Minimum value in the array

*/
int total_int(int *data, int n_data){
  int i;
  int total_data=0;

  for (i=0;i<n_data;i++) total_data = total_data + data[i];

  return total_data;			      
}


/*===============================================================
    This routine counts the number of non-equal elements
    inside an array. The algorithm is quite naive but works fine.
    It is based on a very "human" approach: go through the array
    and write in a separate "comparison" array (comp_array[]) 
    the elements that are not repeated. I think it is efficient
    if the number of nonequal elements is small compared to the
    number of elements in the array. If every element is different
    then it should go something like O(N^2).
*/
int count_different(unsigned short int region[])
{
  int w, q;
  int unique, new_different;
  int comp_array[27];

  //--- Start counter, all regions have at least one index
  unique = 1;
  comp_array[0] = region[0];

  //--- Loop over all elements of region[]...
  for(w=0; w<27; w++){
    //--- Loop over the elements of comp_array[] to see
    //    if the new one is already there.
    new_different = 1;
    for (q=0;q<unique;q++){
      if (comp_array[q] == region[w]) new_different = 0;
    }

    //--- Update number of different elements and comparison array.
    if (new_different){
      comp_array[unique] = region[w];
      ++unique;
    }	  
  }

  return unique;
}

/*===============================================================
  NAME
   histogram_int() 

 DESCRIPTION
   Compute histogram of an int array

 INPUT
   data         Pointer to array 
   n_data       Number of elements in array

 OUTPUT
   histogram and indexes of histogram

*/
int *histogram_int(int *data, int n_data, int *n_bins)
{
  int i,n_bin;
  int *histo_array, *histo_index, *histo_cumul;

  //--- Get number of elements. Add one element for bin zero
  n_bin = max_int(data, n_data) + 1;

  //--- Create array to store histogram.
  histo_array = (int *) malloc(n_bin  * sizeof(int *));
  histo_cumul = (int *) malloc(n_bin  * sizeof(int *));
  histo_index = (int *) malloc(n_data * sizeof(int));

  //--- Fill cells with counter and elements
   for (i=0; i<n_data; i++) histo_array[data[i]] = histo_array[data[i]]+1;

  //--- Cumulative histogram. This array indicatse the position of the
  //    first element in each bin
  histo_cumul[0] = 0;
  for (i=1; i<n_bin; i++) histo_cumul[i] = histo_cumul[i-1] + histo_array[i-1];

  //--- Store indexes of histogram
  for (i=0; i<n_data; i++) {
    histo_index[histo_cumul[data[i]]] = i;
    histo_cumul[data[i]] = histo_cumul[data[i]]+1;
   }

  *n_bins = n_bin;  //--- return this value
  return histo_array;
}

/*===============================================================
  NAME
   histogram_int_index() 

 DESCRIPTION
   Compute histogram of an int array and return its indexes

 INPUT
   data         Pointer to array 
   n_data       Number of elements in array

 USAGE

  int *histo, n_histo=0;
  int *histo_index;
  int *histo_cumul;

  histo       = histogram_int(data, n_data, &n_histo);
  histo_index = histogram_int_index(data, n_data);

  //--- Cumulative histogram
  histo_cumul = (int *) malloc(n_histo * sizeof(int *));
  histo_cumul[0] = 0;
  for (i=1; i<n_histo; i++) histo_cumul[i] = histo_cumul[i-1] + histo[i-1];

  for (i=0; i<n_histo; i++) {
    if (histo[i] == 0) continue; /--- No elements in bin
    for (j=0;j<histo[i];j++) printf("Group[%d] = %d \n",i, data[histo_index[histo_cumul[i]+j]] );
    printf("--\n");
  }


 OUTPUT
   histogram and indexes of histogram

*/
int *histogram_int_index(int *data, int n_data)
{
  int i,n_bin;
  int *histo_array, *histo_index, *histo_cumul;

  //--- Get number of elements. Add one element for bin zero
  n_bin = max_int(data, n_data) + 1;

  //--- Create array to store histogram.
  histo_array = (int *) malloc(n_bin  * sizeof(int *));
  histo_cumul = (int *) malloc(n_bin  * sizeof(int *));
  histo_index = (int *) malloc(n_data * sizeof(int));

  //--- Fill cells with counter and elements
   for (i=0; i<n_data; i++) histo_array[data[i]] = histo_array[data[i]]+1;

  //--- Cumulative histogram. This array indicatse the position of the
  //    first element in each bin
  histo_cumul[0] = 0;
  for (i=1; i<n_bin; i++) histo_cumul[i] = histo_cumul[i-1] + histo_array[i-1];

  //--- Store indexes of histogram
  for (i=0; i<n_data; i++) {
    histo_index[histo_cumul[data[i]]] = i;
    histo_cumul[data[i]] = histo_cumul[data[i]]+1;
   }

  return histo_index;
}

