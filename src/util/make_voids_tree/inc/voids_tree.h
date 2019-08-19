#ifndef WATER_TREE_H
#define WATER_TREE_H
/*



*/

void fill_void_tree(int *voids1, int *voids2, int *voids3, int n_pixels);
void get_parent_child_link(int *voids1, int *voids2, int n_pixels, int *parent_node, int *children_node);

void write_tree_xml  (int *parent_node, int n_parent_node, int *children_node, int n_children_node);
void write_tree_ascii(int *parent_node, int n_parent_node, int *children_node, int n_children_node);

void write_tree_ascii_1_2_3(int *parent_node_1_2, int n_parent_node_1_2, int *children_node_1_2, int n_children_node_1_2,
			    int *parent_node_2_3, int n_parent_node_2_3, int *children_node_2_3, int n_children_node_2_3);

void write_tree_ascii_1_2_3_plain(int *parent_node_1_2, int n_parent_node_1_2, int *children_node_1_2, int n_children_node_1_2,
				  int *parent_node_2_3, int n_parent_node_2_3, int *children_node_2_3, int n_children_node_2_3);


#endif
