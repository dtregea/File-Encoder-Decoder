//
// file: decode.h
// description: Definition file for utility functions for decoding a file
//
// @author Daniel Tregea
//

#ifndef DECODE_H
#define DECODE_H
#include "packman_utils.h"

/// Get each bit from an unsigned integer array to be put in a string array
/// @param num_bits Number of bits in encoded_binary
/// @param encoded_binary Array of unsigned integers
/// @return Array of strings each holding a bit from encoded_binary
char ** uint_to_str_bits( uint num_bits, uint * encoded_binary );

/// Decode and write each bit from a string array
/// @param bits String array holding binary sequence to be decoded
/// @param num_bits Number of bits in bit array
/// @param lut Look up table
/// @param fp Output stream to write to
/// @return Method success or realloc failure
int write_bits( char ** bits, uint num_bits, char ** lut, FILE * fp );
#endif