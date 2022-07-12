//
// file: encode.c
// description: Implementation file for utility functions for encoding a file
//
// @author Daniel Tregea
//

#include <stdlib.h>
#include "packman_utils.h"
#include "encode.h"
#include "utilities.h"

/// Comparison function for min heaps
/// @param first_node Node to be compared to second_node
/// @param second_node Node to be compared to first_node
/// @return Whether first_node is less than second_node
int compare_node_min( const void * first_node, const void * second_node ){
    Tree_node left = (Tree_node)first_node;
    Tree_node right = (Tree_node)second_node;
    return left->freq < right->freq;
}

/// Print function for frequency heap nodes
/// @param item Frequency heap node to print
/// @param outfp Stream to print node to
void print_node( const void * item, FILE * outfp ){
    Tree_node node = (Tree_node) item;
    fprintf(outfp, "Frequency: %d, Symbol: %u\n", node->freq, node->sym);
}

/// Generate a huffman tree from a frequency heap
/// @param heap Frequency heap
/// @return Head of newly created huffman tree
Tree_node heap_to_huffman( Heap heap ){
    while(hdt_size(heap) >= 2){
        Tree_node n1 = hdt_remove_top(heap);
        Tree_node n2 = hdt_remove_top(heap);
        Tree_node nx = create_tree_node(0, (n1->freq + n2->freq), 1);
        nx->left = n1;
        nx->right = n2;
        hdt_insert_item(heap, nx);
    }
    return (Tree_node) hdt_top(heap);
}

/// Compress a sequence of 1's and 0's into an array of unsigned integers
/// @param bits Array of encoded binary
/// @param num_bits size of "bits"
/// @param num_uint Number of unsigned integers (4 bytes) needed to hold all bits in "bits" array
/// @return Array of unsigned integers holding binary sequence from "bits" array
uint * pack_bits( const uint * bits, uint num_bits, uint num_uint ){
    uint * encoded_binary = calloc(num_uint, sizeof(uint));
    for(uint i = 0; i < num_bits; i++){
        if(bits[i] == 1) // insert a 1 in the correct position within an unsigned integer
            encoded_binary[get_byte_index(i)] |= get_mask(i);
    }
    return encoded_binary;
}