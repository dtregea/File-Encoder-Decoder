//
// file: encode.h
// description: Definition file for utility functions for encoding a file
//
// @author Daniel Tregea
//

#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include "HeapDT.h"
#include "packman_utils.h"

/// Comparison function for min heaps
/// @param first_node Node to be compared to second_node
/// @param second_node Node to be compared to first_node
/// @return Whether first_node is less than second_node
int compare_node_min( const void * first_node, const void * second_node );

/// Print function for frequency heap nodes
/// @param item Frequency heap node to print
/// @param outfp Stream to print node to
void print_node( const void * item, FILE * outfp );

/// Generate a huffman tree from a frequency heap
/// @param heap Frequency heap
/// @return Head of newly created huffman tree
Tree_node heap_to_huffman( Heap heap );

/// Compress a sequence of 1's and 0's into an array of unsigned integers
/// @param bits Array of encoded binary
/// @param num_bits size of "bits"
/// @param num_uint Number of unsigned integers (4 bytes) needed to hold all bits in "bits" array
/// @return Array of unsigned integers holding each bit from the "bits" array
uint * pack_bits( const uint * bits, uint num_bits, uint num_uint );

#endif