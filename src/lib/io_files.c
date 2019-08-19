#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "header_struct.h"
#include "io_files.h"

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



//==========================================
//   READ CHAR VOLUME [1]
//==========================================
char *Read_DataCube_char(char *fname, struct Header *Header_in)
{
  int  i;
  FILE *file_pointer;
  char *DataCube;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"r"))) 
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Reading file %s ...\n", fname);fflush(stdout);
  dump = fread(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Allocate memory for DataCube
  if ((DataCube = (char *) malloc(sizeof(char)*Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed. Exit program... \n\n");
      exit (1);
    }

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fread(DataCube,sizeof(char),1, file_pointer);
    DataCube++;
  }
  //--- Rebin pointer to original position
  DataCube = DataCube - Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ;

  //--- Close file
  printf("    Finish reading file \n");fflush(stdout);
  fclose(file_pointer);

  return DataCube;
}

//==========================================
//   READ UNSIGNED SHORT INT VOLUME [4]
//==========================================
unsigned short int *Read_DataCube_usi(char *fname, struct Header *Header_in)
{
  int      i;
  FILE     *file_pointer;
  unsigned short int  *DataCube;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"r"))) 
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Reading file %s ...\n", fname);fflush(stdout);
  dump = fread(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Allocate memory for DataCube
  if ((DataCube = (unsigned short int *) malloc(sizeof(unsigned short int)*Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed. Exit program... \n\n");
      exit (1);
    }

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fread(DataCube,sizeof(unsigned short int),1, file_pointer);
    DataCube++;
  }
  //--- Rebin pointer to original position
  DataCube = DataCube - Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ;

  //--- Close file
  printf("    Finish reading file \n");fflush(stdout);
  fclose(file_pointer);

  return DataCube;
}

//==========================================
//   READ INT VOLUME [5]
//==========================================
int *Read_DataCube_int(char *fname, struct Header *Header_in)
{
  int  i;
  FILE *file_pointer;
  int  *DataCube;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"r"))) 
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Reading file %s ...\n", fname);fflush(stdout);
  dump = fread(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Allocate memory for DataCube
  if ((DataCube = (int *) malloc(sizeof(int)*Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed. Exit program... \n\n");
      exit (1);
    }

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fread(DataCube,sizeof(int),1, file_pointer);
    DataCube++;
  }
  //--- Rebin pointer to original position
  DataCube = DataCube - Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ;

  //--- Close file
  printf("    Finish reading file \n");fflush(stdout);
  fclose(file_pointer);

  return DataCube;
}


//==========================================
//   READ FLOAT VOLUME [8]
//==========================================
float *Read_DataCube_float(char *fname, struct Header *Header_in)
{
  int   i;
  FILE  *file_pointer;
  float *DataCube;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"r"))) 
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Reading file %s ...\n", fname);fflush(stdout);
  dump = fread(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Allocate memory for DataCube
  if ((DataCube = (float *) malloc(sizeof(float)*Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed. Exit program... \n\n");
      exit (1);
    }

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fread(DataCube,sizeof(float),1, file_pointer);
    DataCube++;
  }
  //--- Rebin pointer to original position
  DataCube = DataCube - Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ;

  //--- Close file
  printf("    Finish reading file. \n");fflush(stdout);
  fclose(file_pointer);

  return DataCube;
}

//==========================================
//   READ DOUBLE VOLUME [9]
//==========================================
double *Read_DataCube_double(char *fname, struct Header *Header_in)
{
  int    i;
  FILE   *file_pointer;
  double *DataCube;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"r"))) 
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Reading file %s ...\n", fname);fflush(stdout);
  dump = fread(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Allocate memory for DataCube
  if ((DataCube = (double *) malloc(sizeof(double)*Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ )) == NULL) 
    {
      printf(">>> ERROR: malloc failed. Exit program... \n\n");
      exit (1);
    }

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fread(DataCube,sizeof(double),1, file_pointer);
    DataCube++;
  }
  //--- Rebin pointer to original position
  DataCube = DataCube - Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ;

  //--- Close file
  printf("    Finish reading file. \n");fflush(stdout);
  fclose(file_pointer);

  return DataCube;
}



//====================================================================================
//
//                               WRITE FUNCTIONS
//
//====================================================================================

//==========================================
//   WRITE BYTE VOLUME [1]
//==========================================
void Write_DataCube_char(char *fname, struct Header *Header_in, char *Data)
{
  int  i;
  FILE *file_pointer;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w")))
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Writting to file %s ...\n", fname);fflush(stdout);
  dump = fwrite(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fwrite(Data,sizeof(char),1, file_pointer);
    Data++;
  }

  //--- Close file
  printf("    Finish writting file \n");fflush(stdout);
  fclose(file_pointer);
}


//==========================================
//   WRITE UNSIGNED SHORT INT VOLUME [4]
//==========================================
void Write_DataCube_usi(char *fname, struct Header *Header_in, unsigned short int *Data)
{
  int  i;
  FILE *file_pointer;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w")))
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Writting to file %s ...\n", fname);fflush(stdout);
  dump = fwrite(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fwrite(Data,sizeof(unsigned short int),1, file_pointer);
    Data++;
  }

  //--- Close file
  printf("    Finish writting file \n");fflush(stdout);
  fclose(file_pointer);
}

//==========================================
//   WRITE INT VOLUME [5]
//==========================================
void Write_DataCube_int(char *fname, struct Header *Header_in, int *Data)
{
  int   i;
  FILE *file_pointer;
  int   n_pixels = Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w")))
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Writting to file %s ...\n", fname);fflush(stdout);
  dump = fwrite(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Read the data from cube
  for (i=0; i<n_pixels; i++) {
    dump = fwrite(Data,sizeof(int),1, file_pointer);
    Data++;
  }

  //--- Close file
  printf("    Finish writting file \n");fflush(stdout);
  fclose(file_pointer);
}


//==========================================
//   WRITE FLOAT VOLUME [8]
//==========================================
void Write_DataCube_float(char *fname, struct Header *Header_in, float *Data)
{
  int  i;
  FILE *file_pointer;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w")))
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Writting to file %s ...\n", fname);fflush(stdout);
  dump = fwrite(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fwrite(Data,sizeof(float),1, file_pointer);
    Data++;
  }

  //--- Close file
  printf("    Finish writting file \n");fflush(stdout);
  fclose(file_pointer);
}


//==========================================
//   WRITE DOUBLE VOLUME [9]
//==========================================
void Write_DataCube_double(char *fname, struct Header *Header_in, double *Data)
{
  int  i;
  FILE *file_pointer;
  size_t dump;

  //--- Attempt to open file, exit otherwise
  if(!(file_pointer=fopen(fname,"w")))
    { 
      printf(">>> Can't open file '%s'. Exit program...\n",fname); 
      exit(1); 
    }

  //--- Read header of file.
  printf(">>> Writting to file %s ...\n", fname);fflush(stdout);
  dump = fwrite(Header_in, sizeof(*Header_in), 1, file_pointer);

  //--- Read the data from cube
  for (i=0; i<Header_in->sizeX*Header_in->sizeY*Header_in->sizeZ; i++) {
    dump = fwrite(Data,sizeof(double),1, file_pointer);
    Data++;
  }

  //--- Close file
  printf("    Finish writting file \n");fflush(stdout);
  fclose(file_pointer);
}
