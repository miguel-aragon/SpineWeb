/*


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../lib/pixel3.h"
#include "../../lib/numeric.h"
#include "../inc/tree.h"
#include "../inc/voids_tree.h"

//=========================================================
//   Return *parent_node with the number of children per parent
//   and *children_node with the ID's of parent for each child.
//=========================================================
void get_parent_child_link(int *voids1, int *voids2, int n_pixels, int *parent_node, int *children_node)
{
    int  i,j,k;
    int *histo1=NULL;
    int *histo1_index=NULL;
    int  histo1_index_off=0;
    int  n_histo1=0;
    //--- Intersection
    int *inter_array=NULL;
    int *histo_inter_array=NULL;
    int  n_histo_inter_array=0;
    //--- Trick to get the maximum element that is not 0
    int *histo_inter_array_off=NULL;
    //--- maximum contribution
    int *where_max=NULL;
    int  n_where_max;
    int  max_inter_ID;

    histo1       = histogram_int(voids1, n_pixels, &n_histo1);
    histo1_index = histogram_int_index(voids1, n_pixels);


    //=============================================
    //--- Loop over all regions in level 1 and find the intersection regions in
    //    level 2.
    //=============================================
    histo1_index_off = histo1[0]; //--- Start at region 1, 0 is the watershed
    for (i=1;i<n_histo1;i++)
    {

        //--- Skip empty bins
        if (histo1[i] == 0) continue;

        //--- get the intersection between voids 1 and voids2. Check which regions
        //    in voids2 are covered by region with Id i in voids1
        //--- Allocate memory for intersection
        inter_array = (int *) malloc(sizeof(int)*histo1[i]);
        for (j=0;j<histo1[i];j++) inter_array[j] = voids2[histo1_index[histo1_index_off + j]];

        //--- Histogram of indexes inside intersection
        histo_inter_array = histogram_int(inter_array, histo1[i], &n_histo_inter_array);

        //--- Get the region in voids2 to which region i in voids1 contributes
        //    the most. There may be a degeneracy in the case of two or more
        //    maximum. In this case just take one. One could improve this by using
        //    density to choose between maximum or something like that.
        //    In the case of the delaunay if one takes the volume this should not be
        //    a problem. Here arises from the fix size of the pixels.
        n_where_max = n_histo_inter_array;
        where_max   = where_int(max_int(histo_inter_array,n_histo_inter_array),"=",histo_inter_array, &n_where_max);

        //--- Add offset in case 0 is not the minimim Id in inter_array. Not sure if this
        //    actually works. The second line seems to make more sense...
        //max_inter_ID = min_int(inter_array, histo1[i]) + where_max[0];
        max_inter_ID = where_max[0];

        //--- If the parent is the watershed (Id=0) then choose the second largest
        if (max_inter_ID == 0)
        {
            //--- Find the largest element in histo_inter_array that is not 0. In this case it is easy since
            //    we know that cero is ALWAYS the first element. It may be that inter_array does not contain
            //    ceroes but that case is excluded by the if(max_inter_ID == 0).
            n_where_max = n_histo_inter_array-1;
            histo_inter_array_off = histo_inter_array;
            histo_inter_array_off++;
            where_max   = where_int(max_int(histo_inter_array_off,n_histo_inter_array-1),"=",histo_inter_array_off, &n_where_max);
            max_inter_ID = where_max[0]+1;
        }

	//--- Increase the number of children in each child cell;
	parent_node[max_inter_ID] = parent_node[max_inter_ID]+1;
	//--- Store the parent ID in the children_node. Not the children!!! Choose better name.
	children_node[i] = max_inter_ID;

        //--- Increase offset of histo_index1
        histo1_index_off = histo1_index_off+histo1[i];
        //--- Free array, otherwise bad memory leak...
        free(inter_array);
    }


}


//=========================================================
//
//
//
//=========================================================
void fill_void_tree(int *voids1, int *voids2, int *voids3, int n_pixels)
{

  int  i,j,k;
  int *parent_node_1_2=NULL;
  int  n_parent_node_1_2;
  int *children_node_1_2=NULL;
  int  n_children_node_1_2;
  int *parent_node_2_3=NULL;
  int  n_parent_node_2_3;
  int *children_node_2_3=NULL;
  int  n_children_node_2_3;


  //===================================================
  //--- Allocate children nodes. Add one element to account for cero index
  n_children_node_1_2 = max_int(voids1, n_pixels) + 1;
  children_node_1_2   = (int *) malloc(sizeof(int) * n_children_node_1_2);
  for (i=0;i<n_children_node_1_2;i++) children_node_1_2[i]=0;

  //--- Allocate parent node. Add one element to account for cero index
  n_parent_node_1_2  = max_int(voids2, n_pixels) + 1;
  parent_node_1_2     = (int *) malloc(sizeof(int) * n_parent_node_1_2);
  for (i=0;i<n_parent_node_1_2;i++) parent_node_1_2[i]=0;
  //---------------------------------------------------

  //--- Get the relation between child-parent
  get_parent_child_link(voids1,voids2,n_pixels, parent_node_1_2,children_node_1_2);

  //--- Print to file
  write_tree_xml(parent_node_1_2, n_parent_node_1_2, children_node_1_2, n_children_node_1_2);
  write_tree_ascii(parent_node_1_2, n_parent_node_1_2, children_node_1_2, n_children_node_1_2);


  //===================================================
  //--- Allocate children nodes. Add one element to account for cero index
  n_children_node_2_3 = max_int(voids2, n_pixels) + 1;
  children_node_2_3   = (int *) malloc(sizeof(int) * n_children_node_2_3);
  for (i=0;i<n_children_node_2_3;i++) children_node_2_3[i]=0;

  //--- Allocate parent node. Add one element to account for cero index
  n_parent_node_2_3  = max_int(voids3, n_pixels) + 1;
  parent_node_2_3     = (int *) malloc(sizeof(int) * n_parent_node_2_3);
  for (i=0;i<n_parent_node_2_3;i++) parent_node_2_3[i]=0;
  //---------------------------------------------------

  //--- Get the relation between child-parent
  get_parent_child_link(voids2,voids3,n_pixels, parent_node_2_3,children_node_2_3);

  //--- Print to file
  //write_tree_xml(parent_node_2_3, n_parent_node_2_3, children_node_2_3, n_children_node_2_3);
  //write_tree_ascii(parent_node_2_3, n_parent_node_2_3, children_node_2_3, n_children_node_2_3);

  //write_tree_ascii_1_2_3(parent_node_1_2, n_parent_node_1_2, children_node_1_2, n_children_node_1_2, parent_node_2_3, n_parent_node_2_3, children_node_2_3, n_children_node_2_3);

  write_tree_ascii_1_2_3_plain(parent_node_1_2, n_parent_node_1_2, children_node_1_2, n_children_node_1_2, parent_node_2_3, n_parent_node_2_3, children_node_2_3, n_children_node_2_3);


  free(parent_node_1_2);
  free(children_node_1_2);
  free(parent_node_2_3);
  free(children_node_2_3);


}

//=========================================================
//
//
//
//=========================================================
void write_tree_xml(int *parent_node, int n_parent_node, int *children_node, int n_children_node)
{

  int i,j;
  FILE *file_out;


  printf(">>> Write XML... %d %d \n",n_parent_node, n_children_node);
  fflush(stdout);

  file_out = fopen("tree.xml", "w");
  
  //--- Write header
  fprintf(file_out, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
  
  //--- Root node
  fprintf(file_out, "<Root>\n");
  fprintf(file_out, "<Id> 0 </Id>\n");
  fprintf(file_out, "<N_ch> %d </N_ch>\n", n_parent_node);
  for (i=0;i<n_parent_node;i++) {

    fprintf(file_out,"\t<Node>\n");
    fprintf(file_out, "\t\t<Id> %d </Id>\n", i);
    fprintf(file_out, "\t\t<N_ch> %d </N_ch>\n", parent_node[i]);

    //--- Look for the parent in the children array. Brute force...
    for (j=0;j<n_children_node;j++) { 
      if (children_node[j] == i) 
	{
	  fprintf(file_out,"\t\t<Node>\n");
	  fprintf(file_out,"\t\t\t<Id> %d </Id>\n", j);
	  fprintf(file_out,"\t\t\t<N_ch> 0 </N_ch>\n");
	  fprintf(file_out,"\t\t</Node>\n");
	}
    } //  end for j
    fprintf(file_out,"\t</Node>\n");
  } //  end for i
  
  fprintf(file_out, "</Root>\n");
  //--- Close file
  fclose(file_out); 
  
}


//=========================================================
//
//
//
//=========================================================
void write_tree_ascii(int *parent_node, int n_parent_node, int *children_node, int n_children_node)
{
  int i,j;
  FILE *file_out;

  printf(">>> Write ascii...\n");
  fflush(stdout);


  file_out = fopen("tree.dat", "w");
  
  //--- Write header
  //--- Root node
  fprintf(file_out, "%d\n", n_parent_node);
  for (i=0;i<n_parent_node;i++) {
    
    fprintf(file_out, "\t%d  %d\n", i, parent_node[i]);

    //--- Look for the parent in the children array. Brute force...
    for (j=0;j<n_children_node;j++) {      
      if (children_node[j] == i) 
	{
	  fprintf(file_out,"\t\t%d\n", j);
	}
      
    } //  end for j
  } //  end for i
    //--- Close file
  fclose(file_out); 

}

//=========================================================
//
//
//
//=========================================================
void write_tree_ascii_1_2_3(int *parent_node_1_2, int n_parent_node_1_2, int *children_node_1_2, int n_children_node_1_2,
		       int *parent_node_2_3, int n_parent_node_2_3, int *children_node_2_3, int n_children_node_2_3)
{
  int i,j,k;
  FILE *file_out;

  printf(">>> Write ascii...\n");
  fflush(stdout);


  file_out = fopen("tree_123.dat", "w");
  
  fprintf(file_out, "%d\n", n_parent_node_2_3);

  //--- Loop over parent voids and find the children that are attached to it.
  for (i=0;i<n_parent_node_2_3;i++) {
    
    fprintf(file_out, "\t%d  [%d]\n", i, parent_node_2_3[i]);

    //--- If no children then stop
    if (parent_node_2_3[i] == 0) continue;

    //--- Look for the parent in the children array. Brute force...
    for (j=0;j<n_children_node_2_3;j++) {      
      if (children_node_2_3[j] == i) 
	{
	  fprintf(file_out,"\t\t%d  [%d]\n", j, parent_node_1_2[j]);

	  //--- If no children then stop
	  if (parent_node_1_2[j] == 0) continue;
	  
	  //--- Look in the second level of the tree
	  for (k=0;k<n_children_node_1_2;k++)
	    {
	      if (children_node_1_2[k] == j)
		{
		  fprintf(file_out,"\t\t\t%d  0\n", k);		
		}  // end if
	    } //  end for k
	} // end if
    } //  end for j
  } //  end for i

  //--- Close file
  fclose(file_out); 

}

//=========================================================
//
//
//
//=========================================================
void write_tree_ascii_1_2_3_plain(int *parent_node_1_2, int n_parent_node_1_2, int *children_node_1_2, int n_children_node_1_2,
				  int *parent_node_2_3, int n_parent_node_2_3, int *children_node_2_3, int n_children_node_2_3)
{
  int i,j,k;
  FILE *file_out;

  printf(">>> Write ascii...\n");
  fflush(stdout);


  file_out = fopen("tree_123.raw", "w");
  
  fprintf(file_out, "%d\n", n_parent_node_2_3);

  //--- Loop over parent voids and find the children that are attached to it.
  for (i=0;i<n_parent_node_2_3;i++) {
    
    fprintf(file_out, "%d\n%d\n", i, parent_node_2_3[i]);

    //--- If no children then stop
    if (parent_node_2_3[i] == 0) continue;

    //--- Look for the parent in the children array. Brute force...
    for (j=0;j<n_children_node_2_3;j++) {      
      if (children_node_2_3[j] == i) 
	{
	  fprintf(file_out,"%d\n%d\n", j, parent_node_1_2[j]);
	  
	  //--- If no children then stop
	  if (parent_node_1_2[j] == 0) continue;

	  //--- Look in the second level of the tree
	  for (k=0;k<n_children_node_1_2;k++)
	    {
	      if (children_node_1_2[k] == j){
		fprintf(file_out,"%d\n", k);	
	      }  // end if
	    } //  end for k
	} // end if
    } //  end for j
  } //  end for i
  //--- Close file
  fclose(file_out); 

}
