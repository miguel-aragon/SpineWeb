#ifndef HEADER_WATER_HIERARCHY_H
#define HEADER_WATER_HIERARCHY_H
/*



*/


int *watershed_sorted_int_27_marked(double *density, int *voids12, int x_grid, int y_grid, int z_grid);
int *merge_regions_int     (const int *voids1, const int *voids2, int n_pixels);
int *merge_regions_grid_int(const int *voids1, const int *voids2, int n_pixels, int x_grid, int y_grid, int z_grid);
int  adopt_orphan_int(const int *voids1, const int *voids2, int orphan_ID, int n_orphan, int x_grid, int y_grid, int z_grid);
void fix_watershed_IDs(int *voids1, int n_pixels);

#endif
