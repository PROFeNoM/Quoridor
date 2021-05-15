#ifndef _TEST_DIJSKTRA_H_
#define _TEST_DIJSKTRA_H_

#include "dijsktra.h"
#include "graph_pattern.h"
#include "correlation_graph.h"

// Functions in dijsktra.c to test
void init_array_with_value(size_t, size_t[], size_t);
void init_array_with_index(size_t[], size_t);
size_t look_for_jump(struct graph_t *, struct near_neighbours [], size_t [], size_t [], size_t, size_t);
int is_visited(size_t [], size_t);
int is_all_visited(size_t, size_t);
size_t get_dist_min_not_visited(size_t [], size_t [], size_t);
size_t get_nearest_destination(size_t [], size_t [], size_t);
size_t get_first_step_from_shortest_path(size_t [], size_t, size_t);

#endif