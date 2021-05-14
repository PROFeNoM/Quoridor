#ifndef _TEST_SERVER_H_
#define _TEST_SERVER_H_

#include "server.h"
#include "graph_pattern.h"

// Functions in server.c to test
size_t get_number_of_walls(size_t width, char type);
void initialize_graph(size_t width, char type, struct graph_server *graph);
struct move_t get_initial_move();
void update(struct server *server, struct move_t move);

#endif