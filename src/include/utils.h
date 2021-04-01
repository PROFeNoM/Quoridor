#ifndef _UTILS_H_
#define _UTILS_H_

#include <dlfcn.h>

#include "graph.h"
#include "server.h"

struct graph_t * graph_copy(struct graph_t *);

void graph_free(struct graph_t *);

void *load_library(char *);

void *load_function(void *, const char *);

enum color_t get_next_player(enum color_t);

int is_winning(struct graph_t *, enum color_t, size_t);

// Return the connection between two vertices
double get_connection(struct graph_t *, size_t, size_t);

// Change the connection between two vertices
void change_connection(struct graph_t *, size_t, size_t, double);

// Return 1 if vertices are connected, else 0
int is_connected(struct graph_t *, size_t, size_t);

// Return 1 if given vertice is on the graph, else 0
int is_vertice_on_graph(struct graph_t *, size_t);

// Add a wall between given edges
void add_wall(struct graph_t *, struct edge_t[]);

// Return 1 if given vertice is in area, else 0
int is_vertice_in_area(size_t, gsl_spmatrix_uint *, int);

// Return 1 if a path exist between a vertice and a
int is_path_existing(struct graph_t *, size_t, gsl_spmatrix_uint *, int[], int);

// Return 1 if the player is blocked, else 0
int is_player_blocked(struct graph_t *, struct player_server*, int);

// Return 1 if the wall can be added on the board, else 0
int can_add_wall(struct graph_t *, struct edge_t[], struct player_server *);

// Release allocated memory
void graph_free(struct graph_t *);

#endif
