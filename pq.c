#include "pq.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

struct PriorityQueue {
    uint32_t head;
    uint32_t size;
    uint32_t capacity;
    Node **items;
};

PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->head = 0;
        q->size = 0;
        q->capacity = capacity;
        q->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}

void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
    return;
}

bool pq_empty(PriorityQueue *q) {
    if (q->size == 0) {
        return true;
    }
    return false;
}

bool pq_full(PriorityQueue *q) {
    if (q->size == q->capacity) {
        return true;
    }
    return false;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->size;
}

bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->items[q->size] = n;
    q->size += 1;
    Node *temp;
    for (uint64_t i = 1, j, temp_freq; i < (uint64_t) q->size; i++) {
        j = i;
        temp = q->items[i]; // type Node
        temp_freq = q->items[i]->frequency; // type uint64_t
        while (j > 0 && temp_freq < q->items[j - 1]->frequency) {
            q->items[j] = q->items[j - 1];
            j -= 1;
        }
        q->items[j] = temp;
    }
    return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    *n = q->items[q->head];
    for (uint32_t count = 0; count < q->size - 1; count++) {
        q->items[count] = q->items[count + 1];
    }
    q->size -= 1;
    return true;
}

void pq_print(PriorityQueue *q) {
    for (uint32_t count = 0; count < q->size; count++) {
        node_print(q->items[count]);
    }
}
