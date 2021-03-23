#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"

enum direction_t {NO_DIRECTION=0,NORTH,SOUTH,WEST,EAST};


// get the index of direction of j according to i
int get_direction_square(size_t i,size_t j,int m);

// return the spmatrix of a square graph of size m by m
gsl_spmatrix * square_graph(int m);

gsl_spmatrix * matrix_position(int m);

#endif // _GRAPH_PATTERN_H_
