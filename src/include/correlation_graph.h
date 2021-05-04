#ifndef _CORRELATION_GRAPH_H_
#define _CORRELATION_GRAPH_H_

#include "graph.h"
#include "graph_pattern.h"

struct near_neighbours {
  size_t north;
  size_t south;
  size_t west;
  size_t east;
};

/**
 * Complete an array with each neighbour's index for each vertex
 * @param graph Game's board
 * @return Neighbours' array (size = graph vertices number) 
 */
struct near_neighbours *get_correlated_graph(struct graph_t *graph);

/**
 * Free the neighbours graph
 * @param neighbours_graph Neighbours' graph to be freed
 */
void free_correlation_graph(struct near_neighbours *neighbours_graph);
#endif
