//
// file: utilities.c
// description: Implementation file for utility functions common to encoding and decoding
//
// @author Daniel Tregea
//

#define _DEFAULT_SOURCE
#include "utilities.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <endian.h>

/// Populate a look up table with symbol codes, given the head of a huffman tree
/// @param lut Look up table to populate
/// @param node Head of the huffman tree
/// @param code Current code Generated from tree traversal.
/// @param new_char New character to be added onto the code.
/// @precondition code and new_char are empty strings on function call.
void populate_lut( char ** lut, const Tree_node node, char * code, char * new_char ){
    
    // Base case
    if(node == NULL)
        return;
        
    
    // Append the new char (0 or 1) onto the generated code
    char * new_code = malloc(strlen(code) + 2);
    strcpy(new_code, code);
    strcat(new_code, new_char);
    
    // Leaf node of huffman tree reached, copy the generated code to its associated symbol
    if(node->left == NULL && node->right == NULL){
        lut[node->sym] = malloc(strlen(new_code) + 1);
        strcpy(lut[node->sym], new_code);  
      }
    
    // Traverse the left and right sub trees
    populate_lut(lut, node->left, new_code, "0");
    populate_lut(lut, node->right, new_code, "1");
    
    // Free code after subtree traversal finished
    free(new_code);
}

/// Free all the dynamically allocated memory in the look up table
/// @param lut
void free_lut( char ** lut ){
    for(int i = 0; i < 256; i++){
        if(lut[i] != NULL)
            free(lut[i]);
    }
}

/// Determine the existance of the packman magic number in a file
/// @param fp The file to find the magic number
/// @return 1 for magic number found. 0 for magic number not found. -1 on error.
int find_packman_magic( FILE * fp ){
    uchar magic_number[2];
    if(fread(magic_number, sizeof(uchar), 2, fp) == 0)
        return -1;
    
    // Combine the bytes of the two unsigned char's read to a unsigned short
    unsigned short magic_num_short = 0;
    magic_num_short = magic_number[0];
    magic_num_short <<= 8;
    magic_num_short |= magic_number[1];
    magic_num_short = htobe16(magic_num_short);
    
    return magic_num_short == 0x80F0 ? 0 : 1;
}

/// Report errors and return EXIT_FAILURE
/// @param file_name Name of the file the error occured in
/// @param line Line of the file the error occurred in
/// @param input_file_name Name of the input file being read
/// @param message Error message
/// @return EXIT_FAILURE
int handle_error( char * file_name, int line, char * input_file_name, char * message ){
    report_error( file_name, line, input_file_name, message);
    fflush( stderr );
    return EXIT_FAILURE;
}

/// Determine the number of unsigned integers needed to hold a number of bits
/// @param num_bits Number of bits to be held in unsigned integers
/// @return Number of unsigned integers to hold num_bits
uint bits_to_num_uint( uint num_bits ){
    uint num_uint = num_bits / 32; 
    if(num_bits % 32 > 0)
      num_uint++;
    return num_uint;
}

/// Determine the unsigned integer holding the bit at position bit_number
/// @param bit_number The position of the bit
/// @return The unsigned integer index holding the bit at bit position bit_number
uint get_byte_index( uint bit_number ){
    return bit_number / 32;
}

/// Get an unsigned integer with 1 at bit position (bit_number) with 0 on rest of bits
/// @param bit_number Bit position to put a 1
/// @return Unsigned integer with 1 at bit position (bit_number)
uint get_mask( uint bit_number ){
    uint bit_position = 31 - (bit_number % 32); // which position in the byte to insert 1
    uint mask = 0x0001;
    mask <<= bit_position; 
    return mask;
}

/// Determine the output stream from the command line
/// @param argv Command line argument
/// @return Pointer to the stream specified in the command line arguments
FILE * get_output_stream( char ** argv ){
    FILE * fp;
    if(strcmp(argv[2], "-") == 0) // print
        fp = stdout;
    else 
        fp = fopen(argv[2], "wb");
    return fp;
}
