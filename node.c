#include "node.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *N = (Node *) calloc(1, sizeof(Node));
    if (N) {
        N->symbol = symbol;
        N->frequency = frequency;
    }
    N->left = NULL;
    N->right = NULL;
    return N;
}

void node_delete(Node **N) {
    if (*N) {
        free(*N);
        *N = NULL;
    }
    return;
}

Node *node_join(Node *left, Node *right) {
    Node *parent_node = node_create('$', left->frequency + right->frequency);
    parent_node->left = left;
    parent_node->right = right;
    return parent_node;
}

void node_print(Node *N) {
    printf("Symbol: %u\n", N->symbol);
    printf("Frequency: %lu\n", N->frequency);
}
