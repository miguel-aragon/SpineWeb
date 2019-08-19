/*


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pixel2.h"


//==========================================
//   DECOMPOSE INDEX
//==========================================
void decomposed_index2d(int *xy, int Ind0, float xd_grid, float yd_grid){
  double temp, ind_d;
  int    dummy;

  //--- Double version of index
  ind_d = (float) Ind0;

  //--- The coordinades run as Z -> Y -> X
  //--- X
  if(ind_d == 0)   { xy[0] = 0; } //--- Indetermined 0/0
  else             { xy[0] = (int) (ind_d/yd_grid); }

  //--- Y
  xy[1] = (int) (ind_d - xy[0] * yd_grid);
}


//==========================================
//   SHELL AROUND PIXEL, TAKE ONLY 6 NEIGHBOURS
//==========================================
void shell_09(int *shell_ind, int nx, int ny, int x_grid, int y_grid){
  int x00,x01,x02,x03,x04,x05,x06,x07,x08;
  int y00,y01,y02,y03,y04,y05,y06,y07,y08;
  
  //  X            Y 
  x00 = nx;    y00 = ny;
  x01 = nx-1;  y01 = ny+1;
  x02 = nx;    y02 = ny+1;
  x03 = nx+1;  y03 = ny+1;
  x04 = nx-1;  y04 = ny;
  x05 = nx+1;  y05 = ny;
  x06 = nx-1;  y06 = ny-1;
  x07 = nx;    y07 = ny-1;
  x08 = nx+1;  y08 = ny-1;  
  
  //--- Periodic conditions
  if(nx == 0) {
    x01 = x_grid-1;
    x04 = x_grid-1;
    x06 = x_grid-1;
  }
  if(nx == x_grid-1) {
    x03 = 0;
    x05 = 0;
    x08 = 0;
  }
  if(ny == 0){
    y06 = y_grid-1;
    y07 = y_grid-1;
    y08 = y_grid-1;
  }
  if(ny == y_grid-1){
    y01  = 0;
    y02  = 0;
    y03  = 0;
  }

  //--- Evaluate the shell_ind. The order variation of
  //    the indexes go as Z -> Y -> X
  shell_ind[0]  = y00 + x00*y_grid;
  shell_ind[1]  = y01 + x01*y_grid;
  shell_ind[2]  = y02 + x02*y_grid;
  shell_ind[3]  = y03 + x03*y_grid;
  shell_ind[4]  = y04 + x04*y_grid;
  shell_ind[5]  = y05 + x05*y_grid;
  shell_ind[6]  = y06 + x06*y_grid;
  shell_ind[7]  = y07 + x07*y_grid;
  shell_ind[8]  = y08 + x08*y_grid;
}

