#ifndef HEADER_STRUCT_H
#define HEADER_STRUCT_H
/*


dataype  Type               Bytes  Bits                Range
----------------------------------------------------------------------------
0        bit                 1/8       1                0 -> 1
1        signed char          1        8             -128 -> +127
2        unsigned char        1        8                0 -> +255
3        short int            2       16          -32,768 -> +32,767          
4        unsigned short int   2       16                0 -> +65,535          
5        int                  4       32   -2,147,483,648 -> +2,147,483,647
6        unsigned int         4       32                0 -> +4,294,967,295
7        long int             4       32   -2,147,483,648 -> +2,147,483,647
8        float                4       32       3.4 E-38   -> 3.4 E+38 
9        double               8       64       1.7 E-308  -> 1.7 E+308
10       long double          12      96       3.4 E-4932 -> 3.4 E+4932 



gridsize   Description
----------------------------------
1          Regular 1D grid [N]
2          Regular 2D grid [N,M]
3          Regular 3D grid [N,M,O]

*/




//--- Define header structure
struct Header{
  //--- Type of data
  int   datatype,gridtype;
  //--- Grid size
  int   sizeG,sizeX,sizeY,sizeZ;  
  //--- Offset of box
  float offsX,offsY,offsZ;
  //--- Total number of pixels
  char  remaining[256-36];
};


#endif
