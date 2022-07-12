//
// file: packman.c
// description: Main program to encode or decode files
//
// @author Daniel Tregea
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packman_utils.h"
#include "encode.h"
#include "decode.h"
#include "utilities.h"

Tree_node huffman_tree; // Huffman tree read or written to files
char * lut[256]; // Look up table generated from Huffman tree
uint * encoded_binary; // Sequence of binary read or written to files

/// Main function to either encode files or decode packman encoded files
/// @param argc Number of command line arguments
/// @param argv Array of command line arguments containing input and output file
/// @return EXIT_FAILURE on encode/decode failure, or EXIT_SUCCESS on encode/decode success
int main( int argc, char * argv[] ){

    if(argc != 3){
        fprintf(stderr, "usage: packman firstfile secondfile\n");
        return EXIT_FAILURE;
    }

    char * input_file = argv[1];
    int encode;
    FILE * fp;
    
    fp = fopen(input_file, "rb");
    if (fp == NULL){
        return handle_error(__FILE__, __LINE__, input_file, "NoSuchFile");
    }
    
    // Determine whether to encode or decode depending on magic number
    encode = find_packman_magic(fp);
    if(encode < 0){
        fclose(fp);
        return handle_error(__FILE__, __LINE__, input_file, "File has no contents");
    }
    fclose(fp);
        
    if(encode){
    
        fp = fopen( input_file, "rb" );
        if (fp == NULL)
            return handle_error(__FILE__, __LINE__, input_file, "NoSuchFile");
            
        uint * frequencies =  calloc(256, sizeof(uint));
        uchar data[1];
        uint num_items = 0, num_unique = 0;

        // Read in symbol frequencies
        while(fread(data, sizeof(uchar), 1, fp) > 0) {
            num_items++;
            frequencies[data[0]]++;
            if(frequencies[data[0]] == 1)
                num_unique++;
        }
        
        fclose(fp);
        
        // Build frequency heap
        Heap frequency_heap = hdt_create(num_unique, compare_node_min, print_node);
        for(int i = 0; i < 256; i++){
            if(frequencies[i] > 0){
                Tree_node node = create_tree_node(i, frequencies[i], 0);
                hdt_insert_item(frequency_heap,(void*) node);
            }
        }
        
        // Build huffman tree
        huffman_tree = heap_to_huffman(frequency_heap);
        
        // Build look up table
        populate_lut(lut, huffman_tree, "", "");
        
        fp = fopen(input_file, "r");
        if (fp == NULL)
            return handle_error(__FILE__, __LINE__, input_file, "NoSuchFile");
        
        // Read file and create sequence of bits to write to output file
        uint * bits;
        uint num_bits = 0;
        while(fread(data, sizeof(uchar), 1, fp) > 0){ // Iterate one character at a time
            char * code = lut[data[0]]; // get character code from look up table
            uchar code_length = strlen(code); // get length of code
            
            if(num_bits > 0) // increase size of array to fit in new code
                bits = realloc(bits, (code_length + num_bits) * sizeof(uint));
            else
                bits = malloc(code_length * sizeof(uint));
                
            for(uint i = num_bits, j = 0; i < num_bits + code_length; i++, j++) // append each digit of code to the array
                bits[i] = (code[j] == '0' ? 0 : 1);
            
            num_bits += code_length; // length of code is added onto number of bits
        }
        
        fclose(fp);
        
        // Pack character array of code bits into unsigned integers
        uint num_uint = bits_to_num_uint(num_bits);
        encoded_binary = pack_bits(bits, num_bits, num_uint);

        fp = get_output_stream(argv);
        
        if (fp == NULL)
            return handle_error(__FILE__, __LINE__, input_file, "Can't Write to File");
        
        // write magic number, huffman tree decoding, and binary symbol codes
        write_magic(fp);
        write_tree(fp, huffman_tree);
        uint * num_bits_array = malloc(sizeof(uint)); // fwrite needs pointer to integer
        num_bits_array[0] = num_bits;
        fwrite(num_bits_array, sizeof(uint), 1, fp);
        fwrite(encoded_binary, sizeof(uint), num_uint, fp);
        
        // free everything
        hdt_destroy(frequency_heap);
        free(frequencies);
        free(bits);
        free(num_bits_array);

        
    } else { // Decode
        
        fp = fopen( input_file, "rb" );
        if (fp == NULL)
            return handle_error(__FILE__, __LINE__, input_file, "NoSuchFile");

        // Read huffman tree
        find_packman_magic(fp);
        huffman_tree = read_tree(fp);
        if(huffman_tree == NULL){
            fclose(fp);
            return handle_error(__FILE__, __LINE__, input_file, "Binary Tree Not Found");
        }
        // Read number of bits
        uint num_bits_array[1];
        if(fread(num_bits_array, sizeof(uint), 1, fp) == 0){
            free_tree(huffman_tree);
            fclose(fp);
            return handle_error(__FILE__, __LINE__, input_file, "No data found after binary tree");
        }
        
        // Generate look up table
        uint num_bits = num_bits_array[0];
        populate_lut(lut, huffman_tree, "", "");

        // Get number of unsigned integers needed to hold all the symbol code bits
        uint num_uint = bits_to_num_uint(num_bits);
        
        encoded_binary = calloc(num_uint, sizeof(uint));
        
        // Read in the symbol code bits
        fread(encoded_binary, sizeof(uint), num_uint, fp);
        fclose(fp);
        
        // Get each bit from the unsigned integers read and put into a char array
        char ** bits = uint_to_str_bits(num_bits, encoded_binary);
        
        // Determine output stream
        fp = get_output_stream(argv);
        
        if (fp == NULL){
            free(bits);
            free(encoded_binary);
            free_tree(huffman_tree);
            return handle_error(__FILE__, __LINE__, input_file, "Can't Write to File");
        }
        
        // Decode bits and write to output stream
        if(!write_bits(bits, num_bits, lut, fp))
            return handle_error(__FILE__, __LINE__, input_file, "Realloc failure");
        
        free(bits);
    }
    
    // Free all memory
    fclose(fp);
    free(encoded_binary);
    free_tree(huffman_tree);
    free_lut(lut);

    return EXIT_SUCCESS;
}
