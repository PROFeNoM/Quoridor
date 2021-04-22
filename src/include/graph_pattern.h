#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"
#include "utils.h"

enum direction_t { NO_DIRECTION=0, NORTH, SOUTH, WEST, EAST, MAX_DIRECTION };

struct graph_t *get_graph(char type, size_t width);

int abide_graph_constraint(char type, int size);

#endif // _GRAPH_PATTERN_H_
