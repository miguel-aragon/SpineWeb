/*

   DESCRIPTION:
    Computes the shortest distance from a set of points to another set of sampling points.


   MEMORY USAGE:
    Total memory needed +- extra stuff:
    3 x imagesize^3 
    5 x NPart

   USAGE:
    The only tricky part is the "imagesize[Indexed array]" option. 
    In order to speed-up the computation I generate a grid and put the
    particles there. The grid size should be as large as possible
    (not restricted to powers of two). 

   EXAMPLE:

     __________________________________________
    |                                          |
    |  Written by Miguel Angel Aragon Calvo.   |
    |  miguel@pha.jhu.edu                      |
    |__________________________________________|


    Compile as:
       gcc  -O3 distances_euclidean.c -o distances_euclidean -lm -lpthread -D_REENTRANT


    History:
       -Created:  11/June/2009
 
      -Modifications:


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>


//===============================================================
//--- Global variables
//===============================================================
long   *ngp_n, *ngp_o, *ngp_i, *IDs;
//--- Temporal variables
double TempD;
float  TempF;


struct particle_data {
  float  Pos[3];
} *P, *P2;     //--- Pointer to structure

//--- Initial and final counters
struct Counter_range {
  int ini;
  int fin;
} *Range;

struct header_struct {
  //---  General info
  int    N_Data;             //--- Number  of data     points
  int    N_Sampling;         //--- Number  of sampling points
  double Boxsize_Data;       //--- Boxsize of data     points
  double Boxsize_Samplings;  //--- Boxsize of sampling points
  int    GridSize;           //--- Grid used for indexed array
  //---  Files
  char   file_input   [256];
  char   file_output  [256];
  char   file_sampling[256];
  //---  Needed for threads
  double *distance;          //--- Pointer to data arrays
  int    i_ini, i_fin;       //--- Initial and final counters inside each thread
  int    N_Threads;
} header_info;     //--- Pointer to structure


//===============================================================
//================   FUNCTIONS DECLARATION  =====================
//===============================================================

struct particle_data *read_particles(char *fname, int *N_part, double *Boxsize);
void    check_non_empty_cells(long *ngp_n, long GridSize);
void    create_indexed_array(int GridSize, double BoxSize, int N_part);
void   *get_distances(void *Range);
void    save_density(char *fname, int N_part, double *density);
long   *lvector(long nh);
float  *vector(long nh);
double *dvector(long nh);
long   *shell(long *shell_ind, long nx, long ny, long nz, long n_grid);
void    read_command_line(int argc, char **argv, struct header_struct *header_info);


//==============================================
//================   MAIN  =====================
//==============================================
int main(int argc, char **argv)
{

  int       i,j,k;
  double    *distance;
  pthread_t thread_id[header_info.N_Threads];
  int       i_med; //--- Helper variable to divide work in thread
  
  //--- Timer
  time_t t1,t2;
  (void) time(&t1);



  //------------------------------
  //--- Read command line options
  //------------------------------
  read_command_line(argc, argv, &header_info);

  //------------------------------
  //--- Here we read the spine and convert it to xyz positions to be returned to PP
  //------------------------------
  //P = read_positions_from_spine(header_info.file_input, &header_info.N_Data, &header_info.Boxsize_Data);  
  P  = read_particles(header_info.file_input, &header_info.N_Data, &header_info.Boxsize_Data);  

  //------------------------------
  //--- Read sampling points from file
  //------------------------------
  printf("Reading sampling points...\n"); fflush(stdout);
  P2 = read_particles(header_info.file_sampling, &header_info.N_Sampling, &header_info.Boxsize_Samplings);

  //------------------------------
  //--- Allocate memory for density
  //------------------------------
  printf("Allocating %d for sampling distances\n",header_info.N_Sampling); fflush(stdout);
  distance = dvector(header_info.N_Sampling);

  //------------------------------
  //--- Create indexed array
  //------------------------------
  create_indexed_array(header_info.GridSize, header_info.Boxsize_Data, header_info.N_Data);

  //------------------------------
  //--- Check that there are non empty cells in grid
  //------------------------------
  printf("Checking non empty cells...\n"); fflush(stdout);
  check_non_empty_cells(ngp_n, header_info.GridSize);

  //------------------------------
  //--- Compute densities
  //------------------------------
  header_info.distance   = distance;
  
  //--- Allocate memory for range
  Range = (struct Counter_range *) malloc(sizeof (struct Counter_range) * header_info.N_Threads); 

  //--- Start threads
  printf("\n\n");
  printf("|=============================================================|\n");
  printf(" ");
  for(i=0; i < header_info.N_Threads; i++) {
    i_med = (int) header_info.N_Sampling/((float) header_info.N_Threads);

    Range[i].ini = i*i_med;
    Range[i].fin = (i+1)*i_med;

    printf("\n>>> Thread [%d] in range: [%d - %d] \n", i+1, i*i_med, (i+1)*i_med);
    //--- Make sure the last thread reaches the last particle
    if (i == (header_info.N_Threads-1) ) Range[i].fin = header_info.N_Sampling;

    //--- Call thread
    if ( (pthread_create( &thread_id[i], NULL, get_distances, (void *) &Range[i] )) ) {
      printf(">>> Error creating thread!!!\n");
      exit(0);
    } else {
      printf("\n>>> Thread [%d] created!\n", i+1);
    }

  }//--- end for
  
  //--- Join threads
  for(j=0; j < header_info.N_Threads; j++) {
    pthread_join( thread_id[j], NULL); 
  }

  free(Range);

  //------------------------------
  //--- Save densities to file
  //------------------------------
  save_density(header_info.file_output, header_info.N_Sampling, distance);

  
  //--- Check time and print
  (void) time(&t2);
  printf("\nComputing time = %d seconds\n", (int) (t2-t1));

  
  return(0);

}


//==================================================================
//           FUNCTIONS
//==================================================================

//========================================
//--- 
//========================================
void read_command_line(int argc, char **argv, struct header_struct *header_info){

  int i;

  //-------------------------------
  //--- Default options
  //-------------------------------
  header_info->N_Threads = 1;

  //-------------------------------
  //--- Read command line options
  //-------------------------------
  printf("\n-------------------------------------------------\n");
  for (i = 1; i < argc; i++) { 
    //--- OUTPUT FILE
    if (strcmp( "-o", argv[i]) == 0){
      sscanf(argv[i+1],"%s",&header_info->file_output);
      printf(">>>     Output file = %s \n", header_info->file_output);
    }
    //--- INPUT POSITIONS
    if (strcmp( "-i", argv[i]) == 0){
      sscanf(argv[i+1],"%s",&header_info->file_input);
      printf(">>>     Input file = %s \n", header_info->file_input);
    }
    //--- READ GRID SIZE FOR INDEXING
    else if (strcmp( "-g", argv[i]) == 0){
      sscanf(argv[i+1],"%d",&header_info->GridSize);
      printf(">>>     Grid   size = %d\n", header_info->GridSize);
    }
    //--- SAMPLING FILE
    else if (strcmp( "-s", argv[i]) == 0){
      sscanf(argv[i+1],"%s",&header_info->file_sampling);  
      printf(">>>     Sampling file: '%s'\n",header_info->file_sampling);
    }
    //--- NUMBER OF THREADS
    else if (strcmp( "-T", argv[i]) == 0){
      sscanf(argv[i+1],"%i",&header_info->N_Threads);
      printf(">>>     Running on %d threads\n",header_info->N_Threads);
    }
  }

  //-------------------------------
  //--- Some extra info
  //-------------------------------
  if (header_info->N_Threads == 1) printf(">>>     Running default mode: 1 thread\n");


  //--- Running info
  if (argc < 2) {
    printf("Usage: \n");
    printf("       ./distances_euclidean\n");
    printf("                                -i [Input file    (.CGAL filament or wall 'particles')]    \n");
    printf("                                -s [Sampling file (.CGAL halos or galaxies)] \n");
    printf("                                -o [Output file]   \n\n");
    printf("                                -g [Grid size]     \n");
    printf("                                -T [Number of threads, the same as processors] \n\n");
    exit(0);
  }

  printf("-------------------------------------------------\n\n");

}

//========================================
//--- 
//========================================
struct particle_data *read_particles(char *fname, int *N_part, double *BoxSize)
{
  struct particle_data *Part = NULL;             //--- Pointer to structure
  FILE *fd;
  char buf[200];
  int  i,j,k,dummy,ntot_withmasses;
  int  t,n,off,pc,pc_new,pc_sph;
  int  count_err=0;
  size_t dump;

  sprintf(buf,"%s",fname);
  
  if(!(fd=fopen(buf,"r"))){
    printf("can't open file '%s'\n",buf);
    exit(0);}
  
  //--- Read number of particles
  dump = fread(&TempF, sizeof(float), 1, fd);
  dump = fread(N_part, sizeof(int), 1, fd);
  *BoxSize = (double) TempF;
  printf("Reading %d particles inside Box of side %f from file '%s' ...\n", *N_part, *BoxSize,  buf); fflush(stdout);

  //--- Allocate memory for particles
  printf("   Allocating memory for positions...\n"); fflush(stdout);
  Part=(struct particle_data *) malloc((size_t) ( (*N_part) * sizeof(struct particle_data)));
  if (!Part) {
    printf("Error alocating memory for particles"); fflush(stdout);
    exit(0);
  }

  //--- Read particles
  printf("   Reading positions...\n"); fflush(stdout);
  for(n=0; n< (*N_part); n++) {
    dump = fread(&Part[n].Pos[0], sizeof(float), 1, fd);
    dump = fread(&Part[n].Pos[1], sizeof(float), 1, fd);
    dump = fread(&Part[n].Pos[2], sizeof(float), 1, fd);
  }

  fclose(fd);

  //--- Fix boundaries (x >= BoxSize || x < 0)
  printf("   Checking periodic conditions...\n"); fflush(stdout);
  for(i=0; i<= (*N_part)-1; i++){
    if(Part[i].Pos[0] >= *BoxSize){ Part[i].Pos[0] = Part[i].Pos[0] - (*BoxSize); count_err++;}
    if(Part[i].Pos[1] >= *BoxSize){ Part[i].Pos[1] = Part[i].Pos[1] - (*BoxSize); count_err++;}
    if(Part[i].Pos[2] >= *BoxSize){ Part[i].Pos[2] = Part[i].Pos[2] - (*BoxSize); count_err++;}
    if(Part[i].Pos[0] <       0.0){ Part[i].Pos[0] = Part[i].Pos[0] + (*BoxSize); count_err++;}
    if(Part[i].Pos[1] <       0.0){ Part[i].Pos[1] = Part[i].Pos[1] + (*BoxSize); count_err++;}
    if(Part[i].Pos[2] <       0.0){ Part[i].Pos[2] = Part[i].Pos[2] + (*BoxSize); count_err++;}
  }
  printf("   %d particles out of box\n", count_err); fflush(stdout);

  return Part;
}


//========================================
//--- Check that there are not empty cells in grid
//========================================
void check_non_empty_cells(long *ngp_n, long GridSize){

  long   i,j,k;
  
  //--- Loop over all elements in grid
  for(i=0; i<GridSize; i++){
    for(j=0; j<GridSize; j++){
      for(k=0; k<GridSize; k++){

	//--- Avoid empty cells
	if(ngp_n[i + j*GridSize + k*GridSize*GridSize] == 0) {	  
	  printf(">>> ERROR: There are empty grids in the indexed array. Use a smaller GridSize value.\n");
	  exit(1);
	}
	
      } //--- k
    } //--- j
  }//--- i

}



//========================================
//--- Main function to compute densities
//========================================
void *get_distances(void *Range)
{
  long   i,j,k,w;
  int    cont=0;
  long   xgrid,ygrid,zgrid;
  long   Ind_Shell[27];                     //--- Array of long
  long   *shell_ind;                        //--- Pointer to long
  long   neighbour_id;
  double xi,yi,zi,xp,yp,zp;
  long   progress_bar;                      //--- Progress bar
  double dista2;
  double BoxSize,BoxSize2;                  //--- Half boxsize
  int    GridSize;
  double min_dista;                         //--- Minimin distance from spine

  //--- Handle input structure
  struct Counter_range *Range_local = (struct Counter_range *) Range; 
  int    ini, fin;   //--- ss values of Counter_range to local variables
  ini    = Range_local->ini;
  fin    = Range_local->fin;
  
  //--- Handy instead of using the complete structure
  BoxSize    = header_info.Boxsize_Data;
  GridSize   = header_info.GridSize;


  //--- Initialize some variables...
  shell_ind = Ind_Shell;
  BoxSize2  = BoxSize/2.0;
  min_dista = BoxSize*BoxSize;      //--- Assign an unlikely large min_dista

  //-----------------------------
  //--- Preamble to main loop
  //-----------------------------

  //--- Progress bar
  progress_bar = (long) floorf((fin-ini)/60.0);


  //--- Main density loop. Use different ranges for each thread
  for(i=ini; i<fin; i++){

    //--- Restart minimum distance
    min_dista = BoxSize*BoxSize; 

    //--- These are the sampling points
    xi = (double) P2[i].Pos[0];
    yi = (double) P2[i].Pos[1];
    zi = (double) P2[i].Pos[2];
    
    xgrid = ((xi/BoxSize)*GridSize);
    ygrid = ((yi/BoxSize)*GridSize);
    zgrid = ((zi/BoxSize)*GridSize);

    //--- Get shell around [xgrid,ygrid,zgrid] cell
    shell_ind = shell(shell_ind, xgrid,ygrid,zgrid, GridSize);

    //--- Loop over shell...
    for(k=0; k<27; k++){
                
      //--- Loop over particles in cell
      for(j=0; j<=ngp_n[shell_ind[k]]-1; j++){

	//--- ID of neighbour particle
	neighbour_id = ngp_i[ngp_o[shell_ind[k]]+j];
	
	//--- Distance to neighbour particle
	xp = xi - P[neighbour_id].Pos[0];
	yp = yi - P[neighbour_id].Pos[1];
	zp = zi - P[neighbour_id].Pos[2];

	//--- Periodic boundaries...
	if(xp < -BoxSize2) xp = xp + BoxSize;
	if(xp >  BoxSize2) xp = xp - BoxSize;
	if(yp < -BoxSize2) yp = yp + BoxSize;
	if(yp >  BoxSize2) yp = yp - BoxSize;
	if(zp < -BoxSize2) zp = zp + BoxSize;
	if(zp >  BoxSize2) zp = zp - BoxSize; 	
	
	//--- Squared distance from particle
	dista2 = xp*xp + yp*yp + zp*zp;

	//--- Get the smallest value
	if (dista2 < min_dista) min_dista = dista2;
      }
    }

    //--- Store density in array
    //header_info.distance[i] = pow(min_dista,0.5);
    header_info.distance[i] = min_dista;

    //--- Update progress bar...
    if(fmod(((float) cont)/((float) header_info.N_Threads),progress_bar) == 0) 
      putchar(48 + (char) ( ((float) i )/ ((float) header_info.N_Sampling )* ((float) header_info.N_Threads ))); 
    fflush(stdout);
    cont++;


  }
  printf("\n");


  return NULL;

}


//=====================================
//--- Creates the indexed array
//=====================================
void create_indexed_array(int GridSize, double BoxSize, int N_part)
{
  int  i;
  int  xgrid,ygrid,zgrid;
  int  ind;  //--- Linear index
  int  N_GridSize;

  N_GridSize = GridSize*GridSize*GridSize;

  printf("Prepare to allocate %d cells\n", N_GridSize);
  //--- Create Ids array
  IDs     = lvector(N_part);
  //--- Create 1D image
  ngp_n   = lvector(N_GridSize);
  //--- Create 1D offset
  ngp_o   = lvector(N_GridSize);
  //--- Create 1D indexed array
  ngp_i   = lvector(N_part);


  //--- IDs
  printf("Create Id's array... "); fflush(stdout);
  for(i=0; i<N_part; i++) IDs[i] = i;
  printf("done\n"); fflush(stdout);

  //--- Image
  for(i=0; i<N_GridSize; i++) ngp_n[i] = 0;
  printf("Creating image array... "); fflush(stdout);
  for(i=0; i<N_part; i++){
    xgrid = (int) ((P[i].Pos[0]/BoxSize)*GridSize);
    ygrid = (int) ((P[i].Pos[1]/BoxSize)*GridSize);
    zgrid = (int) ((P[i].Pos[2]/BoxSize)*GridSize);
    //--- Linear index
    ind   = xgrid + ygrid*GridSize + zgrid*GridSize*GridSize;
    //--- Increase number of galaxies per grid
    ngp_n[ind] = ngp_n[ind] + 1; 
  }
  printf("done\n"); fflush(stdout);

  //--- Offset
  for(i=0; i<N_GridSize; i++) ngp_o[i] = 0;
  printf("Creating offset array... "); fflush(stdout);
  for(i=1; i<N_GridSize; i++) ngp_o[i] = ngp_o[i-1] + ngp_n[i-1];
  printf("done\n"); fflush(stdout);

  //--- Index
  printf("Creating indexed array... "); fflush(stdout);
  for(i=0; i<N_part; i++){
    xgrid = (long) ((P[i].Pos[0]/BoxSize)*GridSize);
    ygrid = (long) ((P[i].Pos[1]/BoxSize)*GridSize);
    zgrid = (long) ((P[i].Pos[2]/BoxSize)*GridSize);
    //--- Linear index
    ind   = xgrid + ygrid*GridSize + zgrid*GridSize*GridSize;

    //--- Avoid empty cells
    if(ngp_n[ind] == 0) continue;

    //--- Fill indexed array with Id's
    ngp_i[ngp_o[ind]] = IDs[i];  
    //--- Increase the offset in cell to let room for new particle,
    //    Note that this is destructive operation, we must compute
    //    ngp_o again...
    ngp_o[ind] = ngp_o[ind] + 1; 
  }
  printf("done\n"); fflush(stdout);


  for(i=0; i<N_GridSize; i++) ngp_o[i] = 0;
  printf("Creating offset array again... "); fflush(stdout);
  for(i=1; i<N_GridSize; i++) ngp_o[i] = ngp_o[i-1] + ngp_n[i-1];
  printf("done\n"); fflush(stdout);


}


//=====================================
//--- Writes density file
//=====================================
void save_density(char *fname, int N_part, double *density)
{
  FILE *fd;
  char buf[200];
  int  i;
  double tempd; 

  //--- Load filename
  sprintf(buf,"%s",fname);
  
  printf("Writting densities to file '%s' ...\n",buf); fflush(stdout);
  fd=fopen(buf,"wb");
  //--- Header of file (number of partices)
  fwrite(&N_part,sizeof(int),1,fd);

  printf("Writting %d densities \n", N_part);
  for (i=0; i<N_part; i++) fwrite(&density[i],sizeof(double),1,fd);

  fclose(fd);
  printf("     Ready writting density file.\n");
  
}


//=====================================
//--- Numerical Recipes standard error handler
//=====================================
void nrerror(char error_text[])
{
  fprintf(stderr,"Numerical Recipes run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  exit(1);
}

//=====================================
//--- Allocate an integer vector with subscript range v[nl..nh]
//=====================================
long *lvector(long nh)
{
  long *v;

  v=(long *)malloc((size_t) ((nh)*sizeof(long)));
  if (!v) nrerror("allocation failure in lvector()");
  printf("Allocated %ld cells.\n", nh);
  return v;
}

//=====================================
//--- allocate a float vector with subscript range v[nl..nh]
//=====================================
double *dvector(long nh)
{
  double *v;
  v=(double *)malloc((size_t) ((nh)*sizeof(double)));
  if (!v) nrerror("allocation failure in double vector()");
  return v;
}

//=====================================
//--- Get indexes of shell around cell
//=====================================
long *shell(long *shell_ind, long nx, long ny, long nz, long n_grid)
{
  long x00,x01,x02,x03,x04,x05,x06,x07,x08,x09;
  long x10,x11,x12,x13,x14,x15,x16,x17,x18,x19;
  long x20,x21,x22,x23,x24,x25,x26;
  long y00,y01,y02,y03,y04,y05,y06,y07,y08,y09;
  long y10,y11,y12,y13,y14,y15,y16,y17,y18,y19;
  long y20,y21,y22,y23,y24,y25,y26;
  long z00,z01,z02,z03,z04,z05,z06,z07,z08,z09;
  long z10,z11,z12,z13,z14,z15,z16,z17,z18,z19;
  long z20,z21,z22,z23,z24,z25,z26;
  
  //--- X
  x00 = nx;
  x01 = nx-1;
  x02 = nx;
  x03 = nx+1;
  x04 = nx-1;
  x05 = nx;
  x06 = nx+1;
  x07 = nx-1;
  x08 = nx;
  x09 = nx+1;
  x10 = nx-1;
  x11 = nx;
  x12 = nx+1;
  x13 = nx-1;
  x14 = nx+1;
  x15 = nx-1;
  x16 = nx;
  x17 = nx+1;
  x18 = nx-1;
  x19 = nx;
  x20 = nx+1;
  x21 = nx-1;
  x22 = nx;
  x23 = nx+1;
  x24 = nx-1;
  x25 = nx;
  x26 = nx+1;
  //--- Y
  y00 = ny;
  y01 = ny-1;
  y02 = ny-1;
  y03 = ny-1;
  y04 = ny;
  y05 = ny;
  y06 = ny;
  y07 = ny+1;
  y08 = ny+1;
  y09 = ny+1;
  y10 = ny-1;
  y11 = ny-1;
  y12 = ny-1;
  y13 = ny;
  y14 = ny;
  y15 = ny+1;
  y16 = ny+1;
  y17 = ny+1;
  y18 = ny-1;
  y19 = ny-1;
  y20 = ny-1;
  y21 = ny;
  y22 = ny;
  y23 = ny;
  y24 = ny+1;
  y25 = ny+1;
  y26 = ny+1;
  //--- Z
  z00 = nz;
  z01 = nz-1;
  z02 = nz-1;
  z03 = nz-1;
  z04 = nz-1;
  z05 = nz-1;
  z06 = nz-1;
  z07 = nz-1;
  z08 = nz-1;
  z09 = nz-1;
  z10 = nz;
  z11 = nz;
  z12 = nz;
  z13 = nz;
  z14 = nz;
  z15 = nz;
  z16 = nz;
  z17 = nz;
  z18 = nz+1;
  z19 = nz+1;
  z20 = nz+1;
  z21 = nz+1;
  z22 = nz+1;
  z23 = nz+1;
  z24 = nz+1;
  z25 = nz+1;
  z26 = nz+1;
  
  
  //--- Periodic conditions
  if(nx == 0){
    x01 = n_grid-1;
    x04 = n_grid-1;
    x07 = n_grid-1;
    x10 = n_grid-1;
    x13 = n_grid-1;
    x15 = n_grid-1;
    x18 = n_grid-1;
    x21 = n_grid-1;
    x24 = n_grid-1;}
  
  if(nx == n_grid-1){
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
    y01 = n_grid-1;
    y02 = n_grid-1;
    y03 = n_grid-1;
    y10 = n_grid-1;
    y11 = n_grid-1;
    y12 = n_grid-1;
    y18 = n_grid-1;
    y19 = n_grid-1;
    y20 = n_grid-1;}
  
  if(ny == n_grid-1){
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
    z01 = n_grid-1;
    z02 = n_grid-1;
    z03 = n_grid-1;
    z04 = n_grid-1;
    z05 = n_grid-1;
    z06 = n_grid-1;
    z07 = n_grid-1;
    z08 = n_grid-1;
    z09 = n_grid-1;}

  if(nz == n_grid-1){
    z18 = 0;
    z19 = 0;
    z20 = 0;
    z21 = 0;
    z22 = 0;
    z23 = 0;
    z24 = 0;
    z25 = 0;
    z26 = 0;}

  //--- Evaluate the shell_ind
  shell_ind[0] = x00 + y00*n_grid + z00*n_grid*n_grid;
  shell_ind[1] = x01 + y01*n_grid + z01*n_grid*n_grid;
  shell_ind[2] = x02 + y02*n_grid + z02*n_grid*n_grid;
  shell_ind[3] = x03 + y03*n_grid + z03*n_grid*n_grid;
  shell_ind[4] = x04 + y04*n_grid + z04*n_grid*n_grid;
  shell_ind[5] = x05 + y05*n_grid + z05*n_grid*n_grid;
  shell_ind[6] = x06 + y06*n_grid + z06*n_grid*n_grid;
  shell_ind[7] = x07 + y07*n_grid + z07*n_grid*n_grid;
  shell_ind[8] = x08 + y08*n_grid + z08*n_grid*n_grid;
  shell_ind[9] = x09 + y09*n_grid + z09*n_grid*n_grid;
  shell_ind[10] = x10 + y10*n_grid + z10*n_grid*n_grid;
  shell_ind[11] = x11 + y11*n_grid + z11*n_grid*n_grid;
  shell_ind[12] = x12 + y12*n_grid + z12*n_grid*n_grid;
  shell_ind[13] = x13 + y13*n_grid + z13*n_grid*n_grid;
  shell_ind[14] = x14 + y14*n_grid + z14*n_grid*n_grid;
  shell_ind[15] = x15 + y15*n_grid + z15*n_grid*n_grid;
  shell_ind[16] = x16 + y16*n_grid + z16*n_grid*n_grid;
  shell_ind[17] = x17 + y17*n_grid + z17*n_grid*n_grid;
  shell_ind[18] = x18 + y18*n_grid + z18*n_grid*n_grid;
  shell_ind[19] = x19 + y19*n_grid + z19*n_grid*n_grid;
  shell_ind[20] = x20 + y20*n_grid + z20*n_grid*n_grid;
  shell_ind[21] = x21 + y21*n_grid + z21*n_grid*n_grid;
  shell_ind[22] = x22 + y22*n_grid + z22*n_grid*n_grid;
  shell_ind[23] = x23 + y23*n_grid + z23*n_grid*n_grid;
  shell_ind[24] = x24 + y24*n_grid + z24*n_grid*n_grid;
  shell_ind[25] = x25 + y25*n_grid + z25*n_grid*n_grid;
  shell_ind[26] = x26 + y26*n_grid + z26*n_grid*n_grid;
  
  return shell_ind;
}



