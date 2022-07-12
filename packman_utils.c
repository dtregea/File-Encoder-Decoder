//
// File: packman_utils.c
// Description:
//    Utility definitions for Packman, the Encoder-Decoder
//    packman is an encoder/decoder program using huffman encoding.
//
// @author Daniel Tregea, dvt2706@rit.edu
//
// // // // // // // // // // // // // // // // // // // // // // // //

#include "packman_utils.h"
#include <stdlib.h>
#include <stdio.h>

/// Create a TreeNode from a given symbol and frequency
/// @param symbol Symbol to be stored in the node
/// @param frequency Frequency of the symbol
/// @return TreeNode containing the symbol and frequency
Tree_node create_tree_node( uchar sym, int freq, int internal){
  Tree_node new_node = NULL;
  new_node = malloc(sizeof( struct Tree_node_s));
  new_node->sym = sym;
  new_node->freq = freq;
  new_node->internal = internal;
  new_node->right = NULL;
  new_node->left = NULL;
  return new_node;
}

/// Free a TreeNode and all of its children
/// @param node Head of TreeNode to be freed
void free_tree(Tree_node node){
  if(node == NULL){
    return;
  }
  free_tree(node->left);
  free_tree(node->right);
  free(node);
}

/// Write the packman magic number to a file indicating it is encoded by this program
/// @param fp Output stream to write to
int write_magic( FILE * ofp){
  unsigned short * magic_num_array = malloc(sizeof(unsigned short));
  magic_num_array[0] = 0x80F0;
  fwrite(magic_num_array, sizeof(unsigned short), 1, ofp);
  free(magic_num_array);
  return 1;
}

/// Write a binary tree to a file
/// @param tree Tree to be written to file
/// @param fp Output stream to write to
int write_tree( FILE * ofp, Tree_node node){
  if(node == NULL){
    return 0;
  }
  
  uchar symbol[1] = {node->sym};
  uchar leaf[1] = {0x01};
  if(node->internal == 0){ // if leaf write 0x01 first
    fwrite(leaf, sizeof(uchar), 1, ofp);
  }
  fwrite(symbol, sizeof(uchar), 1, ofp); // write symbol
  write_tree(ofp, node->left);
  write_tree(ofp, node->right);
  
  return 1;
  
}

/// Read a binary tree from a file
/// @param fp Input stream to read from
/// @return TreeNode containing the tree read from file
Tree_node read_tree( FILE * fp ){
  long int pos;
  pos = ftell(fp);
  uchar byte_read[1];
  fread(byte_read, sizeof(uchar), 1, fp);
  Tree_node new_node;
  if(byte_read[0] == 0){ // Internal node read
    new_node = create_tree_node(0,0, 1);
    new_node->left = read_tree(fp);
    new_node->right = read_tree(fp);
  } else if(byte_read[0] == 0x01){ // Leaf node read
    
    uchar leaf[1];
    fread(leaf, sizeof(uchar), 1, fp);
    new_node = create_tree_node(leaf[0],0, 0);
  } else {
    fseek (fp, pos, SEEK_SET);
    new_node = NULL;
  }
  
  return new_node;
}

/// Display an error message
/// @param codefile Name of the file the error occurred in
/// @param line Line number the error occurred on
/// @param fname Name of the input file being read
/// @param msg Message to display
void report_error( char * codefile, int line, char * fname, char * msg ){
  fprintf(stderr, "%s:%d | \'%s\' - %s\n", codefile, line, fname, msg);
}

/// Print a tree to stdout
/// @param node Head of TreeNode to be printed
void print_tree( Tree_node tree ){
if(tree == NULL) return;
  printf("[%c, %d]-", tree->sym, tree->freq);
  print_tree(tree->left);
  print_tree(tree->right);
}
