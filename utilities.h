//
// file: utilities.h
// description: Definition file for utility functions common to encoding and decoding
//
// @author Daniel Tregea
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include "packman_utils.h"

/// Populate a look up table with symbol codes, given the head of a huffman tree
/// @param lut Look up table to populate
/// @param node Head of the huffman tree
/// @param code Current code Generated from tree traversal.
/// @param new_char New character to be added onto the code.
/// @precondition code and new_char are empty strings on function call.
void populate_lut( char ** lut, const Tree_node node, char * code, char * new_char );

/// Free all the dynamically allocated memory in the look up table
/// @param lut
void free_lut( char ** lut );

/// Determine the existance of the packman magic number in a file
/// @param fp The file to find the magic number
/// @return 1 for magic number found. 0 for magic number not found. -1 on error.
int find_packman_magic( FILE * fp );

/// Report errors and return EXIT_FAILURE
/// @param file_name Name of the file the error occured in
/// @param line Line of the file the error occurred in
/// @param input_file_name Name of the input file being read
/// @param message Error message
/// @return EXIT_FAILURE
int handle_error( char * file_name, int line, char * input_file_name, char * message );

/// Determine the number of unsigned integers needed to hold a number of bits
/// @param num_bits Number of bits to be held in unsigned integers
/// @return Number of unsigned integers to hold num_bits
uint bits_to_num_uint( uint num_bits );

/// Determine the unsigned integer holding the bit at position bit_number
/// @param bit_number The position of the bit
/// @return The unsigned integer index holding the bit at bit position bit_number
uint get_byte_index( uint bit_number );

/// Get an unsigned integer with 1 at bit position (bit_number) with 0 on rest of bits
/// @param bit_number Bit position to put a 1
/// @return Unsigned integer with 1 at bit position (bit_number)
uint get_mask( uint bit_number );

/// Determine the output stream from the command line
/// @param argv Command line argument
/// @return Pointer to the stream specified in the command line arguments
FILE * get_output_stream( char ** argv );

#endif