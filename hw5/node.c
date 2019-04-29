/*
 * File: node.c
 * 
 *   Implementation of library functions manipulating a block cell node. Feel
 *     free to add, remove, or modify these library functions to serve your
 *     algorithm.
 *  
 * Jose @ ShanghaiTech University
 *
 */

#include <stdlib.h>     /* NULL, malloc, free */
#include <limits.h>     /* INT_MAX */
#include "node.h"

/* 
 * Initialize a node whose position is recorded at (X, Y) with type MARK.
 *   Returns the pointer to the new node.
 *
 */
node_t *
node_init (int x, int y, mark_t mark)
{
    node_t *n = malloc(sizeof(node_t));
    n->x = x;
    n->y = y;
    n->gs_f = INT_MAX;
    n->fs_f = INT_MAX;
    n->gs_b = INT_MAX;
    n->fs_b = INT_MAX;
    n->mark = mark;
    n->heap_id_f = 0;
    n->heap_id_b = 0;
    n->opened_f = false;
    n->opened_b = false;
    n->closed = false;
    n->parent_f = NULL;
    n->parent_b = NULL;
    return n;
}

/* 
 * Delete the memory occupied by the node N.
 *
 */
void
node_destroy (node_t *n)
{
    free(n);
}

/* 
 * Defines the comparison method between nodes, that is, comparing their
 *   A* f-scores.
 *
 */
bool
node_less_f (node_t *n1, node_t *n2)
{
    return n1->fs_f < n2->fs_f;
}

bool
node_less_b (node_t *n1, node_t *n2)
{
    return n1->fs_b < n2->fs_b;
}
