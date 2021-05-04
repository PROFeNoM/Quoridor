#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"
#include "utils.h"

enum direction_t { NO_DIRECTION=0, NORTH, SOUTH, WEST, EAST, MAX_DIRECTION };

/**
 * Fills the adjacency matrix of relation and of win position of the graph :
 * @param type : the type graph
 * @param width : the graph width
 * @return the graph
 */
struct graph_t *get_graph(char type, size_t width);

/**
 * Verifies if the size respects the graph constraint:
 * @param type : the graph type
 * @param size : graph width
 * @return the boolean corresponding to the constraint respect
 */
int abide_graph_constraint(char type, int size);

#endif // _GRAPH_PATTERN_H_
