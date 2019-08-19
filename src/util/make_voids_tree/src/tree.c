#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../inc/tree.h"

//==================================================
//--- Allocate array of N nodes
//==================================================
struct node *node_alloc(int N_nodes)
{
  int i;
  struct node *p;
  
  p = (struct node *) malloc(sizeof(struct node) * N_nodes);

  //--- Initialize pointers
  for (i=0; i<N_nodes; i++){
    p[i].children = NULL;
    p[i].parent   = NULL;
  }

  return p;
}


//==================================================
//--- Create Tree with N children
//==================================================
struct node *CreateNode(struct node *p, int Level, int Id, int N_children)
{
  int i;

  //--- Fill ID and default parent
  p->Level      = Level;
  p->ID         = Id;
  p->parent     = NULL;

  //--- If asked for children then allocate space
  if (N_children == 0) 
    p->children   = NULL;
  else
   p->children   = node_alloc(N_children); 

  //--- Number of children
  p->N_children = N_children;             

  return p;
}



//==================================================
//--- Fill the tree
//==================================================
struct node *fill_tree(struct node *p, int N_level)
{
  static int level=0;
  int        i,j;
  int        N_children;
  int        dump;

  //--- If we reached the bottom of the tree then
  //    break recursion
  if (level >= N_level) return p;

  
  //--- Read number of children from default io
  dump = scanf("%d",&N_children);

  p->Level      = level;
  p->children   = node_alloc(N_children);
  p->N_children = N_children;

  //--- Loop on children iteratively
  for (i=0; i< N_children; i++) {
    //--- Call recursively each children

    //--- Read children's Id from default io
    dump = scanf("%d",&(p->children[0].ID));

    p->children[0].parent = p;
    p->children[0].Level  = p->Level+1;

    //--- Enter recursive function
    level++;
    p->children = fill_tree(p->children, N_level);
    level--;

    //--- Increase pointer to children
    p->children++;
  }

  //--- Bad programming. I should better access each element of
  //    p->children directly in the previous two loops. Now fix the mess...
  p->children -= N_children;

  return p;
}

//==================================================
//--- Find node in tree given its level in the tree and its Id.
//    This fucntion returns a pointer to the speciffied node.
//    You can also use it to cosntruct a tree "by hand". This is
//    handy when you don't have sequential (iterative) access to
//    the data. However, you MUST know the number of children
//    per node in advance.
//==================================================
struct node *get_node_ID_in_level(struct node *p, int ID, int level)
{
  int i,j,k;
  struct node *parent_node  = (*p).parent;
  struct node *current_node = p;
  static int   found = 0;

  //--- Break recursion
  if (p->Level==level && p->ID==ID) 
    {
      found = 1;
      return p;
    } 
      
  //--- Loop on children and call recursively
  for (i=0; i< p->N_children; i++) {
    
    current_node = &p->children[i];
    current_node = get_node_ID_in_level(current_node, ID, level);
    
    //--- Don't continue with the loop
    if (found == 1) break;
    
  } //--- end for

  return current_node;
}      


//==================================================
//--- Print tree
//==================================================
void print_tree(struct node *p)
{
  int i,j,k;
  static int cont=0;

  //--- Add spaces and print ID
  for (j=0; j<p->Level; j++) printf("         |");


  //--- Print ID and parent node
  if (p->parent != NULL) 
    printf("[%d P%d|I%d", p->Level,p->parent->ID, p->ID);
  else 
    printf("[%d I%d|NULL", p->Level, p->ID);
  if (p->children == NULL) 
    printf("|NULL");
  printf("]\n");

  //--- Increase recursion level counter
  cont++;

  //--- Loop on children
  for (i=0; i< p->N_children; i++) {
    //--- Call recursively each children10
    print_tree(&p->children[i]);
    //--- Decrement recursion level after exiting recursion
    cont --;
  }

}


//==================================================
//--- Print tree
//==================================================
void print_tree_simple(struct node *p)
{
  int i,j,k;
  static int cont=0;

  //--- Add spaces and print ID
  for (j=0; j<p->Level; j++) printf("         |");


  //--- Print ID and parent node
  printf("[%d|I%d", p->Level, p->ID);

  if (p->children == NULL) printf("|NULL");

  printf("]\n");

  //--- Increase recursion level counter
  cont++;

  //--- Loop on children
  for (i=0; i< p->N_children; i++) {
    //--- Call recursively each children10
    print_tree(&p->children[i]);
    //--- Decrement recursion level after exiting recursion
    cont --;
  }

}


//==================================================
//--- Add all elements down the tree cumulatively
//==================================================
int cumul_tree(struct node *p){
  static int cumul=0;
  int        i;

  //--- Add element to cumulative
  cumul += p->ID;

  //--- Loop on children and do recursion
  for (i=0; i< p->N_children; i++) {
    cumul = cumul_tree(&p->children[i]);
  }

  return cumul;
}

