#ifndef HEADER_WATERSHED_SPINE_H
#define HEADER_WATERSHED_SPINE_H
/*


*/


char *get_void_boundaries(int *voids, int sizeX, int sizeY, int sizeZ);

void local_watershed_sort(double *den, int *voids, char *boundaries, int sizeX, int sizeY, int sizeZ);
void local_watershed_sort_float(float *den, int *voids, char *boundaries, int sizeX, int sizeY, int sizeZ);


#endif
