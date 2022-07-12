//
// file: decode.c
// description: Implementation file for utility functions for decoding a file
//
// @author Daniel Tregea
//

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "decode.h"

/// Get each bit from an unsigned integer array to be put in a string array
/// @param num_bits Number of bits in encoded_binary
/// @param encoded_binary Array of unsigned integers
/// @return Array of strings each holding a bit from encoded_binary
char ** uint_to_str_bits( uint num_bits, uint * encoded_binary ){
    char ** bits = malloc(num_bits * sizeof(char *));
    for(uint i = 0; i < num_bits; i++)
        bits[i] = (encoded_binary[get_byte_index(i)] & get_mask(i)) != 0 ? "1" : "0";
    return bits;
}

/// Decode and write each bit from a string array
/// @param bits String array holding binary sequence to be decoded
/// @param num_bits Number of bits in bit array
/// @param lut Look up table
/// @param fp Output stream to write to
/// @return Method success or realloc failure
int write_bits( char ** bits, uint num_bits, char ** lut, FILE * fp ){
    // Read each char bit and write symbols as their code is read
    char * cur_code = NULL;
    for(uint i = 0; i < num_bits; i++){
        // Add the new bit to the current code
        if(cur_code == NULL){
            cur_code = malloc(2 * sizeof(char));
            strcpy(cur_code, bits[i]);
        } else {
            cur_code = realloc(cur_code, strlen(cur_code) + 2);
            if(cur_code == NULL)
                return -1;
            cur_code = strcat(cur_code, bits[i]);
        }
        
        // Determine if the newly added bit generates a code from the look up table
        for(int j = 0; j < 256; j++){
            if(lut[j] != NULL && strcmp(cur_code, lut[j]) == 0){
                // If a symbols code is generated, write it to the file
                char * data = malloc(sizeof(char));
                data[0] = (char) j;
                fwrite(data, sizeof(char), 1, fp);
                free(data);
                free(cur_code);
                cur_code = NULL; // sanity check
                j = 256; // End loop on symbols code being found
            }
        }
    }
    return 1;
}