#ifndef IO_FILES_H
#define IO_FILES_H
/*



*/

/*

See "header_struct.h" for details on the header structure. Basically all the 
information on the datafile is stored in a 256 byte record. Here the important
thing to consider is the datatype given as:

Dataype  Type               Bytes  Bits                Range
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

*/


//--- Read functions
char               *Read_DataCube_char  (char *fname, struct Header *Header_in);
unsigned short int *Read_DataCube_usi   (char *fname, struct Header *Header_in);
int                *Read_DataCube_int   (char *fname, struct Header *Header_in);
float              *Read_DataCube_float (char *fname, struct Header *Header_in);
double             *Read_DataCube_double(char *fname, struct Header *Header_in);

//--- Write functions
void               Write_DataCube_char  (char *fname, struct Header *Header_in, char               *Data);
void               Write_DataCube_usi   (char *fname, struct Header *Header_in, unsigned short int *Data);
void               Write_DataCube_int   (char *fname, struct Header *Header_in, int                *Data);
void               Write_DataCube_float (char *fname, struct Header *Header_in, float              *Data);
void               Write_DataCube_double(char *fname, struct Header *Header_in, double             *Data);


#endif
