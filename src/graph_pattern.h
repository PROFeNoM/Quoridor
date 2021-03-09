#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"

#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4
#define NO_DIRECTION 0

// get the index of direction of j according to i
int get_number_edge_square(size_t i,size_t j,int m);
// return the spmatrix of a square graph of size m by m
gsl_spmatrix * square_graph(int m);


#endif // _GRAPH_PATTERN_H_
