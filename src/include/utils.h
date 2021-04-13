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
 * Copy a graph structure
 * @param graph Graph structure representing the game board and its information
 * @return A copy of the given graph
 */
struct graph_t* graph_copy(struct graph_t* graph);


/**
 * 
 **/
void *load_library(char *);

/**
 * 
 **/
void *load_function(void *, const char *);


/**
 * Get the color of the next player
 * @param id Color of the active player
 * @return Color of the next player
 */
enum color_t get_next_player(enum color_t id);

/**
 * Check of a player won given its position on the board
 * @param graph Graph structure representing the game board and its information
 * @param player_id Color of the player
 * @param position Vertex the player is on the board
 * @return
 */
int is_winning(struct graph_t *graph, enum color_t player_id, size_t position);

/**
 * Check a player ownership on a vertex
 * @param graph Graph structure representing the game board and its information
 * @param player Color of the player
 * @param pos Index of the vertex to check the ownership for
 * @return 1 if position is owned by the given player
 */
int is_owned(struct graph_t *graph, enum color_t player_id, size_t position);

/**
 * Get the connection between two vertices
 * @param graph Graph structure representing the game board and its information
 * @param from First vertex (Origin)
 * @param to Second vertex (Extremity)
 * @return The connection type between two vertices
 */
int get_connection_type(struct graph_t* graph, size_t from, size_t to);

/**
 * Change the connection between two vertices
 * @param graph Graph structure representing the game board and its information
 * @param from First vertex (Origin)
 * @param to Second vertex (Extremity)
 * @param connection Type of connection to set between the two vertices
 */
void set_connection_type(struct graph_t *graph, size_t from, size_t to, int connection);

/**
 * Check if two vertices are connected
 * @param graph Graph structure representing the game board and its information
 * @param from First vertex (Origin)
 * @param to Second vertex (Extremity)
 * @return 1 if vertices are connected, else 0
 */
int is_connected(struct graph_t* graph, size_t from, size_t to);

/**
 * Check if a vertex index exist in a graph
 * @param graph Graph structure representing the game board and its information
 * @param vertex Index of the vertex to check
 * @return 1 if given vertex is on the graph, else 0
 */
int is_vertex_in_graph(struct graph_t* graph, size_t vertex);

/**
 * Check if two vertices are in a West-East relationship
 * @param graph Graph structure representing the game board and its information
 * @param from First vertex (Origin)
 * @param to Second vertex (Extremity)
 * @return 1 if given vertices are in a West-East relationship
 */
int is_horizontal_connection(struct graph_t* graph, size_t from, size_t to);

/**
 * Check if two vertices are in a North-South relationship
 * @param graph Graph structure representing the game board and its information
 * @param from First vertex (Origin)
 * @param to Second vertex (Extremity)
 * @return 1 if given vertices are in a North-South relationship
 */
int is_vertical_connection(struct graph_t* graph, size_t from, size_t to);

/**
 * Add a wall between given edges
 * @param graph Graph structure representing the game board and its information
 * @param e Edge structure of the wall to add on the board
 */
void add_wall(struct graph_t* graph, struct edge_t e[]);

/**
 * Check if a path exists from a vertex position of a player to the opponent area
 * @param graph Graph structure representing the game board and its information
 * @param visited Array of visited vertices
 * @param position Position of the player on the board
 * @param player_id Color of the player
 * @return 1 if a path exists, else 0
 */
int is_path_existing(struct graph_t *graph, int visited[], size_t position, enum color_t player_id);

/**
 * Check if a player is blocked
 * @param graph Graph structure representing the game board and its information
 * @param position Position of the player on the board
 * @param player_id Color of the player
 * @return 1 if the player is blocked, else 0
 */
int is_player_blocked(struct graph_t* graph, size_t position, enum color_t player_id);

/**
 * Check if a wall can be added at given position at given state of the game
 * @param graph Graph structure representing the game board and its information
 * @param e Edge structure of the wall to add on the board
 * @param p1_position Position of the first player on the board
 * @param p2_position Position of the second player on the board
 * @return 1 if the wall can be added on the board, else 0
 */
int can_add_wall(struct graph_t* graph, struct edge_t e[], size_t p1_position, size_t p2_position);

/**
 * Check if a player can move to a given position
 * @param graph Graph structure representing the game board and its information
 * @param vertex Vertex the player wants to move to
 * @param active_player Player initiating the move
 * @param p1_position Position of the first player on the board
 * @param p2_position Position of the second player on the board
 * @return 1 if a player can move to given position on the board, else 0
 */
int can_player_move_to(struct graph_t* graph, size_t vertex, enum color_t active_player, size_t p1_position, size_t p2_position);

/**
 * Check if a given move is legal
 * @param graph Graph structure representing the game board and its information
 * @param move Move structure to check for legality
 * @param p1_position Position of the first player on the board
 * @param p2_position Position of the second player on the board
 * @return 1 if the move is legal, else 0
 */
int is_move_legal(struct graph_t* graph, struct move_t* move, size_t p1_position, size_t p2_position);

/**
 * Release allocated memory for a graph
 * @param graph Graph structure representing the game board and its information
 */
void graph_free(struct graph_t *graph);

/**
 * Display the t graph values of a graph_t
 * @param graph Graph structure representing the game board and its information
 */
void display_graph_value(struct graph_t *graph);

#endif