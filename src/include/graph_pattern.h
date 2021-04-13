#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"
#include "utils.h"

enum direction_t { NO_DIRECTION=0, NORTH, SOUTH, WEST, EAST, MAX_DIRECTION };


// get the index of direction of j according to i
int get_direction_square(size_t i,size_t j, size_t m);
struct graph_t *get_graph(char type, size_t width);
gsl_spmatrix_uint *fill_graph(size_t m, int (*is_not_connected)(size_t, size_t, size_t));
int square(size_t x, size_t y, size_t m);
gsl_spmatrix_uint *square_graph(size_t m);
int is_in_hole(size_t x, size_t y, size_t begin_x, size_t begin_y, size_t size_x, size_t size_y);
int toric(size_t x, size_t y, size_t m);

int c_constraint(int size);
int t_constraint(int size);
int h_constraint(int size);
int s_constraint(int size);

#endif // _GRAPH_PATTERN_H_
