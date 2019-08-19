#ifndef HEADER_NUMERIC_H
#define HEADER_NUMERIC_H
/*





*/



#include <stdlib.h>
#include <string.h>
#include <math.h>


int   *where_char  (char   compare, char *option, const char               *arreglo, int *n_arreglo);
int   *where_usint (int    compare, char *option, const unsigned short int *arreglo, int *n_arreglo);
int   *where_int   (int    compare, char *option, const                int *arreglo, int *n_arreglo);
int   *where_float (float  compare, char *option, const float              *arreglo, int *n_arreglo);
int   *where_double(double compare, char *option, const double             *arreglo, int *n_arreglo);

int    max_int    (const int    *arreglo, int n_arreglo);
float  max_float  (const float  *arreglo, int n_arreglo);
double max_double (const double *arreglo, int n_arreglo);

int    min_int    (const int    *arreglo, int n_arreglo);
float  min_float  (const float  *arreglo, int n_arreglo);
double min_double (const double *arreglo, int n_arreglo);

int    total_int    (const int    *data, int n_data);
float  total_float  (const float  *data, int n_data);
double total_double (const double *data, int n_data);

int    count_different_usi(unsigned short int region[], int n_array_in);
int    count_different_int(               int region[], int n_array_in);

int    get_unique_usi(unsigned short int region[], int n_array_in);
int    get_unique_int(               int region[], int n_array_in);

int   *histogram_int(const int *data, int n_data, int *n_bins);
int   *histogram_int_index(const int *data, int n_data);

int    compare_double(const void *v1, const void *v2);
int    compare_float (const void *v1, const void *v2);

int   *sort_index_float (float *array, int n_array);
int   *sort_index_double(double *array, int n_array);

char *dilate_int(const int *data, int grid_X, int grid_Y, int grid_Z);
char  *erode_int(const int *data, int grid_X, int grid_Y, int grid_Z);


#endif
