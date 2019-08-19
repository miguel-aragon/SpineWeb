#ifndef FIND_MINIMA_H
#define FIND_MINIMA_H
/*




*/


//--- 3D
int *find_minima_int_27(double *density, int x_grid, int y_grid, int z_grid, int *total_minima);
int *find_minima_int_07(double *density, int x_grid, int y_grid, int z_grid, int *total_minima);

int *find_minima_int_27_float(float *density, int x_grid, int y_grid, int z_grid, int *total_minima);
int *find_minima_int_07_float(float *density, int x_grid, int y_grid, int z_grid, int *total_minima);

//--- 2D
int *find_minima_int_09_float(float *density, int x_grid, int y_grid, int *total_minima);


#endif
