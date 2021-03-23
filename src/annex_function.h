#ifndef _ANNEX_FUNCTION_H_
#define _ANNEX_FUNCTION_H_

#include "graph.h"
#include "graph_pattern.h"

struct graph_t * graph_copy(struct graph_t *);

// Display current board of the game
// TODO: Handle walls
// TODO: Handle different shape of board
void display_graph(struct graph_t*, size_t, struct player_server *);

void graph_free(struct graph_t *);

void is_connected(struct graph_t*, size_t, size_t);

void add_wall(struct graph_t*, size_t, size_t);

#endif // _ANNEX_FUNCTION_H_
