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
#include "pixel3.h"


/*===============================================================      
  NAME
   where_char

 DESCRIPTION
   Simpliffied c version of the where() function. 

 INPUT
   compare       Float indicating the number to compare
   *option       Choose one relationship <, >, ==, !=
   *arreglo      The array being compared
   *n_arreglo    Number of elements in array to be compared

 OUTPUT
   *where_array  Return a pointer to an array containing the 
                 indexes of valid elements in *arreglo. Valid 
                 elements are those who follow the *option relationship
   *n_arreglo    IMPORTANT: the value of n_arreglo is changed to the
                 number of elements in *where_array


*/
int *where_char(char compare, char *option, const char *arreglo, int *n_arreglo){
  int   i,cont;
  int *where_array=NULL;

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
   where_usint

 DESCRIPTION
   Simpliffied c version of the where() function. 

 INPUT
   compare       Float indicating the number to compare
   *option       Choose one relationship <, >, ==, !=
   *arreglo      The array being compared
   *n_arreglo    Number of elements in array to be compared

 OUTPUT
   *where_array  Return a pointer to an array containing the 
                 indexes of valid elements in *arreglo. Valid 
                 elements are those who follow the *option relationship
   *n_arreglo    IMPORTANT: the value of n_arreglo is changed to the
                 number of elements in *where_array


*/
int *where_usint(int compare, char *option, const unsigned short int *arreglo, int *n_arreglo){
  int i,cont=0;
  int *where_array=NULL;

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
   where_int

 DESCRIPTION
   Simpliffied c version of the where() function. 

 INPUT
   compare       Float indicating the number to compare
   *option       Choose one relationship <, >, ==, !=
   *arreglo      The array being compared
   *n_arreglo    Number of elements in array to be compared

 OUTPUT
   *where_array  Return a pointer to an array containing the 
                 indexes of valid elements in *arreglo. Valid 
                 elements are those who follow the *option relationship
   *n_arreglo    IMPORTANT: the value of n_arreglo is changed to the
                 number of elements in *where_array

*/
int *where_int(int compare, char *option, const int *arreglo, int *n_arreglo){
  int i,cont=0;
  int *where_array=NULL;

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
   where_float

 DESCRIPTION
   Simpliffied c version of the where() function. 

 INPUT
   compare       Float indicating the number to compare
   *option       Choose one relationship <, >, ==, !=
   *arreglo      The array being compared
   *n_arreglo    Number of elements in array to be compared

 OUTPUT
   *where_array  Return a pointer to an array containing the 
                 indexes of valid elements in *arreglo. Valid 
                 elements are those who follow the *option relationship
   *n_arreglo    IMPORTANT: the value of n_arreglo is changed to the
                 number of elements in *where_array
*/
int *where_float(float compare, char *option, const float *arreglo, int *n_arreglo){
  int  i,cont;
  int  *where_array=NULL;

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
   where_double

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
int *where_double(double compare, char *option, const double *arreglo, int *n_arreglo){
  int  i,cont;
  int  *where_array=NULL;

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
   max_int() 

 DESCRIPTION
   Find maximum of array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Maximum value in the array

*/
int max_int(const int *arreglo, int n_arreglo){
  int i;
  int max_number;

  max_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (max_number < arreglo[i]) max_number = arreglo[i];

  return max_number;			      
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
float max_float(const float *arreglo, int n_arreglo){
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
double max_double(const double *arreglo, int n_arreglo){
  int i;
  double max_number;

  max_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (max_number < arreglo[i]) max_number = arreglo[i];

  return max_number;			      
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
int min_int(const int *arreglo, int n_arreglo){
  int i;
  int min_number;

  min_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (min_number > arreglo[i]) min_number = arreglo[i]; 

  return min_number;			      
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
float min_float(const float *arreglo, int n_arreglo){
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
double min_double(const double *arreglo, int n_arreglo){
  int i;
  double min_number;

  min_number = arreglo[0];
  for (i=0;i<n_arreglo;i++) if (min_number > arreglo[i]) min_number = arreglo[i]; 

  return min_number;			      
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
int total_int(const int *data, int n_data){
  int i;
  int total_data=0;

  for (i=0;i<n_data;i++) total_data = total_data + data[i];

  return total_data;			      
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
float total_float(const float *data, int n_data){
  int i;
  float total_data=0;

  for (i=0;i<n_data;i++) total_data = total_data + data[i];

  return total_data;			      
}

/*===============================================================
  NAME
   total_double() 

 DESCRIPTION
   Sum of all elements in array

 INPUT
   *arreglo       Pointer to array 
   n_arreglo      Number of elements in array

 OUTPUT
   max_number     Minimum value in the array

*/
double total_double(const double *data, int n_data){
  int i;
  double total_data=0;

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
int count_different_usi(unsigned short int region[], int n_array_in)
{
  int w, q;
  int unique, new_different;
  int comp_array[n_array_in];

  //--- Start counter, all regions have at least one index
  unique = 1;
  comp_array[0] = region[0];

  //--- Loop over all elements of region[]...
  for(w=0; w<n_array_in; w++){
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
    This routine counts the number of non-equal elements
    inside an array. The algorithm is quite naive but works fine.
    It is based on a very "human" approach: go through the array
    and write in a separate "comparison" array (comp_array[]) 
    the elements that are not repeated. I think it is efficient
    if the number of nonequal elements is small compared to the
    number of elements in the array. If every element is different
    then it should go something like O(N^2).
*/
int count_different_int(int region[], int n_array_in)
{
  int w, q;
  int unique, new_different;
  int comp_array[n_array_in];
  
  //--- Start counter, all regions have at least one index
  unique = 1;
  comp_array[0] = region[0];

  //--- Loop over all elements of region[]...
  for(w=0; w<n_array_in; w++){
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
    This routine returns the non-equal elements
    inside an array. The algorithm is quite naive but works fine.
    It is based on a very "human" approach: go through the array
    and write in a separate "comparison" array (comp_array[]) 
    the elements that are not repeated. I think it is efficient
    if the number of nonequal elements is small compared to the
    number of elements in the array. If every element is different
    then it should go something like O(N^2).
    OJO: This function overwrites the content of region. After using
    the function only n_array_in elements will be valid.
*/
int get_unique_usi(unsigned short int region[], int n_array_in)
{
  int w, q;
  int n_unique, new_different;

  //--- Start counter, all regions have at least one index
  n_unique = 1;

  //--- Loop over all elements of region[]...
  for(w=0; w<n_array_in; w++){
    //--- Loop over the elements of comp_array[] to see
    //    if the new one is already there.
    new_different = 1;
    for (q=0;q<n_unique;q++){
      if (region[q] == region[w]) new_different = 0;
    }

    //--- Update number of different elements and comparison array.
    if (new_different){
      region[n_unique] = region[w];
      ++n_unique;
    }
  }

  return n_unique;

}

/*===============================================================
    This routine returns the non-equal elements
    inside an array. The algorithm is quite naive but works fine.
    It is based on a very "human" approach: go through the array
    and write in a separate "comparison" array (comp_array[]) 
    the elements that are not repeated. I think it is efficient
    if the number of nonequal elements is small compared to the
    number of elements in the array. If every element is different
    then it should go something like O(N^2).
    OJO: This function overwrites the content of region. After using
    the function only n_array_in elements will be valid.
*/
int get_unique_int(int region[], int n_array_in)
{
  int w, q;
  int n_unique, new_different;

  //--- Start counter, all regions have at least one index
  n_unique = 1;

  //--- Loop over all elements of region[]...
  for(w=0; w<n_array_in; w++){
    //--- Loop over the elements of comp_array[] to see
    //    if the new one is already there.
    new_different = 1;
    for (q=0;q<n_unique;q++){
      if (region[q] == region[w]) new_different = 0;
    }

    //--- Update number of different elements and comparison array.
    if (new_different){
      region[n_unique] = region[w];
      ++n_unique;
    }
  }

  //for (w=0;w<n_unique;w++) printf("[%d] ", region[w]);

  return n_unique;

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
int *histogram_int(const int *data, int n_data, int *n_bins)
{
  int i,n_bin;
  int *histo_array;

  //--- Get number of elements. Add one element for bin zero
  n_bin = max_int(data, n_data) + 1;

  //--- Create array to store histogram.
  histo_array = (int *) malloc(n_bin  * sizeof(int *));

  //--- Initialize histogram array. Important!
  for (i=0; i<n_bin; i++) histo_array[i] = 0;

  //--- Fill cells with counter and elements
  for (i=0; i<n_data; i++) histo_array[data[i]] = histo_array[data[i]]+1;

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
int *histogram_int_index(const int *data, int n_data)
{
  int i,n_bin;
  int *histo_array, *histo_index, *histo_cumul;
  
  //--- Get number of elements. Add one element for bin zero
  n_bin = max_int(data, n_data) + 1;
  
  //--- Create array to store histogram.
  histo_array = (int *) malloc(n_bin  * sizeof(int *));
  histo_cumul = (int *) malloc(n_bin  * sizeof(int *));
  histo_index = (int *) malloc(n_data * sizeof(int));
  
  //--- Initialize histogram array. Important!
  for (i=0; i<n_bin; i++) histo_array[i] = 0;
  
  //--- Fill bins of histogram
  for (i=0; i<n_data; i++) histo_array[data[i]] = histo_array[data[i]]+1;
  
  //--- Cumulative histogram. This array indicates the position of the
  //    first element in each bin. Initialize only first element.
  histo_cumul[0] = 0;
  for (i=1; i<n_bin; i++) histo_cumul[i] = histo_cumul[i-1] + histo_array[i-1];
  
  //--- Store indexes of histogram
  for (i=0; i<n_data; i++) {
    histo_index[histo_cumul[data[i]]] = i;
    histo_cumul[data[i]] = histo_cumul[data[i]]+1;
  }
  
  return histo_index;
}

//==========================================
//   SORT_INDEX_FLOAT
//==========================================
int *sort_index_float(float *array, int n_array)
{

  float **parray; //--- Pointer to pointers
  int   *sorted_indexes=malloc(n_array*sizeof(int));
  int   i;
  float *ptr_temp=NULL;

  //--- Allocate memory for pointer-to-pointers
  parray = (float **)malloc(n_array *sizeof(float*));

  //--- Fill array of pointers to addresses of data
  for(i = 0; i < n_array;i++) parray[i] = &array[i];

  //--- Sort array of pointers
  qsort(parray,n_array, sizeof *parray,compare_float);

  //--- Fill array with sorted indexes
  for(i = 0;i < n_array;i++) sorted_indexes[i] = parray[i]-&array[0];

  return sorted_indexes;

}

//==========================================
//   SORT_INDEX_DOUBLE
//==========================================
int *sort_index_double(double *array, int n_array)
{

  double **parray; //--- Pointer to pointers
  int    *sorted_indexes=malloc(n_array*sizeof(int));
  int    i;

  //--- Allocate memory for pointer-to-pointers
  parray = (double **)malloc(n_array *sizeof(double*));

  //--- Fill array of pointers to addresses of data
  for(i = 0; i < n_array;i++) parray[i] = &array[i];

  //--- Sort array of pointers
  qsort(parray,n_array, sizeof *parray,compare_double);

  //--- Fill array with sorted indexes
  for(i = 0;i < n_array;i++) sorted_indexes[i] = parray[i]-&array[0];

  return sorted_indexes;

}


//==========================================
//   COMPARE_FLOAT
//==========================================
int compare_float(const void *v1, const void *v2)
{
const float i1 = **(const float **)v1;
const float i2 = **(const float **)v2;
 if (i2>=i1) return 0;
 else        return 1;
}

//==========================================
//   COMPARE_DOUBLE
//==========================================
int compare_double(const void *v1, const void *v2)
{
const double i1 = **(const double **)v1;
const double i2 = **(const double **)v2;
 if (i2>=i1) return 0;
 else        return 1;
}



//==========================================
//   MORPHOLOGICAL DILATION
//==========================================
char *dilate_int(const int *data, int grid_X, int grid_Y, int grid_Z)
{
  int  i,j,k,w;
  //--- Shell
  int  n_pixels_shell = 27;
  int  Ind_Shell[n_pixels_shell];
  int *shell_ind = Ind_Shell;
  //--- Total number of pixels
  int  n_pixels = grid_X*grid_Y*grid_Z;

  //--- Allocate output array
  char *data_dilated = (char *) malloc(sizeof(char *) * n_pixels);


  //--- Loop over all pixels and erode
  for (i=0; i<grid_X; i++){
    for (j=0; j<grid_Y; j++){
      for (k=0; k<grid_Z; k++){

	//--- Get shell around pixel
	shell_27(shell_ind, i,j,k, grid_X,grid_Y,grid_Z);

	//--- Only zeroes
	if (data[shell_ind[0]] > 0) continue;
  
	//--- If at least one element in shell is non zero
	//    then set pixel to zero.
	for (w=0;w<n_pixels_shell;w++) {
	  if (data[shell_ind[w]]) {
	    data_dilated[shell_ind[0]] = 1;
	    break;
	  }
	}
	
      }
    }
  }
  
  return data_dilated;
}


//==========================================
//   MORPHOLOGICAL EROSION
//==========================================
char *erode_int(const int *data, int grid_X, int grid_Y, int grid_Z)
{
  int  i,j,k,w;
  //--- Shell
  int  n_pixels_shell = 27;
  int  Ind_Shell[n_pixels_shell];
  int *shell_ind = Ind_Shell;
  //--- Total number of pixels
  int  n_pixels = grid_X*grid_Y*grid_Z;

  //--- Allocate output array
  char *data_dilated = (char *) malloc(sizeof(char *) * n_pixels);


  //--- Loop over all pixels and erode
  for (i=0; i<grid_X; i++){
    for (j=0; j<grid_Y; j++){
      for (k=0; k<grid_Z; k++){

	//--- Get shell around pixel
	shell_27(shell_ind, i,j,k, grid_X,grid_Y,grid_Z);

	//--- Only zeroes
	if (data[shell_ind[0]] == 0) continue;
  
	//--- If at least one element in shell is non zero
	//    then set pixel to zero.
	for (w=0;w<n_pixels_shell;w++) {
	  if (!data[shell_ind[w]]) {
	    data_dilated[shell_ind[0]] = 0;
	    break;
	  }
	}
	
      }
    }
  }
  
  return data_dilated;
}

