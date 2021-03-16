#include "annex_function.h"

struct graph_t* graph_copy(struct graph_t* graph)
{
    struct graph_t* cp = malloc(sizeof(struct graph_t));

    cp->num_vertices = graph->num_vertices;

    cp->t = gsl_spmatrix_alloc(graph->num_vertices, graph->num_vertices);
    gsl_spmatrix_memcpy(cp->t, graph->t);

    cp->o = gsl_spmatrix_alloc(2, graph->num_vertices);
    gsl_spmatrix_memcpy(cp->o, graph->o);

    return cp;
}



void graph_free(struct graph_t *graph)
{
  gsl_spmatrix_free(graph->t);
  gsl_spmatrix_free(graph->o);
  free(graph);
}
