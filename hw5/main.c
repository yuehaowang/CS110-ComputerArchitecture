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
#include <pthread.h> 
#include <limits.h>     /* INT_MAX */
#include "heap.h"
#include "node.h"
#include "maze.h"
#include "compass.h"    /* The heuristic. */

/*
 * Entrance point. Time ticking will be performed on the whole procedure,
 *   including I/O. Parallelize and optimize as much as you can.
 *
 */

int u = INT_MAX, F1, F2, is_finished;
pthread_mutex_t meet_pt_lock;

node_t *meet_point = NULL;


void* astar_forward();
void* astar_backward();


int
main (int argc, char *argv[])
{
    pthread_t forward_thread;
    pthread_t backward_thread;
    maze_t *maze;
    
    assert(argc == 2);  /* Must have given the source file name. */

    /* init threads and locks */
    pthread_mutex_init(&meet_pt_lock, NULL);

    /* Initializations. */
    maze = maze_init(argv[1]);

    maze->start->gs_f = 0;
    maze->start->fs_f = heuristic(maze->start, maze->goal);

    maze->goal->gs_b = 0;
    maze->goal->fs_b = heuristic(maze->goal, maze->start);

    /*openset_f = heap_init();
    heap_insert_f(openset_f, maze->start);
    openset_b = heap_init();
    heap_insert_b(openset_b, maze->goal);*/

    /* path search */
    /*astar_forward();*/

    pthread_create(&forward_thread, NULL, &astar_forward, &maze);
    pthread_create(&backward_thread, NULL, &astar_backward, &maze);

    pthread_join(forward_thread, NULL);
    pthread_join(backward_thread, NULL);

    /* Print the steps back. */
    /*printf("%d %d\n", meet_point->x, meet_point->y);*/
    if (meet_point != NULL) {
        maze_print_step(maze, meet_point);
    }
    /* Free resources and return. */
    pthread_mutex_destroy(&meet_pt_lock);

    maze_destroy(maze);
    
    return 0;
}


void* astar_forward(void** m)
{
    heap_t *openset_f;
    node_t *cur = NULL;
    maze_t *maze = *(maze_t**)m;

    openset_f = heap_init();
    heap_insert_f(openset_f, maze->start);

    /* Loop and repeatedly extracts the node with the highest f-score to
       process on. */
    while (!is_finished) {
        int direction;

        cur = heap_extract_f(openset_f);

        if (cur->mark == GOAL)  /* Goal point reached. */
            break;

        if (cur->closed != true) {
            if (cur->fs_f < u && (cur->gs_f + F2 - heuristic(cur, maze->start) < u)) {
                /* Check all the neighbours. Since we are using a block maze, at most
                    four neighbours on the four directions. */
                node_t *neighbours[4];
                neighbours[0] = maze->nodes[maze->cols * cur->x + cur->y - 1];
                neighbours[1] = maze->nodes[maze-> cols * (cur->x + 1) + cur->y];
                neighbours[2] = maze->nodes[maze-> cols * cur->x + cur->y + 1];
                neighbours[3] = maze->nodes[maze-> cols * (cur->x - 1) + cur->y];
                for (direction = 0; direction < 4; ++direction) {
                    node_t *n = neighbours[direction];
                    
                    if (n == NULL || n->mark == WALL || n->closed) {
                        continue;   /* Not valid, or closed already. */
                    }

                    if (n->opened_f && cur->gs_f + 1 >= n->gs_f){
                        continue;   /* Old node met, not getting shorter. */
                    }
                    /* Passing through CUR is the shortest way up to now. Update. */
                    n->parent_f = cur;
                    n->gs_f = cur->gs_f + 1;
                    n->fs_f = n->gs_f + heuristic(n, maze->goal);
                    if (!n->opened_f) {   /* New node discovered, add into heap. */
                        n->opened_f = true;
                        heap_insert_f(openset_f, n);
                    } else {              /* Updated old node. */
                        heap_update_f(openset_f, n);
                    }

                    if (n->gs_f != INT_MAX && n->gs_b != INT_MAX && n->gs_f + n->gs_b < u) {
                        pthread_mutex_lock(&meet_pt_lock);
                        if (n->gs_f != INT_MAX && n->gs_b != INT_MAX && n->gs_f + n->gs_b < u) {
                            u = n->gs_f + n->gs_b;
                            meet_point = n;
                        }
                        pthread_mutex_unlock(&meet_pt_lock);
                    }
                }
            }

            cur->closed = true;
        }

        if (openset_f->size > 0) {
            F1 = openset_f->nodes[1]->fs_f;
        } else {
            is_finished = 1;
        }
    }

    heap_destroy(openset_f);

    return NULL;
}

void* astar_backward(void** m)
{
    heap_t *openset_b;
    node_t *cur = NULL;
    maze_t *maze = *(maze_t**)m;

    openset_b = heap_init();
    heap_insert_b(openset_b, maze->goal);

    /* Loop and repeatedly extracts the node with the highest f-score to
       process on. */
    while (!is_finished) {
        int direction;

        cur = heap_extract_b(openset_b);

        if (cur->mark == START)  /* Goal point reached. */
            break;

        if (cur->closed != true) {
            if (cur->fs_b < u && (cur->gs_b + F1 - heuristic(cur, maze->goal) < u)) {
                /* Check all the neighbours. Since we are using a block maze, at most
                    four neighbours on the four directions. */
                node_t *neighbours[4];
                neighbours[0] = maze->nodes[maze->cols * cur->x + cur->y - 1];
                neighbours[1] = maze->nodes[maze-> cols * (cur->x + 1) + cur->y];
                neighbours[2] = maze->nodes[maze-> cols * cur->x + cur->y + 1];
                neighbours[3] = maze->nodes[maze-> cols * (cur->x - 1) + cur->y];
                for (direction = 0; direction < 4; ++direction) {
                    node_t *n = neighbours[direction];
                    
                    if (n == NULL || n->mark == WALL || n->closed) {
                        continue;   /* Not valid, or closed already. */
                    }

                    if (n->opened_b && cur->gs_b + 1 >= n->gs_b) {
                        continue;   /* Old node met, not getting shorter. */
                    }

                    /* Passing through CUR is the shortest way up to now. Update. */
                    n->parent_b = cur;
                    n->gs_b = cur->gs_b + 1;
                    n->fs_b = n->gs_b + heuristic(n, maze->start);
                    if (!n->opened_b) {   /* New node discovered, add into heap. */
                        n->opened_b = true;
                        heap_insert_b(openset_b, n);
                    } else {              /* Updated old node. */
                        heap_update_b(openset_b, n);
                    }

                    if (n->gs_b != INT_MAX && n->gs_f != INT_MAX && n->gs_b + n->gs_f < u) {
                        pthread_mutex_lock(&meet_pt_lock);
                        if (n->gs_b != INT_MAX && n->gs_f != INT_MAX && n->gs_b + n->gs_f < u) {
                            u = n->gs_b + n->gs_f;
                            meet_point = n;
                        }
                        pthread_mutex_unlock(&meet_pt_lock);
                    }
                }
            }

            cur->closed = true;
        }
        
        if (openset_b->size > 0) {
            F2 = openset_b->nodes[1]->fs_b;
        } else {
            is_finished = 1;
        }
    }

    heap_destroy(openset_b);

    return NULL;
}

