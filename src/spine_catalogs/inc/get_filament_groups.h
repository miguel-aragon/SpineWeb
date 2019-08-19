#ifndef HEADER_GET_FILAMENT_GROUPS_H
#define HEADER_GET_FILAMENT_GROUPS_H
/*


*/

struct triplet_{
  long ind;
  long group;
  long v0;
  long v1;
  long v2;
};

typedef struct triplet_ triplet;

int *get_filament_groups(int *voids, char *spine, int sizeX, int sizeY, int sizeZ);

static int comp_3(const void *m1, const void *m2);

void compare_and_swap(long *a, long *b);

void sort_3long(long *a, long *b, long *c);
 

#endif
