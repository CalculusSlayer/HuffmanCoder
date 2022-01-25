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

// Start of main function for decode algorithm.
// Use getopt to parse through arguments.
int main(int argc, char **argv) {
    bool no_input = true;
    bool h_flag = false;
    bool v_flag = false;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
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

    // If -h flag is raised print help message.
    if (h_flag == true) {
        printf("SYNOPSIS\n");
        printf("  A Huffman decoder.\n");
        printf("  Decompresses a file using the Huffman coding algorithm.\n");
        printf("\n");
        printf("Usage\n");
        printf("  ./decode [-h] [-i infile] [-o outfile]\n");
        printf("\n");
        printf("OPTIONS\n");
        printf("  -h             Program usage and help.\n");
        printf("  -v             Print compression statistics.\n");
        printf("  -i infile      Input file to decompress.\n");
        printf("  -o outfile     Output of decompressed data.\n");
    } else {
        // Step 1 Read header from infile
        Header h = { 0, 0, 0, 0 };
        read_bytes(infile, (uint8_t *) &h, sizeof(h));
        if (h.magic != MAGIC) {
            fprintf(stderr, "bad magic");
            close(infile);
            close(outfile);
            exit(EXIT_FAILURE);
        }
        // Step 2 Set the permissions using fchmod
        fchmod(outfile, h.permissions);

        // Step 3 Rebuilding the tree
        uint8_t tree_array[h.tree_size];
        read_bytes(infile, tree_array, h.tree_size);
        Node *root_node = rebuild_tree(h.tree_size, tree_array);
        Node *r_copy = root_node;
        uint64_t a = 0;
        uint8_t bit = 0;

        // Step 4 Tree Walk Algorithm
        while (a < h.file_size) {
            read_bit(infile, &bit);
            if (bit == 0) {
                root_node = root_node->left;
            } else {
                root_node = root_node->right;
            }
            if (root_node->left == NULL && root_node->right == NULL) {
                a++;
                write_bytes(outfile, &root_node->symbol, 1);
                root_node = r_copy;
            }
        }
        // Print stats if v flag is raised.
        if (v_flag) {
            double result = 100 * (1 - bytes_read / (double) h.file_size);
            fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", h.file_size);
            fprintf(stderr, "Decompressed file size: %" PRIu64 " bytes\n", bytes_read);
            fprintf(stderr, "Space saving: %.2lf%c\n", result, 37);
        }
        delete_tree(&root_node);
    }
    // Step 5 Close infile and outfile
    close(infile);
    close(outfile);

    return 0;
}
