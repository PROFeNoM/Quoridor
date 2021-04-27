#ifndef _DIJSKTRA_H_
#define _DIJSKTRA_H_

#include "graph.h"


struct dijsktra_t;
struct dijsktra_t * init_dijsktra(size_t dist, size_t prec);
void free_dijsktra(struct dijsktra_t * d);
int all_visited(gsl_spmatrix_uint * visited);
gsl_spmatrix_uint *  init_visited(size_t num);
gsl_spmatrix_uint * init_dist(size_t num);
gsl_spmatrix_uint * init_prec(size_t num);
int is_visited(gsl_spmatrix_uint * visited,size_t index);
size_t get_dist_min_not_visited(gsl_spmatrix_uint * dist,gsl_spmatrix_uint * visited);
struct dijsktra_t * dijsktra(struct graph_t * graph,size_t src,size_t dst);
size_t mini_dijsktra(struct graph_t * graph, size_t position, enum color_t id);

#endif // _DIJSKTRA_H_
