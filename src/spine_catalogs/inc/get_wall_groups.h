#ifndef HEADER_GET_WALL_GROUPS_H
#define HEADER_GET_WALL_GROUPS_H
/*


*/

struct tuple_{
  long ind;
  long group;
  long v0;
  long v1;
};

typedef struct tuple_ tuple;

int *get_wall_groups(int *voids, char *spine, int sizeX, int sizeY, int sizeZ);

static int compmi(const void *m1, const void *m2);


#endif
