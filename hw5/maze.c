/*
 * File: maze.c
 * 
 *   Implementation of library functions manipulating a minecraft-style block
 *     maze. Feel free to add, remove, or modify these library functions to
 *     serve your algorithm.
 *  
 * Jose @ ShanghaiTech University
 *
 */

#include <stdlib.h>     /* abs, malloc, free */
#include <assert.h>     /* assert */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include "maze.h"
#include "node.h"

/* Local helpers prototype. */
static mark_t char_to_mark (char c);


/* 
 * Initialize a maze from a formatted source file named FILENAME. Returns the
 *   pointer to the new maze.
 *
 */
maze_t *
maze_init (char *filename)
{
    int rows, cols, i, j, f;
    maze_t *m = malloc(sizeof(maze_t));
    /* fopen file handler */
    FILE* temp_file;
    /* a struct to store file info */
    struct stat file_info;
    /* map-reading index */
    int map_offset;

    /* Open the source file and read in number of rows & cols. */
    temp_file = fopen(filename, "r+");    /* Open with "r+" since might modify */
                                          /* at maze_print_step.               */
    /* get rows and cols */
    fscanf(temp_file, "%d %d\n", &rows, &cols);
    /* store rows */
    m->rows = rows;
    /* store cols */
    m->cols = cols;
    /* close file handle */
    fclose(temp_file);

    /* open file handle */
    f = open(filename, O_RDWR, (mode_t)0600);
    /* if failed to open file */
    if (f == -1) {
        perror("Error opening file for writing");
    }
    /* get file info */
    if (fstat(f, &file_info) == -1) {
        perror("Error getting the file size");
    }

    /* store file handle */
    m->file = f;
    /* store file size */
    m->file_size = file_info.st_size;

    /* create memory map */
    m->map = (char*)mmap(0, file_info.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, f, 0);
    /* init map-reading index */
    map_offset = 0;
    /* if mmap failed */
    if (m->map == MAP_FAILED) {
        perror("Failed to mmap file (read map)");
    }
    /* skip rows and cols */
    while (m->map[map_offset++] != '\n');

    /* Allocate space for all nodes (cells) inside the maze, then read them
       in from the source file. The maze records all pointers to its nodes
       in an array NODES. */
    m->nodes = malloc(rows * cols * sizeof(node_t *));
    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            mark_t mark = char_to_mark(m->map[map_offset + i * (cols + 1) + j]);
            maze_set_cell(m, i, j, mark);

            if (mark == START)
                m->start = maze_get_cell(m, i, j);
            else if (mark == GOAL)
                m->goal  = maze_get_cell(m, i, j);
        }
        
    }

    return m;
}

/* 
 * Delete the memory occupied by the maze M.
 *
 */
void
maze_destroy (maze_t *m, int is_unmap)
{
    int i, j;
    for (i = 0; i < m->rows; ++i)
        for (j = 0; j < m->cols; ++j)
            free(m->nodes[i * m->cols + j]);
    free(m->nodes);
    /* unmap memory */
    if (is_unmap == 1) {
        munmap(m->map, m->file_size);
        close(m->file);
    }
    free(m);
}

/* 
 * Initialize a cell in maze M at position (X, Y) to be a MARK-type cell.
 *
 */
void
maze_set_cell (maze_t *m, int x, int y, mark_t mark)
{
    assert(x >= 0 && x < m->rows &&
           y >= 0 && y < m->cols);
    m->nodes[x * m->cols + y] = node_init(x, y, mark);
}

/* 
 * Get the cell in maze M at position (X, Y). Returns the pointer to the
 *   specified cell.
 *
 */
node_t *
maze_get_cell (maze_t *m, int x, int y)
{
    if (x < 0 || x >= m->rows ||    /* Returns NULL if exceeds boundary. */
        y < 0 || y >= m->cols)
        return NULL;
    return m->nodes[x * m->cols + y];   /* Notice the row-major order. */
}

/* 
 * Sets the position of node N in maze M in the source file to be "*",
 *   indicating it is an intermediate step along the shortest path.
 *
 */
void
maze_print_step (maze_t *m, node_t *n)
{
    /*int i;*/
    char* map;
    /*long map_idx;*/
    long offset_map_idx;

    map = m->map;
    if (map == MAP_FAILED)
    {
        perror("Failed to mmap file (write map)");
        exit(EXIT_FAILURE);
    }

    offset_map_idx = 0;
    while (map[offset_map_idx++] != '\n');

    while (n != NULL && n->parent != NULL) {
        /*map_idx = offset_map_idx;
        for (i = 0; i < n->x; ++i)
            while (map[map_idx++] != '\n');
        for (i = 0; i < n->y; ++i)
            map_idx++;
        map[map_idx++] = '*';*/

        /* write to memory */
        if (n->mark != START && n->mark != GOAL)
            map[offset_map_idx + (n->x * (m->cols + 1)) + n->y] = '*';
        
        n = n->parent;
    }

    /* sync to disk */
    if (msync(map, m->file_size, MS_SYNC) == -1) {
        /* if failed to sync */
        perror("Failed to sync the file to disk (write map)");
    }
}


maze_t *
maze_copy (maze_t *src)
{
    int i, j;
    int map_offset;
    
    maze_t *m = malloc(sizeof(maze_t));
    
    /* store rows */
    m->rows = src->rows;
    /* store cols */
    m->cols = src->cols;
    /* store file handle */
    m->file = src->file;
    /* store file size */
    m->file_size = src->file_size;
    /* create memory map */
    m->map = src->map;
    /* clone nodes */
    m->nodes = malloc(src->rows * src->cols * sizeof(node_t *));

    map_offset = 0;
    /* skip rows and cols */
    while (m->map[map_offset++] != '\n');

    for (i = 0; i < src->rows; ++i) {
        for (j = 0; j < src->cols; ++j) {
            mark_t mark = char_to_mark(m->map[map_offset + i * (src->cols + 1) + j]);
            maze_set_cell(m, i, j, mark);

            if (mark == START)
                m->start = maze_get_cell(m, i, j);
            else if (mark == GOAL)
                m->goal  = maze_get_cell(m, i, j);
        }
    }

    return m;
}

/* 
 * Maps a character C to its corresponding mark type. Returns the mark.
 *
 */
static mark_t
char_to_mark (char c)
{
    switch (c) {
        case '#': return WALL;
        case '@': return START;
        case '%': return GOAL;
        default:  return NONE;
    }
}
