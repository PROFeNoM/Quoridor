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


void display_graph(struct graph_t* graph, size_t m, struct player_server * players)
{
    for (size_t i = 0; i < m; i++){
        for (size_t j = 0; j < m; j++){
            size_t vertex = i * m + j;
            printf(" %c ", vertex == players[0].pos ? '1' : (vertex == players[1].pos ? '2' : 'X'));
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

void is_connected(struct graph_t* graph, size_t a, size_t b)
{
    return gsl_spmatrix_get(graph->t, a, b) > 0;
}

void add_wall(struct graph_t* graph, size_t a, size_t b)
{
    static int wall_placed = 1;
    gsl_spmatrix_set(graph->t, a, b, -wall_placed);
    gsl_spmatrix_set(graph->t, b, a, -wall_placed++);
}