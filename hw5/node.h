/*
 * File: node.h
 * 
 *   Declaration of a block cell node data structure and library functions
 *     manipulating the node. Feel free to add, remove, or modify these
 *     declarations to serve your algorithm.
 *  
 * Jose @ ShanghaiTech University
 *
 */

#ifndef _NODE_H_
#define _NODE_H_

/* 
 * Introduce the "bool" type. Though I really love C/C++, I would still say
 *   it is stupid for not even defining a standard "bool" type in ANSI C
 *   standard.
 *
 */
typedef int bool;
#define true  (1)
#define false (0)

/* 
 * Explicitly defines the four different node types.
 *   START: start point
 *   GOAL:  goal point
 *   WALL:  wall block, cannot step on / move across
 *   NONE:  normal blank point
 *
 */
typedef enum {START, GOAL, WALL, NONE} mark_t;

/* 
 * Structure of a block cell node.
 *
 */
typedef struct node_t
{
    int x;          /* X coordinate, starting from 0. */
    int y;          /* Y coordinate, starting from 0. */
    int gs_f;         /* A* g-score. */
    int gs_b;
    int fs_f;         /* A* f-score. */
    int fs_b;
    mark_t mark;    /* Node type. */
    int heap_id_f;    /* Position on min heap, used by updating. */
    int heap_id_b;
    bool opened_f;    /* Has been discovered? */
    bool opened_b;
    bool closed;    /* Has been closed? */
    struct node_t *parent_f;  /* Parent node along the path. */
    struct node_t *parent_b;
} node_t;

/* Function prototypes. */
node_t *node_init (int x, int y, mark_t mark);
void node_destroy (node_t *n);
bool node_less_f (node_t *n1, node_t *n2);
bool node_less_b (node_t *n1, node_t *n2);

#endif
