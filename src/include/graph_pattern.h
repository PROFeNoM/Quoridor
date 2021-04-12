#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"
#include "utils.h"

enum direction_t { NO_DIRECTION=0, NORTH, SOUTH, WEST, EAST, MAX_DIRECTION };


// get the index of direction of j according to i
int get_direction_square(size_t i,size_t j, size_t m);

struct graph_t *get_graph(char type, size_t width);

int c_constraint(int size);
int t_constraint(int size);
int h_constraint(int size);
int s_constraint(int size);

#endif // _GRAPH_PATTERN_H_
