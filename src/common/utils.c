#include "utils.h"

struct graph_t* graph_copy(struct graph_t* graph)
{
    struct graph_t* cp = malloc(sizeof(struct graph_t));

    cp->num_vertices = graph->num_vertices;

    cp->t = gsl_spmatrix_uint_alloc(graph->num_vertices, graph->num_vertices);
    gsl_spmatrix_uint_memcpy(cp->t, graph->t);

    cp->o = gsl_spmatrix_uint_alloc(2, graph->num_vertices);
    gsl_spmatrix_uint_memcpy(cp->o, graph->o);

    return cp;
}


void graph_free(struct graph_t *graph)
{
  gsl_spmatrix_uint_free(graph->t);
  gsl_spmatrix_uint_free(graph->o);
  free(graph);
}

void *load_library(char *path)
{
  void *lib;
  lib = dlopen(path, RTLD_LAZY);
  if (!lib)
    return NULL;
  return lib;
}

void *load_function(void *lib, const char *name)
{
  void *function = dlsym(lib, name);
  if (!function)
    return NULL;
  return function;
}

enum color_t get_next_player(enum color_t id)
{
  return (id == BLACK) ? WHITE : BLACK;
}

int is_winning(struct graph_t *graph, enum color_t player_id, size_t position)
{
  return gsl_spmatrix_uint_get(graph->o, get_next_player(player_id), position);
}

int get_connection(struct graph_t* graph, size_t a, size_t b)
{
    return gsl_spmatrix_uint_get(graph->t, a, b);
}

int is_connected(struct graph_t* graph, size_t a, size_t b)
{
    return MAX_DIRECTION > get_connection(graph, a, b) && get_connection(graph, a, b) > 0;
}

void change_connection(struct graph_t* graph, size_t a, size_t b, int connection)
{
    gsl_spmatrix_uint_set(graph->t, a, b, connection);
}

int is_vertex_on_graph(struct graph_t* graph, size_t a)
{
    return a < graph->num_vertices;
}

int is_horizontal_relation(struct graph_t* graph, size_t a, size_t b)
{
    int relation = get_connection(graph, a, b);
    return relation == EAST || relation == WEST;
}

int is_vertical_relation(struct graph_t* graph, size_t a, size_t b)
{
    int relation = get_connection(graph, a, b);
    return relation == NORTH || relation == SOUTH;
}

void add_wall(struct graph_t* graph, struct edge_t e[])
{
    if (is_horizontal_relation(graph, e[0].fr, e[0].to))
    {
        change_connection(graph, e[0].fr, e[0].to, e[0].fr > e[1].fr ? POINT_TO_NORTH : POINT_TO_SOUTH);
        change_connection(graph, e[0].to, e[0].fr, e[0].fr > e[1].fr ? POINT_TO_NORTH : POINT_TO_SOUTH);
        change_connection(graph, e[1].fr, e[1].to, e[0].fr > e[1].fr ? POINT_TO_SOUTH : POINT_TO_NORTH);
        change_connection(graph, e[1].to, e[1].fr, e[0].fr > e[1].fr ? POINT_TO_SOUTH : POINT_TO_NORTH);
    }
    else
    {
        change_connection(graph, e[0].fr, e[0].to, e[0].fr > e[1].fr ? POINT_TO_WEST : POINT_TO_EAST);
        change_connection(graph, e[0].to, e[0].fr, e[0].fr > e[1].fr ? POINT_TO_WEST : POINT_TO_EAST);
        change_connection(graph, e[1].fr, e[1].to, e[0].fr > e[1].fr ? POINT_TO_EAST : POINT_TO_WEST);
        change_connection(graph, e[1].to, e[1].fr, e[0].fr > e[1].fr ? POINT_TO_EAST : POINT_TO_WEST);
    }
}

int is_vertex_in_area(size_t pos, gsl_spmatrix_uint* o, int p)
{
    return gsl_spmatrix_uint_get(o, p, pos) == 1;
}

int is_path_existing(struct graph_t* graph, size_t pos, gsl_spmatrix_uint* o, int visited[], int p)
{
    visited[(int)pos] = 1;
    if (is_vertex_in_area(pos, o, p))
        return 1;

    for (size_t j = 0; j < graph->num_vertices; j++)
        if (is_connected(graph, pos, j) && !visited[(int)j] && is_path_existing(graph, j, o, visited, p))
            return 1;

    return 0;
}

int is_player_blocked(struct graph_t* graph, struct player_server *player, int p)
{
    int visited[graph->num_vertices];
    for (size_t i = 0; i < graph->num_vertices; i++)
        visited[i] = 0;

    return !is_path_existing(graph, player->pos, graph->o, visited, p);
}

// size of e is 2
int can_add_wall(struct graph_t* graph, struct edge_t e[], struct player_server * players)
{
    // TODO: Check nb of walls left for the active player
    // TODO: Change struct player_server to integers

    // Vertices must be on graph
    if (!(is_vertex_on_graph(graph, e[0].fr) && is_vertex_on_graph(graph, e[0].to)
        && is_vertex_on_graph(graph, e[1].fr) && is_vertex_on_graph(graph, e[1].to)))
        return 0;

    // Check if vertices are connected AND if it won't overlap an other wall
    if (!(is_connected(graph, e[0].fr, e[0].to) && is_connected(graph, e[1].fr, e[1].to)))
        return 0;

    // Check vertices relation
    int flag_connection = 0;
    if (is_horizontal_relation(graph, e[0].fr, e[0].to))
    {
        if (is_connected(graph, e[0].fr, e[1].fr))
        {
            if (!is_vertical_relation(graph, e[0].fr, e[1].fr))
                return 0;

            flag_connection = 1;
        }

        if (is_connected(graph, e[0].fr, e[1].to))
        {
            if (flag_connection || !is_vertical_relation(graph, e[0].fr, e[1].to))
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
            if (!is_horizontal_relation(graph, e[0].fr, e[1].fr))
                return 0;

            flag_connection = 1;
        }

        if (is_connected(graph, e[0].fr, e[1].to))
        {
            if (flag_connection || !is_horizontal_relation(graph, e[0].fr, e[1].to))
                return 0;

            flag_connection = 1;
        }

        if (!flag_connection)
            return 0;
    }

    // Check if players aren't blocked
    struct graph_t* graph_with_wall = graph_copy(graph);
    add_wall(graph_with_wall, e);
    if (is_player_blocked(graph_with_wall, &players[0], 1)
        || is_player_blocked(graph_with_wall, &players[1], 0))
        return 0;
    graph_free(graph_with_wall);

    // The wall doesn't violate any rules, hence it's a legal move
    return 1;
}

int is_vertex_unoccupied(size_t pos, size_t position_player1, size_t position_player2)
{
    return !(pos == position_player1 || pos == position_player2);
}

int is_connection_existing(struct graph_t* graph, size_t i, int direction)
{
    for (size_t j = 0; j < graph->num_vertices; j++)
        if (get_connection(graph, i, j) == direction)
            return 1;

    return 0;
}

int can_player_move_to(struct graph_t* graph, size_t pos, int active_player, size_t position_player1, size_t position_player2)
{
    // Check if position is on the board
    if (!is_vertex_on_graph(graph, pos))
        return 0;

    // Check if the opponent isn't already on it
    if (!is_vertex_unoccupied(pos, position_player1, position_player2))
        return 0;

    size_t initial_position = active_player == 0 ? position_player1 : position_player2;
    size_t opponent_position = active_player == 0 ? position_player2 : position_player1;

    // The position is directly accessible, hence the player can move to it
    if (is_connected(graph, initial_position, pos))
        return 1;

    // The position isn't directly accessible
    // Check if player are facing each other, and can jump to the position
    if (is_connected(graph, initial_position, opponent_position))
    {
        // Check if the opponent is next to the position
        if (!is_connected(graph, opponent_position, pos))
            return 0;

        // The opponent is next to the position

        // Check if position is behind the opponent
        if (get_connection(graph, initial_position, opponent_position) == get_connection(graph, opponent_position, pos))
            return 1;

        // There's a wall/edge of the board behind the opponent player
        if (!is_connection_existing(graph, opponent_position, get_connection(graph, initial_position, opponent_position)))
            return 1;
    }

    // The position isn't accessible
    return 0;
}