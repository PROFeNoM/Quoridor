#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdlib.h>

#include "graph.h"
#include "graph_pattern.h"


/**
 * 
 **/
struct player_server {
    void *lib;
    char const *(*get_player_name)(void);
    void (*initialize)(enum color_t, struct graph_t *, size_t);
    struct move_t (*play)(struct move_t);
    void (*finalize)(void);
    size_t pos;
};

struct graph_server {
    struct graph_t *graph;
    size_t width;
    char type;
};

struct server {
    struct player_server players[2];
    struct graph_server graph;
};

struct server *initialize_server(char *, char *, size_t, char);
void run_server(struct server *);
void free_server(struct server *server);

#endif
