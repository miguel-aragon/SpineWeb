#ifndef HEADER_WATER_FUNCTIONS_UPHILL_H
#define HEADER_WATER_FUNCTIONS_UPHILL_H
/*



*/

int  *watershed_uphill_27(double *density, int x_grid, int y_grid, int z_grid);
int  *watershed_uphill_27_float(float *density, int x_grid, int y_grid, int z_grid);

void flood_basin_uphill(int *closest_minima, int *voids, int x_grid, int y_grid, int z_grid, int total_minima, int *local_minima);
void recursive_flood_uphill(int *children, int *closest_minima, int *voids, int ID, int x_grid, int y_grid, int z_grid);



#endif
