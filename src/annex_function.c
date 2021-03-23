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


double get_connection(struct graph_t* graph, size_t a, size_t b)
{
    return gsl_spmatrix_get(graph->t, a, b);
}

int is_connected(struct graph_t* graph, size_t a, size_t b)
{
    return get_connection(graph, a, b) > 0;
}

void change_connection(struct graph_t* graph, size_t a, size_t b, double connection)
{
    gsl_spmatrix_set(graph->t, a, b, connection);
}

int is_vertice_on_graph(struct graph_t* graph, size_t a)
{
    return 0 <= a && a <= graph->num_vertices;
}

void add_wall(struct graph_t* graph, struct edge_t e[])
{
    static int wall_placed = 1;
    change_connection(graph, e[0].fr, e[0].to, -wall_placed);
    change_connection(graph, e[1].fr, e[1].to, -wall_placed++);
}

int is_vertice_in_area(size_t pos, gsl_spmatrix* o, int p)
{
    return gsl_spmatrix_get(o, p, pos) == 1;
}

int is_path_existing(struct graph_t* graph, size_t pos, gsl_spmatrix* o, int visited[], int p)
{
    visited[(int)pos] = 1;
    if (is_vertice_in_area(pos, o, p))
        return 1;

    int flag_path = 0;
    for (size_t j = 0; j < graph->num_vertices; j++)
        if (is_connected(graph, pos, j) && !visited[(int)j])
        {
            int path = is_path_existing(graph, j, o, visited, p);
            if (path)
                return 1;
        }

    return 0;
}

int is_player_blocked(struct graph_t* graph, struct player_server player, int p)
{
    int visited[graph->num_vertices];
    for (int i = 0; i < graph->num_vertices; i++)
        visited[i] = 0;

    int path = is_path_existing(graph, player.pos, graph->o, visited, p);
    return !path;
}

// size of e is 2
int can_add_wall(struct graph_t* graph, struct edge_t e[], struct player_server * players)
{
    // Vertices must be on graph
    if (!(is_vertice_on_graph(graph, e[0].fr) && is_vertice_on_graph(graph, e[0].to)
        && is_vertice_on_graph(graph, e[1].fr) && is_vertice_on_graph(graph, e[1].to)))
        return 0;

    // Check if vertices are connected AND if it won't overlap an other wall
    if (!(is_connected(graph, e[0].fr, e[0].to) && is_connected(graph, e[1].fr, e[1].to)))
        return 0;

    // Check vertices relation
    double relation1 = get_connection(graph, e[0].fr, e[0].to);
    int flag_connection = 0;
    if (relation1 == EAST || relation1 == WEST)
    {
        if (is_connected(graph, e[0].fr, e[1].fr))
        {
            double relation2 = get_connection(graph, e[0].fr, e[1].fr);
            if (relation2 != NORTH && relation2 != SOUTH)
                return 0;

            flag_connection = 1;
        }

        if (is_connected(graph, e[0].fr, e[1].to))
        {
            if (flag_connection)
                return 0;

            double relation2 = get_connection(graph, e[0].fr, e[1].to);
            if (relation2 != NORTH && relation2 != SOUTH)
                return 0;

            flag_connection = 1;
        }

        if (!flag_connection)
            return 0;
    }
    else // North-South relation
    {
        if (is_connected(graph, e[0].fr, e[1].fr))
        {
            double relation2 = get_connection(graph, e[0].fr, e[1].fr);
            if (relation2 != EAST && relation2 != WEST)
                return 0;

            flag_connection = 1;
        }

        if (is_connected(graph, e[0].fr, e[1].to))
        {
            if (flag_connection)
                return 1;

            double relation2 = get_connection(graph, e[0].fr, e[1].to);
            if (relation2 != EAST && relation2 != WEST)
                return 0;

            flag_connection = 1;
        }

        if (!flag_connection)
            return 0;
    }

    // Check if players aren't blocked
    struct graph_t* graph_with_wall = graph_copy(graph);
    add_wall(graph_with_wall, e);
    if (is_player_blocked(graph_with_wall, players[0], 1)
        || is_player_blocked(graph_with_wall, players[1], 0))
        return 0;
    // TODO: There's a problem during the tests with this free.
    //graph_free(graph);

    // The wall doesn't violate any rules, hence it's a legal move
    return 1;
}
