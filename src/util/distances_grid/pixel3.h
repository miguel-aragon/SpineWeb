/*



*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void decomposed_index3d(int *xyz, int Ind0, double xd_grid,double yd_grid,double zd_grid);

void shell(int *shell_ind, int nx, int ny, int nz, int x_grid, int y_grid, int z_grid);

void shell_sph(int *shell_ind, int nx, int ny, int nz, int x_grid, int y_grid, int z_grid);
