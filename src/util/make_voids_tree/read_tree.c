#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "inc/tree.h"


int main(void) 
{
  int i,j,k,w;
  
  //--- Create tree
  struct node *Tree       = node_alloc(1);
  struct node *temp_node1 = NULL;
  struct node *temp_node2 = NULL;
  
  
  //--- Recursively fill tree
  Tree = fill_tree(Tree, 3);


  //--- Print tree
  print_tree(Tree);
  
  
  //--- Get a speciffic node 
  temp_node1 = get_node_ID_in_level(Tree, 2579 , 1);

  printf("========================================\n");

  print_tree(temp_node1);

  printf("========================================\n");

  printf("%d\n", cumul_tree(temp_node1));

  printf("========================================\n");

  temp_node1->body.Size=10;

  printf("[%d, %d]\n", temp_node1->ID, temp_node1->body.Size);
  
  /*

  printf("----------------------------\n");

  temp_node1->ID = 99;
  print_tree(temp_node1);

  printf("----------------------------\n");

  temp_node1->ID = 11;
  print_tree(temp_node1);

  */

  //--- Get an speciffic node 
  //temp_node2 = get_node_ID_in_level(Tree, 10, 1);

  //--- Exchange children
  //temp_node1->children =  temp_node2->children;
  
  
  //print_tree(Tree);


  exit(0);
}
