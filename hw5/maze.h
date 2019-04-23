/*
 * File: maze.h
 * 
 *   Declaration of a minecraft-style block maze data structure and library
 *     functions manipulating the maze. Feel free to add, remove, or modify
 *     these declarations to serve your algorithm.
 *  
 * Jose @ ShanghaiTech University
 *
 */

#ifndef _MAZE_H_
#define _MAZE_H_

#include <stdio.h>  /* FILE */
#include "node.h"

/* 
 * Structure of a minecraft-style block maze.
 *
 */
typedef struct maze_t
{
    int rows;           /* Number of rows. */
    int cols;           /* Number of cols. */
    node_t **nodes;     /* Array of node pointers. */
    node_t *start;      /* Start node pointer. */
    node_t *goal;       /* Goal node pointer. */
    int file;           /* Source file pointer. */
    int file_size;      /* Size of file */
    char* map;          /* Memory map address */
} maze_t;

/* Function prototypes. */
maze_t *maze_init (char *filename);
void maze_destroy (maze_t *m);
void maze_set_cell (maze_t *m, int x, int y, mark_t mark);
node_t *maze_get_cell (maze_t *m, int x, int y);
void maze_print_step (maze_t *m, node_t *n);
maze_t *maze_copy (maze_t *src);

#endif
