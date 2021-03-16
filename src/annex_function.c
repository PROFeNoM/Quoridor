#include <stdio.h>

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

// Returns owner (either '1' or '2') of the vertex i, else 'X' if vertex isn't owned
char get_vertex_owner(gsl_spmatrix* o, size_t m, size_t i)
{
    double player1_ownership = gsl_spmatrix_get(o, 0, i);
    double player2_ownership = gsl_spmatrix_get(o, 1, i);
    return player1_ownership ? '1' : (player2_ownership ? '2' : 'X');
}

void display_graph(struct graph_t* graph, size_t m)
{
    for (size_t i = 0; i < m; i++){
        for (size_t j = 0; j < m; j++){
            printf(" %c ", get_vertex_owner(graph->o, m, i * m + j));
        }
        printf("\n");
    }
}



void graph_free(struct graph_t *graph)
{
  gsl_spmatrix_free(graph->t);
  gsl_spmatrix_free(graph->o);
  free(graph);
}
