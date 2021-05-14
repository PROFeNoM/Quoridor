#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdlib.h>

#include "graph.h"
#include "graph_pattern.h"

/**
 * Represent a player on the server side
 */
struct player_server {
    void *lib;
    char const *(*get_player_name)(void);
    void (*initialize)(enum color_t, struct graph_t *, size_t);
    struct move_t (*play)(struct move_t);
    void (*finalize)(void);
    size_t num_wall;
    size_t pos;
    size_t is_initialized;
};

/**
 * Represent a graph on the server side
 */
struct graph_server {
    struct graph_t *graph;
    size_t width;
    size_t num_wall;
    char type;
};

/**
 * Represent a server
 */
struct server {
    struct player_server players[2];
    struct graph_server graph;
};

/**
 * Initializes the server structure  :
 * @param path_lib_player1 : path to the first player librairy function
 * @param path_lib_player2 : path to the second player librairy function
 * @param width : graph width
 * @param type : graph type
 * @return the server structure initialized
 */
struct server *initialize_server(char *path_lib_player1, char *path_lib_player2, size_t width, char type);

/**
 * Runs the game loop :
 * @param server : the server structure
 * @param print : indicates wether the boardgame should be displayed
 * @param delay : the delay between two displays
 */
void run_server(struct server *server, int print, int delay);

/**
 * free the server structure  :
 * @param server : the server structure
 */
void free_server(struct server *server);

#endif
