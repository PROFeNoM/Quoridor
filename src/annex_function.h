#ifndef _ANNEX_FUNCTION_H_
#define _ANNEX_FUNCTION_H_

#include "graph.h"

struct graph_t * graph_copy(struct graph_t *);

// Display current board of the game
// TODO: Handle walls
// TODO: Handle different shape of board
void display_graph(struct graph_t*, size_t);

void graph_free(struct graph_t *);

#endif // _ANNEX_FUNCTION_H_
