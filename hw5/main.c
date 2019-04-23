/*
 * File: main.c
 * 
 *   Main body of A* path searching algorithm on a block maze. The maze is
 *     given in a source file, whose name is put as a command-line argument.
 *     Then it should perform an A* search on the maze and prints the steps
 *     along the computed shortest path back to the file.
 *
 *     * Heuristic function chosen as the "Manhattan Distance":
 *
 *         heuristic(n1, n2) = |n1.x - n2.x| + |n1.y - n2.y|
 *
 *     * The whole procedure, including I/O and computing, will be time-
 *         ticked. So you are allowed to modify and optimize everything in
 *         this file and in all the libraries, as long as it satisfies:
 *
 *         1. It performs an A* path searching algorithm on the given maze.
 *
 *         2. It computes one SHORTEST (exactly optimal) path and prints the
 *              steps along the shortest path back to file, just as the
 *              original version.
 *
 *         3. Compiles with the given "Makefile". That means we are using
 *              (and only manually using) "pthread" for parallelization.
 *              Further parallelization techniques, such as OpenMP and SSE,
 *              are not required (and not allowed).
 *
 *         4. If there are multiple shortest paths, any one of them will be
 *              accepted. Please make sure you only print exactly one valid
 *              path to the file.
 *  
 * Jose @ ShanghaiTech University
 *
 */

#include <stdlib.h>     /* NULL */
#include <assert.h>     /* assert */
#include "heap.h"
#include "node.h"
#include "maze.h"
#include "utils.h"
#include "compass.h"    /* The heuristic. */
#include <pthread.h> 

/* Local helper functions. */
static node_t *fetch_neighbour (maze_t *m, node_t *n, int direction);

/*
 * Entrance point. Time ticking will be performed on the whole procedure,
 *   including I/O. Parallelize and optimize as much as you can.
 *
 */

maze_t *maze;
heap_t *openset_f, *openset_b;
long u = -1;
pthread_mutex_t maze_lock, openset_f_lock, openset_b_lock;


void* astar_forward();
void* astar_backward();

int
main (int argc, char *argv[])
{
    assert(argc == 2);  /* Must have given the source file name. */

    /* Initializations. */
    timer_start();
    maze = maze_init(argv[1]);
    maze->start->gs = 0;
    maze->start->fs = heuristic(maze->start, maze->goal);
    maze->goal->gs = 0;
    maze->goal->fs = heuristic(maze->goal, maze->start);
    timer_end();

    /* path search */
    timer_start();

    astar_forward();
    timer_end();

    /* Print the steps back. */
    timer_start();
    maze_print_step(maze, maze->goal->parent);
    timer_end();

    /* Free resources and return. */
    
    maze_destroy(maze);
    return 0;
}


void* astar_forward()
{
    openset_f = heap_init();
    heap_insert(openset_f, maze->start);

    /* Loop and repeatedly extracts the node with the highest f-score to
       process on. */
    while (openset_f->size > 0) {
        int direction;
        node_t *cur = heap_extract(openset_f);
        
        if (cur->mark == GOAL)  /* Goal point reached. */
            break;

        cur->closed = true;

        /* Check all the neighbours. Since we are using a block maze, at most
           four neighbours on the four directions. */
        for (direction = 0; direction < 4; ++direction) {
            node_t *n = fetch_neighbour(maze, cur, direction);
            
            if (n == NULL || n->mark == WALL || n->closed)
                continue;   /* Not valid, or closed already. */

            if (n->opened && cur->gs + 1 >= n->gs)
                continue;   /* Old node met, not getting shorter. */

            /* Passing through CUR is the shortest way up to now. Update. */
            n->parent = cur;
            n->gs = cur->gs + 1;
            n->fs = n->gs + heuristic(n, maze->goal);
            if (!n->opened) {   /* New node discovered, add into heap. */
                n->opened = true;
                heap_insert(openset_f, n);
            } else              /* Updated old node. */
                heap_update(openset_f, n);
        }
    }

    heap_destroy(openset_f);

    return NULL;
}

void* astar_backward()
{
    openset_b = heap_init();
    heap_insert(openset_b, maze->goal);

    /* Loop and repeatedly extracts the node with the highest f-score to
       process on. */
    while (openset_b->size > 0) {
        int direction;
        node_t *cur = heap_extract(openset_b);
        
        if (cur->mark == START)  /* Goal point reached. */
            break;

        cur->closed = true;

        /* Check all the neighbours. Since we are using a block maze, at most
           four neighbours on the four directions. */
        for (direction = 0; direction < 4; ++direction) {
            node_t *n = fetch_neighbour(maze, cur, direction);
            
            if (n == NULL || n->mark == WALL || n->closed)
                continue;   /* Not valid, or closed already. */

            if (n->opened && cur->gs + 1 >= n->gs)
                continue;   /* Old node met, not getting shorter. */

            /* Passing through CUR is the shortest way up to now. Update. */
            n->parent = cur;
            n->gs = cur->gs + 1;
            n->fs = n->gs + heuristic(n, maze->start);
            if (!n->opened) {   /* New node discovered, add into heap. */
                n->opened = true;
                heap_insert(openset_b, n);
            } else              /* Updated old node. */
                heap_update(openset_b, n);
        }
    }

    heap_destroy(openset_b);

    return NULL;
}


/*
 * Fetch the neighbour located at direction DIRECTION of node N, in the
 *   maze M. Returns pointer to the neighbour node.
 *
 */
static node_t *
fetch_neighbour (maze_t *m, node_t *n, int direction)
{
    switch (direction) {
        case 0: return maze_get_cell(m, n->x, n->y - 1);
        case 1: return maze_get_cell(m, n->x + 1, n->y);
        case 2: return maze_get_cell(m, n->x, n->y + 1);
        case 3: return maze_get_cell(m, n->x - 1, n->y);
    }
    return NULL;
}
