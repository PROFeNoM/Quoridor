#ifndef _UTILS_H_
#define _UTILS_H_

#include <dlfcn.h>

#include "graph.h"
#include "server.h"

enum wall_orientation_t {
    POINT_TO_NORTH = 5,
    POINT_TO_SOUTH,
    POINT_TO_WEST,
    POINT_TO_EAST,
    NOT_CONNECTED
};

/**
 * 
 **/
struct graph_t * graph_copy(struct graph_t *);

/**
 * 
 **/
void graph_free(struct graph_t *);

/**
 * 
 **/
void *load_library(char *);

/**
 * 
 **/
void *load_function(void *, const char *);

/**
 * 
 **/
enum color_t get_next_player(enum color_t);

/**
 * 
 **/
int is_winning(struct graph_t *, enum color_t, size_t);

/**
 * 
 **/
// Return 1 if position is owned by the given player
int is_owned(struct graph_t*, int, size_t);

/**
 * 
 **/
// Return the connection between two vertices
int get_connection_type(struct graph_t *, size_t, size_t);

/**
 * 
 **/
// Change the connection between two vertices
void set_connection_type(struct graph_t *, size_t, size_t, int);

/**
 * 
 **/
// Return 1 if vertices are connected, else 0
int is_connected(struct graph_t *, size_t, size_t);

/**
 * 
 **/
// Return 1 if given vertex is on the graph, else 0
int is_vertex_in_graph(struct graph_t *, size_t);

/**
 * 
 **/
// Return 1 if given vertices are in a West-East relationship
int is_horizontal_connection(struct graph_t*, size_t, size_t);

/**
 * 
 **/
// Return 1 if given vertices are in a North-South relationship
int is_vertical_connection(struct graph_t*, size_t, size_t);

/**
 * 
 **/
// Add a wall between given edges
void add_wall(struct graph_t *, struct edge_t[]);

/**
 * 
 **/
// Return 1 if given vertex is in area, else 0
int is_vertex_in_area(struct graph_t *, enum color_t, size_t);

/**
 * 
 **/
// Return 1 if a path exist between a vertex and a
int is_path_existing(struct graph_t *, int[], size_t, enum color_t);

/**
 * 
 **/
// Return 1 if the player is blocked, else 0
int is_player_blocked(struct graph_t *, size_t, enum color_t);

/**
 * 
 **/
// Return 1 if the wall can be added on the board, else 0
int can_add_wall(struct graph_t *, struct edge_t[], size_t, size_t);

/**
 * 
 **/
// Return 1 if a player can move to a given position on the board, else 0
int can_player_move_to(struct graph_t*, size_t, enum color_t, size_t, size_t);

/**
 * 
 **/
// Return 1 if no player is on the vertex, else 0
int is_vertex_unoccupied(size_t, size_t, size_t);

/**
 * 
 **/
// Return 1 if the move is legal, else 0
int is_move_legal(struct graph_t*, struct move_t*, size_t, size_t);

/**
 * 
 **/
// Release allocated memory
void graph_free(struct graph_t *);

/**
 * 
 **/
// Display the t graph values of a graph_t
void display_graph_value(struct graph_t *graph);

#endif