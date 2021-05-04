#ifndef _DIJSKTRA_H_
#define _DIJSKTRA_H_

#include "correlation_graph.h"
#include "graph.h"

/**
 * Apply dijsktra algorithm to a graph in order to obtain the vertex belonging to the shortest path to one of the dst vertices
 * @param graph Graph representing the board to consider
 * @param neighbours_graph Array of neighbour's index for each vertex
 * @param dst Array containing the possible destination vertices
 * @param dst_size Number of possible destination (need to be > 0)
 * @param player Vertex where dijkstra algorithm start (the player position)
 * @param other_player Position of the other player
 */
size_t dijsktra(struct graph_t *graph, struct near_neighbours neighbours_graph[], size_t dst[], size_t dst_size, size_t player, size_t other_player);

#endif // _DIJSKTRA_H_
