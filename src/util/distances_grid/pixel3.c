/*


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "pixel3.h"


//==========================================
//   DECOMPOSE INDEX
//==========================================
void decomposed_index3d(int *xyz, int Ind0, double xd_grid,double yd_grid,double zd_grid){
  double temp, ind_d;
  int    dummy;

  //--- Double version of index
  ind_d = (double) Ind0;

  //--- The coordinades run as Z -> Y -> X
  //--- X
  if(ind_d == 0)   { xyz[0] = 0; } //--- Indetermined 0/0
  else             { xyz[0] = (int) (ind_d/yd_grid/zd_grid); }
                     temp   = (int) (ind_d - xyz[0] * (yd_grid*zd_grid)); 
  //--- Y
  if(zd_grid == 0) { xyz[1] = 0; } //--- Indetermined 0/0
  else             { xyz[1] = (int) (temp/zd_grid); }
  //--- Z
                     xyz[2]   = (int) (temp - xyz[1] * zd_grid);
}


//==========================================
//   SHELL (RECTANGLE)
//==========================================
void shell(int *shell_ind, int nx, int ny, int nz, int x_grid, int y_grid, int z_grid){
  int x00,x01,x02,x03,x04,x05,x06,x07,x08,x09;
  int x10,x11,x12,x13,x14,x15,x16,x17,x18,x19;
  int x20,x21,x22,x23,x24,x25,x26;
  int y00,y01,y02,y03,y04,y05,y06,y07,y08,y09;
  int y10,y11,y12,y13,y14,y15,y16,y17,y18,y19;
  int y20,y21,y22,y23,y24,y25,y26;
  int z00,z01,z02,z03,z04,z05,z06,z07,z08,z09;
  int z10,z11,z12,z13,z14,z15,z16,z17,z18,z19;
  int z20,z21,z22,z23,z24,z25,z26;
  
  //  X            Y            Z
  x00 = nx;    y00 = ny;    z00 = nz;
  x01 = nx-1;  y01 = ny-1;  z01 = nz-1;
  x02 = nx;    y02 = ny-1;  z02 = nz-1;
  x03 = nx+1;  y03 = ny-1;  z03 = nz-1;
  x04 = nx-1;  y04 = ny;    z04 = nz-1;
  x05 = nx;    y05 = ny;    z05 = nz-1;
  x06 = nx+1;  y06 = ny;    z06 = nz-1;
  x07 = nx-1;  y07 = ny+1;  z07 = nz-1;
  x08 = nx;    y08 = ny+1;  z08 = nz-1;
  x09 = nx+1;  y09 = ny+1;  z09 = nz-1;
  x10 = nx-1;  y10 = ny-1;  z10 = nz;
  x11 = nx;    y11 = ny-1;  z11 = nz;
  x12 = nx+1;  y12 = ny-1;  z12 = nz;
  x13 = nx-1;  y13 = ny;    z13 = nz;
  x14 = nx+1;  y14 = ny;    z14 = nz;
  x15 = nx-1;  y15 = ny+1;  z15 = nz;
  x16 = nx;    y16 = ny+1;  z16 = nz;
  x17 = nx+1;  y17 = ny+1;  z17 = nz;
  x18 = nx-1;  y18 = ny-1;  z18 = nz+1;
  x19 = nx;    y19 = ny-1;  z19 = nz+1;
  x20 = nx+1;  y20 = ny-1;  z20 = nz+1;
  x21 = nx-1;  y21 = ny;    z21 = nz+1;
  x22 = nx;    y22 = ny;    z22 = nz+1;
  x23 = nx+1;  y23 = ny;    z23 = nz+1;
  x24 = nx-1;  y24 = ny+1;  z24 = nz+1;
  x25 = nx;    y25 = ny+1;  z25 = nz+1;
  x26 = nx+1;  y26 = ny+1;  z26 = nz+1;


  //--- Periodic conditions
  if(nx == 0){
    x01 = x_grid-1;
    x04 = x_grid-1;
    x07 = x_grid-1;
    x10 = x_grid-1;
    x13 = x_grid-1;
    x15 = x_grid-1;
    x18 = x_grid-1;
    x21 = x_grid-1;
    x24 = x_grid-1;}
  
  if(nx == x_grid-1){
    x03 = 0;
    x06 = 0;
    x09 = 0;
    x12 = 0;
    x14 = 0;
    x17 = 0;
    x20 = 0;
    x23 = 0;
    x26 = 0;}
  
  if(ny == 0){
    y01 = y_grid-1;
    y02 = y_grid-1;
    y03 = y_grid-1;
    y10 = y_grid-1;
    y11 = y_grid-1;
    y12 = y_grid-1;
    y18 = y_grid-1;
    y19 = y_grid-1;
    y20 = y_grid-1;}
  
  if(ny == y_grid-1){
    y07  = 0;
    y08  = 0;
    y09  = 0;
    y15  = 0;
    y16  = 0;
    y17  = 0;
    y24  = 0;
    y25  = 0;
    y26  = 0;}

  if(nz == 0){
    z01 = z_grid-1;
    z02 = z_grid-1;
    z03 = z_grid-1;
    z04 = z_grid-1;
    z05 = z_grid-1;
    z06 = z_grid-1;
    z07 = z_grid-1;
    z08 = z_grid-1;
    z09 = z_grid-1;}

  if(nz == z_grid-1){
    z18 = 0;
    z19 = 0;
    z20 = 0;
    z21 = 0;
    z22 = 0;
    z23 = 0;
    z24 = 0;
    z25 = 0;
    z26 = 0;}

  //--- Evaluate the shell_ind. The order variation of
  //    the indexes go as Z -> Y -> X
  shell_ind[0]  = z00 + y00*z_grid + x00*y_grid*z_grid;
  shell_ind[1]  = z01 + y01*z_grid + x01*y_grid*z_grid;
  shell_ind[2]  = z02 + y02*z_grid + x02*y_grid*z_grid;
  shell_ind[3]  = z03 + y03*z_grid + x03*y_grid*z_grid;
  shell_ind[4]  = z04 + y04*z_grid + x04*y_grid*z_grid;
  shell_ind[5]  = z05 + y05*z_grid + x05*y_grid*z_grid;
  shell_ind[6]  = z06 + y06*z_grid + x06*y_grid*z_grid;
  shell_ind[7]  = z07 + y07*z_grid + x07*y_grid*z_grid;
  shell_ind[8]  = z08 + y08*z_grid + x08*y_grid*z_grid;
  shell_ind[9]  = z09 + y09*z_grid + x09*y_grid*z_grid;
  shell_ind[10] = z10 + y10*z_grid + x10*y_grid*z_grid;
  shell_ind[11] = z11 + y11*z_grid + x11*y_grid*z_grid;
  shell_ind[12] = z12 + y12*z_grid + x12*y_grid*z_grid;
  shell_ind[13] = z13 + y13*z_grid + x13*y_grid*z_grid;
  shell_ind[14] = z14 + y14*z_grid + x14*y_grid*z_grid;
  shell_ind[15] = z15 + y15*z_grid + x15*y_grid*z_grid;
  shell_ind[16] = z16 + y16*z_grid + x16*y_grid*z_grid;
  shell_ind[17] = z17 + y17*z_grid + x17*y_grid*z_grid;
  shell_ind[18] = z18 + y18*z_grid + x18*y_grid*z_grid;
  shell_ind[19] = z19 + y19*z_grid + x19*y_grid*z_grid;
  shell_ind[20] = z20 + y20*z_grid + x20*y_grid*z_grid;
  shell_ind[21] = z21 + y21*z_grid + x21*y_grid*z_grid;
  shell_ind[22] = z22 + y22*z_grid + x22*y_grid*z_grid;
  shell_ind[23] = z23 + y23*z_grid + x23*y_grid*z_grid;
  shell_ind[24] = z24 + y24*z_grid + x24*y_grid*z_grid;
  shell_ind[25] = z25 + y25*z_grid + x25*y_grid*z_grid;
  shell_ind[26] = z26 + y26*z_grid + x26*y_grid*z_grid;

}

//==========================================
//   SHELL (SEMI-SPHERE)
//==========================================
void shell_sph(int *shell_ind, int nx, int ny, int nz, int x_grid, int y_grid, int z_grid){
  int x00,x02,x04,x05,x06,x08;
  int x10,x11,x12,x13,x14,x15,x16,x17,x19;
  int x21,x22,x23,x25;
  int y00,y02,y04,y05,y06,y08;
  int y10,y11,y12,y13,y14,y15,y16,y17,y19;
  int y21,y22,y23,y25;
  int z00,z02,z04,z05,z06,z08;
  int z10,z11,z12,z13,z14,z15,z16,z17,z19;
  int z21,z22,z23,z25;
  
  //  X            Y            Z
  x00 = nx;    y00 = ny;    z00 = nz;
  //x01 = nx-1;  y01 = ny-1;  z01 = nz-1; //
  x02 = nx;    y02 = ny-1;  z02 = nz-1;
  //x03 = nx+1;  y03 = ny-1;  z03 = nz-1; //
  x04 = nx-1;  y04 = ny;    z04 = nz-1;
  x05 = nx;    y05 = ny;    z05 = nz-1;
  x06 = nx+1;  y06 = ny;    z06 = nz-1;
  //x07 = nx-1;  y07 = ny+1;  z07 = nz-1; //
  x08 = nx;    y08 = ny+1;  z08 = nz-1;
  //x09 = nx+1;  y09 = ny+1;  z09 = nz-1; //
  x10 = nx-1;  y10 = ny-1;  z10 = nz;
  x11 = nx;    y11 = ny-1;  z11 = nz;
  x12 = nx+1;  y12 = ny-1;  z12 = nz;
  x13 = nx-1;  y13 = ny;    z13 = nz;
  x14 = nx+1;  y14 = ny;    z14 = nz;
  x15 = nx-1;  y15 = ny+1;  z15 = nz;
  x16 = nx;    y16 = ny+1;  z16 = nz;
  x17 = nx+1;  y17 = ny+1;  z17 = nz;
  //x18 = nx-1;  y18 = ny-1;  z18 = nz+1; //
  x19 = nx;    y19 = ny-1;  z19 = nz+1;
  //x20 = nx+1;  y20 = ny-1;  z20 = nz+1; //
  x21 = nx-1;  y21 = ny;    z21 = nz+1;
  x22 = nx;    y22 = ny;    z22 = nz+1;
  x23 = nx+1;  y23 = ny;    z23 = nz+1;
  //x24 = nx-1;  y24 = ny+1;  z24 = nz+1; //
  x25 = nx;    y25 = ny+1;  z25 = nz+1;
  //x26 = nx+1;  y26 = ny+1;  z26 = nz+1; //


  //--- Periodic conditions
  if(nx == 0){
    x04 = x_grid-1;
    x10 = x_grid-1;
    x13 = x_grid-1;
    x15 = x_grid-1;
    x21 = x_grid-1;}
  
  if(nx == x_grid-1){
    x06 = 0;
    x12 = 0;
    x14 = 0;
    x17 = 0;
    x23 = 0;}
  
  if(ny == 0){
    y02 = y_grid-1;
    y10 = y_grid-1;
    y11 = y_grid-1;
    y12 = y_grid-1;
    y19 = y_grid-1;}
  
  if(ny == y_grid-1){
    y08  = 0;
    y15  = 0;
    y16  = 0;
    y17  = 0;
    y25  = 0;}

  if(nz == 0){
    z02 = z_grid-1;
    z04 = z_grid-1;
    z05 = z_grid-1;
    z06 = z_grid-1;
    z08 = z_grid-1;}

  if(nz == z_grid-1){
    z19 = 0;
    z21 = 0;
    z22 = 0;
    z23 = 0;
    z25 = 0;}

  //--- Evaluate the shell_ind. The order variation of
  //    the indexes go as Z -> Y -> X
  shell_ind[0]  = z00 + y00*z_grid + x00*y_grid*z_grid;
  shell_ind[1]  = z02 + y02*z_grid + x02*y_grid*z_grid;
  shell_ind[2]  = z04 + y04*z_grid + x04*y_grid*z_grid;
  shell_ind[3]  = z05 + y05*z_grid + x05*y_grid*z_grid;
  shell_ind[4]  = z06 + y06*z_grid + x06*y_grid*z_grid;
  shell_ind[5]  = z08 + y08*z_grid + x08*y_grid*z_grid;
  shell_ind[6]  = z10 + y10*z_grid + x10*y_grid*z_grid;
  shell_ind[7]  = z11 + y11*z_grid + x11*y_grid*z_grid;
  shell_ind[8]  = z12 + y12*z_grid + x12*y_grid*z_grid;
  shell_ind[9]  = z13 + y13*z_grid + x13*y_grid*z_grid;
  shell_ind[10] = z14 + y14*z_grid + x14*y_grid*z_grid;
  shell_ind[11] = z15 + y15*z_grid + x15*y_grid*z_grid;
  shell_ind[12] = z16 + y16*z_grid + x16*y_grid*z_grid;
  shell_ind[13] = z17 + y17*z_grid + x17*y_grid*z_grid;
  shell_ind[14] = z19 + y19*z_grid + x19*y_grid*z_grid;
  shell_ind[15] = z21 + y21*z_grid + x21*y_grid*z_grid;
  shell_ind[16] = z22 + y22*z_grid + x22*y_grid*z_grid;
  shell_ind[17] = z23 + y23*z_grid + x23*y_grid*z_grid;
  shell_ind[18] = z25 + y25*z_grid + x25*y_grid*z_grid;

  //shell_ind[1]  = z01 + y01*z_grid + x01*y_grid*z_grid;
  //shell_ind[3]  = z03 + y03*z_grid + x03*y_grid*z_grid;
  //shell_ind[7]  = z07 + y07*z_grid + x07*y_grid*z_grid;
  //shell_ind[9]  = z09 + y09*z_grid + x09*y_grid*z_grid;
  //shell_ind[18] = z18 + y18*z_grid + x18*y_grid*z_grid;
  //shell_ind[20] = z20 + y20*z_grid + x20*y_grid*z_grid;
  //shell_ind[24] = z24 + y24*z_grid + x24*y_grid*z_grid;
  //shell_ind[26] = z26 + y26*z_grid + x26*y_grid*z_grid;

}

