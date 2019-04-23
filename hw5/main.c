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
#include "utils.h"
#include "compass.h"    /* The heuristic. */

/* Local helper functions. */
static node_t *fetch_neighbour (maze_t *m, node_t *n, int direction);

/*
 * Entrance point. Time ticking will be performed on the whole procedure,
 *   including I/O. Parallelize and optimize as much as you can.
 *
 */

maze_t *maze_f, *maze_b;
heap_t *openset_f, *openset_b;
long u = INT_MAX;

pthread_t forward_thread;
pthread_t backward_thread;
pthread_mutex_t maze_lock, openset_f_lock, openset_b_lock;


void* astar_forward();
void* astar_backward();


int
main (int argc, char *argv[])
{
    node_t *meet_point_f;
    node_t *meet_point_b;

    assert(argc == 2);  /* Must have given the source file name. */

    /* init threads and locks */
    pthread_mutex_init(&maze_lock, NULL);
    pthread_mutex_init(&openset_f_lock, NULL);
    pthread_mutex_init(&openset_b_lock, NULL);

    /* Initializations. */
    timer_start();

    maze_f = maze_init(argv[1]);
    maze_b = maze_copy(maze_f);

    maze_f->start->gs = 0;
    maze_f->start->fs = heuristic(maze_f->start, maze_f->goal);

    maze_b->goal->gs = 0;
    maze_b->goal->fs = heuristic(maze_b->goal, maze_b->start);

    openset_f = heap_init();
    heap_insert(openset_f, maze_f->start);
    openset_b = heap_init();
    heap_insert(openset_b, maze_b->goal);

    timer_end();

    /* path search */
    timer_start();
    pthread_create(&forward_thread, NULL, &astar_forward, NULL);
    pthread_create(&backward_thread, NULL, &astar_backward, NULL);
    pthread_join(forward_thread, (void**)&meet_point_f);
    pthread_join(backward_thread, (void**)&meet_point_b);
    timer_end();

    /* Print the steps back. */
    timer_start();
    printf("meet point f: %d %d\n", meet_point_f->x, meet_point_f->y);
    printf("meet point b: %d %d\n", meet_point_b->x, meet_point_b->y);
    meet_point_b = maze_b->nodes[meet_point_f->x * maze_b->cols + meet_point_f->y];
    maze_print_step(maze_f, meet_point_f);
    maze_print_step(maze_b, meet_point_b);
    timer_end();

    /* Free resources and return. */
    pthread_mutex_destroy(&maze_lock);
    pthread_mutex_destroy(&openset_f_lock);
    pthread_mutex_destroy(&openset_b_lock);

    maze_destroy(maze_f);
    maze_destroy(maze_b);
    heap_destroy(openset_f);
    heap_destroy(openset_b);
    return 0;
}


void* astar_forward()
{
    node_t *cur = NULL;
    /* Loop and repeatedly extracts the node with the highest f-score to
       process on. */
    while (openset_f->size > 0) {
        int direction;
        int k_f, k_b;
        node_t* cur_temp;

        pthread_mutex_lock(&openset_f_lock);
        cur = heap_extract(openset_f);
        pthread_mutex_unlock(&openset_f_lock);
        
        if (cur->mark == GOAL)  /* Goal point reached. */
            break;

        cur->closed = true;

        pthread_mutex_lock(&maze_lock);
        cur_temp = maze_b->nodes[cur->x * maze_b->cols + cur->y];
        if (cur_temp -> closed == true) {
            u = (u > (cur->gs + cur_temp->gs)) ? (cur->gs + cur_temp->gs) : u;
            k_f = openset_f->nodes[1]->fs;
            k_b = openset_b->nodes[1]->fs;

            if ((k_f > k_b ? k_f : k_b) >= u) {
                pthread_mutex_unlock(&maze_lock);
                break;
            }
        }
        pthread_mutex_unlock(&maze_lock);

        /* Check all the neighbours. Since we are using a block maze, at most
           four neighbours on the four directions. */
        for (direction = 0; direction < 4; ++direction) {
            node_t *n = fetch_neighbour(maze_f, cur, direction);
            
            if (n == NULL || n->mark == WALL || n->closed)
                continue;   /* Not valid, or closed already. */

            if (n->opened && cur->gs + 1 >= n->gs)
                continue;   /* Old node met, not getting shorter. */

            /* Passing through CUR is the shortest way up to now. Update. */
            n->parent = cur;
            n->gs = cur->gs + 1;
            n->fs = n->gs + heuristic(n, maze_f->goal);
            if (!n->opened) {   /* New node discovered, add into heap. */
                n->opened = true;
                heap_insert(openset_f, n);
            } else              /* Updated old node. */
                heap_update(openset_f, n);
        }
    }

    return (void*)cur;
}

void* astar_backward()
{
    node_t *cur = NULL;
    /* Loop and repeatedly extracts the node with the highest f-score to
       process on. */
    while (openset_b->size > 0) {
        int direction;
        int k_f, k_b;
        node_t* cur_temp;

        pthread_mutex_lock(&openset_b_lock);
        cur = heap_extract(openset_b);
        pthread_mutex_unlock(&openset_b_lock);
        
        if (cur->mark == START)  /* Goal point reached. */
            break;

        cur->closed = true;

        pthread_mutex_lock(&maze_lock);
        cur_temp = maze_f->nodes[cur->x * maze_f->cols + cur->y];
        if (cur_temp -> closed == true) {
            node_t* cur_temp = maze_f->nodes[cur->x * maze_f->cols + cur->y];
            u = (u > (cur->gs + cur_temp->gs)) ? (cur->gs + cur_temp->gs) : u;
            k_f = openset_f->nodes[1]->fs;
            k_b = openset_b->nodes[1]->fs;

            if ((k_f > k_b ? k_f : k_b) >= u) {
                pthread_mutex_unlock(&maze_lock);
                break;
            }
        }
        pthread_mutex_unlock(&maze_lock);

        /* Check all the neighbours. Since we are using a block maze, at most
           four neighbours on the four directions. */
        for (direction = 0; direction < 4; ++direction) {
            node_t *n = fetch_neighbour(maze_b, cur, direction);
            
            if (n == NULL || n->mark == WALL || n->closed)
                continue;   /* Not valid, or closed already. */

            if (n->opened && cur->gs + 1 >= n->gs)
                continue;   /* Old node met, not getting shorter. */

            /* Passing through CUR is the shortest way up to now. Update. */
            n->parent = cur;
            n->gs = cur->gs + 1;
            n->fs = n->gs + heuristic(n, maze_b->start);
            if (!n->opened) {   /* New node discovered, add into heap. */
                n->opened = true;
                heap_insert(openset_b, n);
            } else              /* Updated old node. */
                heap_update(openset_b, n);
        }
    }

    return cur;
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
