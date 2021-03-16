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


void display_graph(struct graph_t* graph, size_t m, struct move_t* player1, struct move_t* player2)
{
    for (size_t i = 0; i < m; i++){
        for (size_t j = 0; j < m; j++){
            size_t vertex = i * m + j;
            printf(" %c ", vertex == player1->m ? '1' : (vertex == player2->m ? '2' : 'X'));
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
