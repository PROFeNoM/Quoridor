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

int is_owned(struct graph_t *graph, enum color_t player_id, size_t position)
{
    return gsl_spmatrix_uint_get(graph->o, player_id, position) == 1;
}

int is_winning(struct graph_t *graph, enum color_t player_id, size_t position)
{
	if (!is_vertex_in_graph(graph, position))
		return 0;
  	return is_owned(graph, get_next_player(player_id), position);
}

int get_connection_type(struct graph_t* graph, size_t from, size_t to)
{
    return gsl_spmatrix_uint_get(graph->t, from, to);
}

void set_connection_type(struct graph_t *graph, size_t from, size_t to, int connection)
{
    gsl_spmatrix_uint_set(graph->t, from, to, connection);
}

int is_connected(struct graph_t* graph, size_t from, size_t to)
{
	if (!(is_vertex_in_graph(graph, from) && is_vertex_in_graph(graph, to)))
		return 0;
    int connection_type = get_connection_type(graph, from, to);
    return MAX_DIRECTION > connection_type  && connection_type > NO_DIRECTION;
}

int is_vertex_in_graph(struct graph_t* graph, size_t vertex)
{
    return vertex < graph->num_vertices;
}

int is_horizontal_connection(struct graph_t* graph, size_t from, size_t to)
{
    int relation = get_connection_type(graph, from, to);
    return relation == EAST || relation == WEST;
}

int is_vertical_connection(struct graph_t* graph, size_t from, size_t to)
{
    int relation = get_connection_type(graph, from, to);
    return relation == NORTH || relation == SOUTH;
}

enum wall_orientation_t get_wall_orientation_opposed(enum wall_orientation_t orientation) 
{
    switch (orientation)
    {
    case POINT_TO_NORTH:
        return POINT_TO_SOUTH;
    case POINT_TO_SOUTH:
        return POINT_TO_NORTH;
    case POINT_TO_EAST:
        return POINT_TO_WEST;
    case POINT_TO_WEST:
        return POINT_TO_EAST;
    default:
        return NOT_CONNECTED;
    }
}

void add_wall(struct graph_t* graph, struct edge_t e[])
{
    if (is_horizontal_connection(graph, e[0].fr, e[0].to))
    {
        enum wall_orientation_t orientation_e1 = e[0].fr > e[1].fr ? POINT_TO_NORTH : POINT_TO_SOUTH;
        enum wall_orientation_t orientation_e2 = get_wall_orientation_opposed(orientation_e1);
        set_connection_type(graph, e[0].fr, e[0].to, orientation_e1);
        set_connection_type(graph, e[0].to, e[0].fr, orientation_e1);
        set_connection_type(graph, e[1].fr, e[1].to, orientation_e2);
        set_connection_type(graph, e[1].to, e[1].fr, orientation_e2);
    }
    else
    {
        enum wall_orientation_t orientation_e1 = e[0].fr > e[1].fr ? POINT_TO_WEST : POINT_TO_EAST;
        enum wall_orientation_t orientation_e2 = get_wall_orientation_opposed(orientation_e1);
        set_connection_type(graph, e[0].fr, e[0].to, orientation_e1);
        set_connection_type(graph, e[0].to, e[0].fr, orientation_e1);
        set_connection_type(graph, e[1].fr, e[1].to, orientation_e2);
        set_connection_type(graph, e[1].to, e[1].fr, orientation_e2);
    }
}

int is_path_existing(struct graph_t *graph, int visited[], size_t position, enum color_t player_id)
{
	printf("\t\tDEBUG: Position > %zd\n", position);
    visited[position] = 1;
    printf("\t\tDEBUG: Marked as visited\n");
    if (is_winning(graph, player_id, position))
        return 1;
	printf("\t\tDEBUG: Isn't destination\n");
    for (size_t j = 0; j < graph->num_vertices; j++)
	{
    	printf("\t\tDEBUG: j = %zd\n", j);
		if (is_connected(graph, position, j) && !visited[(int)j] && is_path_existing(graph, visited, j, player_id))
			return 1;
	}
	printf("\t\tDEBUG: Out\n");
    return 0;
}

int is_player_blocked(struct graph_t* graph, size_t position, enum color_t player_id)
{
	printf("\tDEBUG: Test path from %zd\n", position);
	if (!is_vertex_in_graph(graph, position))
		return 1;
	printf("\tDEBUG: Player's initial position is legal\n");
    int visited[graph->num_vertices];
    for (size_t i = 0; i < graph->num_vertices; i++)
        visited[i] = 0;
	printf("\t\tDEBUG: Visited array created of size %zd\n", graph->num_vertices);
    return !is_path_existing(graph, visited, position, player_id);
}

// size of e is 2
int can_add_wall(struct graph_t* graph, struct edge_t e[], size_t p1_position, size_t p2_position)
{
    // TODO: Check nb of walls left for the active player
	printf("DEBUG: Start of can_add_wall\n");
    // Vertices must be on graph
    if (!(is_vertex_in_graph(graph, e[0].fr) && is_vertex_in_graph(graph, e[0].to)
        && is_vertex_in_graph(graph, e[1].fr) && is_vertex_in_graph(graph, e[1].to)))
	{
    	return 0;
	}

	printf("DEBUG: Vertices are on the graph\n");

    // Check if vertices are connected AND if it won't overlap an other wall
    if (!(is_connected(graph, e[0].fr, e[0].to) && is_connected(graph, e[1].fr, e[1].to)))
	{
    	return 0;
	}

	printf("DEBUG: Vertices are connected and the edge won't overlap an other wall\n");
    // Check vertices relation
    int flag_connection = 0;
    if (is_horizontal_connection(graph, e[0].fr, e[0].to))
    {
    	printf("\tDEBUG: Horizontal connection\n");
        if (is_connected(graph, e[0].fr, e[1].fr))
        {
        	printf("\t\tDEBUG: Type 1\n");
            if (!is_vertical_connection(graph, e[0].fr, e[1].fr))
                return 0;

            flag_connection = 1;
        }

        if (is_connected(graph, e[0].fr, e[1].to))
        {
			printf("\t\tDEBUG: Type 2\n");
            if (flag_connection || !is_vertical_connection(graph, e[0].fr, e[1].to))
                return 0;

            flag_connection = 1;
        }

        if (!flag_connection)
            return 0;
    }
    else // North-South relation
    {
    	printf("DEBUG: Vertical relation\n");
        if (is_connected(graph, e[0].fr, e[1].fr))
        {
			printf("\t\tDEBUG: Type 1\n");
            if (!is_horizontal_connection(graph, e[0].fr, e[1].fr))
                return 0;

            flag_connection = 1;
        }

        if (is_connected(graph, e[0].fr, e[1].to))
        {
			printf("\t\tDEBUG: Type 2\n");
            if (flag_connection || !is_horizontal_connection(graph, e[0].fr, e[1].to))
                return 0;

            flag_connection = 1;
        }

        if (!flag_connection)
            return 0;
    }
	printf("DEBUG: Vertices relationship is alright\n");
    // Check if players aren't blocked
    struct graph_t* graph_with_wall = graph_copy(graph);
    printf("\tDEBUG: Graph copied\n");
    add_wall(graph_with_wall, e);
    printf("\tDEBUG: Wall added\n");
    if (is_player_blocked(graph_with_wall, p1_position, BLACK)
        || is_player_blocked(graph_with_wall, p2_position, WHITE))
	{
		graph_free(graph_with_wall);
    	return 0;
	}
	graph_free(graph_with_wall);
    printf("DEBUG: Players aren't blocked\n");
	printf("DEBUG: Legal move\n");
    // The wall doesn't violate any rules, hence it's a legal move
    return 1;
}

int is_vertex_available(size_t vertex, size_t p1_position, size_t p2_position)
{
    return vertex != p1_position && vertex != p2_position;
}

int is_connection_existing(struct graph_t* graph, size_t i, int direction)
{
    for (size_t j = 0; j < graph->num_vertices; j++)
        if (get_connection_type(graph, i, j) == direction)
            return 1;

    return 0;
}

int can_player_move_to(struct graph_t* graph, size_t vertex, enum color_t active_player, size_t p1_position, size_t p2_position)
{
    // Check if position is on the board
    if (!is_vertex_in_graph(graph, vertex))
        return 0;

    // Check if the opponent isn't already on it
    if (!is_vertex_available(vertex, p1_position, p2_position))
        return 0;

    size_t initial_position = active_player == 0 ? p1_position : p2_position;
    size_t opponent_position = active_player == 0 ? p2_position : p1_position;

    // The position is directly accessible, hence the player can move to it
    if (is_connected(graph, initial_position, vertex))
        return 1;

    // The position isn't directly accessible
    // Check if player are facing each other, and can jump to the position
    if (is_connected(graph, initial_position, opponent_position))
    {
        // Check if the opponent is next to the position
        if (!is_connected(graph, opponent_position, vertex))
            return 0;

        // The opponent is next to the position

        // Check if position is behind the opponent
        if (get_connection_type(graph, initial_position, opponent_position) == get_connection_type(graph, opponent_position, vertex))
            return 1;

        // There's a wall/edge of the board behind the opponent player
        if (!is_connection_existing(graph, opponent_position, get_connection_type(graph, initial_position, opponent_position)))
            return 1;
    }

    // The position isn't accessible
    return 0;
}

int is_move_legal(struct graph_t* graph, struct move_t* move, size_t p1_position, size_t p2_position)
{
    if (move->t == WALL)
        return can_add_wall(graph, move->e, p1_position, p2_position);
    else if (move->t == MOVE)
        return can_player_move_to(graph, move->m, move->c, p1_position, p2_position);
    else
        return 0;
}