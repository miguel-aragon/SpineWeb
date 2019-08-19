/*





*/
#include <stdlib.h>
#include <string.h>
#include <math.h>


int *where_float(float compare, char *option, float *arreglo, int *n_arreglo);
int *where_char(char compare, char *option, char *arreglo, int *n_arreglo);
int *where_usint(unsigned short int compare, char *option, unsigned short int *arreglo, int *n_arreglo);

float max_float( float  *arreglo, int n_arreglo);
float max_double(double *arreglo, int n_arreglo);
int   max_int(   int    *arreglo, int n_arreglo);
float min_float( float  *arreglo, int n_arreglo);
float min_double(double *arreglo, int n_arreglo);
int   min_int(   int    *arreglo, int n_arreglo);

int   count_different(unsigned short int region[]);

int *histogram_int(int *data, int n_data, int *n_bins);
int *histogram_int_index(int *data, int n_data);

