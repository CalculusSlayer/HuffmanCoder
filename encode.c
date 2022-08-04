#include "defines.h"
#include "stack.h"
#include "io.h"
#include "pq.h"
#include "node.h"
#include "huffman.h"
#include "header.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define OPTIONS "hvi:o:"

// Start of main function.
// Takes program arguments and process with getopt.
int main(int argc, char **argv) {
    bool no_input = true;
    bool h_flag = false;
    bool v_flag = false;
    int infile = STDIN_FILENO; // standard input stream
    int outfile = STDOUT_FILENO; // standard output stream
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        no_input = false;
        switch (opt) {

        case 'h': h_flag = true; break;
        case 'v': v_flag = true; break;

        case 'i':
            infile = open(optarg, O_RDONLY); 
            if (infile == -1) {
                fprintf(stderr, "Infile error");
                exit(EXIT_FAILURE);
            }
            break;

        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); 
            if (outfile == -1) {
                fprintf(stderr, "Outfile error");
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    // Standard Error Message if h flag is raised
    if (h_flag == true) {
        printf("SYNOPSIS\n");
        printf("  A Huffman encoder.\n");
        printf("  Compresses a file using the Huffman coding algorithm.\n");
        printf("\n");
        printf("USAGE\n");
        printf("  ./encode [-h] [-i infile] [-o outfile]\n");
        printf("\n");
        printf("OPTIONS\n");
        printf("  -h            Program usage and help.\n");
        printf("  -v            Print compression statistics.\n");
        printf("  -i infile     Input file to compress.\n");
        printf("  -o outfile    Output of compressed data.\n");
    } 

    else {
        // Steps 1-2 Making and initializing histogram
        // Increment first and last symbol by 1.
        uint64_t histogram[ALPHABET] = { 0 };
        uint32_t unique_syms = 0;
        uint8_t buffer[BLOCK] = { 0 };
        histogram[0]++;
        histogram[ALPHABET - 1]++;
        int temp;
        while ((temp = read_bytes(infile, buffer, BLOCK)) > 0) {
            for (int count = 0; count < temp; count++) {
                uint8_t current_symbol;
                current_symbol = buffer[count];
                histogram[current_symbol]++;
            }
        }
        // Step 3 Making tree from histogram
        Node *root21 = build_tree(histogram);
        Code table[ALPHABET] = { {0} };
        for (int count = 0; count < ALPHABET; count++) {
            if (histogram[count] > 0) {
                unique_syms++;
            }
        }
        // Step 4 Constructing Code table
        build_codes(root21, table);
        struct stat statbuff;
        fstat(infile, &statbuff);

        // Step 5 Making a header 
        fchmod(outfile, statbuff.st_mode); // file permissions
        Header h = { 0, 0, 0, 0 };
        h.magic = MAGIC;
        h.permissions = statbuff.st_mode;
        h.tree_size = 3 * unique_syms - 1;
        h.file_size = statbuff.st_size;

        // Step 6 Writing header to outfile
        write_bytes(outfile, (uint8_t *) &h, sizeof(h));

        // Step 7 Write constructed huffman tree
        dump_tree(outfile, root21);
        lseek(infile, 0, SEEK_SET);

        // Step 8 Write code for each symbol
        while ((temp = read_bytes(infile, buffer, BLOCK)) > 0) {
            for (int count = 0; count < temp; count++) {
                write_code(outfile, &table[buffer[count]]);
            }
        }
        flush_codes(outfile);
        // Print stats if v flag is raised.
        if (v_flag) {
            double result = 100 * (1 - bytes_written / (double) h.file_size);
            fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", h.file_size);
            fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_written);
            fprintf(stderr, "Save spacing: %.2lf%c\n", result, 37);
        }
        delete_tree(&root21);

        // Step 9 Close infile and outfile
        close(infile);
        close(outfile);
    }

    return 0;
}
