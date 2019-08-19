#ifndef HEADER_PIXEL3_H
#define HEADER_PIXEL3_H


/*



*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void decomposed_index3d(int *xyz, int Ind0, double xd_grid,double yd_grid,double zd_grid);

void shell_27(int *shell_ind, int nx, int ny, int nz, int x_grid, int y_grid, int z_grid);
void shell_07(int *shell_ind, int nx, int ny, int nz, int x_grid, int y_grid, int z_grid);

#endif
