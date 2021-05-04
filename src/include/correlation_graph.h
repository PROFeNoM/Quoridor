#ifndef _CORRELATION_GRAPH_H_
#define _CORRELATION_GRAPH_H_

#include "graph.h"

struct near_neighbours {
  size_t north;
  size_t south;
  size_t west;
  size_t east;
};

/**
 * Complete an array with each neighbour's index for each vertex
 * @param graph Game's board
 * @param neighbour_graph Array to be completed (size >= graph vertices number) 
 */
void get_correlated_graph(struct graph_t *graph, struct near_neighbours neighbours_graph[]);

#endif
