#include "huffman.h"
#include "defines.h"
#include "pq.h"
#include "node.h"
#include "io.h"
#include <stdlib.h>
#include "stack.h"

void help_code(Code c, Node *root, Code table[static ALPHABET]) {
    uint8_t random = 0;
    if (root) {
        if (root->left == NULL && root->right == NULL) {
            table[root->symbol] = c;
        } else {
            if (root->left != NULL) {
                code_push_bit(&c, 0);
                help_code(c, root->left, table);
                code_pop_bit(&c, &random);
            }
            if (root->right != NULL) {
                code_push_bit(&c, 1);
                help_code(c, root->right, table);
                code_pop_bit(&c, &random);
            }
        }
    }
    return;
}

Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *huffman_q = pq_create(ALPHABET);
    for (uint64_t count = 0; count < ALPHABET; count++) {
        if (hist[count] > 0) {
            Node *N = node_create(count, hist[count]);
            enqueue(huffman_q, N);
        }
    }
    while (pq_size(huffman_q) > 1) {
        Node *l, *r;
        dequeue(huffman_q, &l);
        dequeue(huffman_q, &r);
        Node *comb = node_join(l, r);
        enqueue(huffman_q, comb);
    }
    Node *root;
    dequeue(huffman_q, &root);
    pq_delete(&huffman_q);
    return root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();
    help_code(c, root, table);
}

void dump_tree(int outfile, Node *root) {
    uint8_t L = 'L';
    uint8_t I = 'I';
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (root->left == NULL && root->right == NULL) {
            write_bytes(outfile, &L, 1);
            write_bytes(outfile, &root->symbol, 1);
        } else {
            write_bytes(outfile, &I, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {

    Stack *tree_stack = stack_create(ALPHABET);
    for (uint16_t count = 0; count < nbytes; count++) {
        if (tree_dump[count] == 'L') {
            count += 1;
            Node *t = node_create(tree_dump[count], 0);
            stack_push(tree_stack, t);
        } else if (tree_dump[count] == 'I') {
            Node *R;
            Node *L;
            stack_pop(tree_stack, &R);
            stack_pop(tree_stack, &L);
            Node *P;
            P = node_join(L, R);
            stack_push(tree_stack, P);
        }
    }
    Node *root_return;
    stack_pop(tree_stack, &root_return);
    stack_delete(&tree_stack);
    return root_return;
}

void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        node_delete(root);
    }
}
