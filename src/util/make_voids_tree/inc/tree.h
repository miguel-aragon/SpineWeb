#ifndef HEADER_TREE_H
#define HEADER_TREE_H
/*



*/

//==========================
//--- The void node
//==========================
struct Voids {
  int    ID;       //--- ID of node
  int    Level;    //--- Level in tree

  //--- Void properties
  int    Size;
  float  MinDen;
  float  MaxDen;
  float  MeanDen;
  float  MedianDen;

};


//==========================
//--- The tree node
//==========================
struct node {
  int    ID;       //--- ID of node
  int    Level;    //--- Level in tree

  //--- body contains the void properties
  struct Voids body;


  //--- Pointer to wall graph
  //struct wall *walls;

  //--- Pointer to filament graph
  //struct filament *filas;

  //--- Tree structure (scale)
  struct node *parent;     //--- Points to the parent node
  int    N_children;
  struct node *children;   //--- Points to array of children
  //--- Tree structure (time)
  //int    N_past;
  //struct node *past;
  //int    N_next;
  //struct node *next;

  //--- Transverse tree
  //struct node *next_horizontal;     //--- Points to the next node in the same level

};





//--- Core tree functions
struct node *node_alloc(int N_nodes);
struct node *CreateNode(struct node *p, int Level, int Id, int N_children);
struct node *fill_tree(struct node *p, int N_level);

//--- Tree tools
struct node *get_node_ID_in_level (struct node *p, int ID, int level);
void         print_tree(struct node *p);
void         print_tree_simple(struct node *p);
void         nodes_in_level(struct node *p, int comp);
int          cumul_tree(struct node *p);



#endif
