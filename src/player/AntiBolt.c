#include <string.h>
#include <math.h>
#include "utils.h"
#include "player.h"
#include "correlation_graph.h"

#define UNINITIALIZED 0

struct player
{
    struct graph_t *graph; //graph of the player
    struct near_neighbours *neighbours_graph;
    size_t position[2]; //position of the two players on the board
    size_t num_walls;   //number of walls in the hand of the player
    int best_direction;
    enum color_t id; //id of the player
};

static struct player player = {
    .graph = NULL,
    .neighbours_graph = NULL,
    .position = {UNINITIALIZED, UNINITIALIZED},
    .num_walls = UNINITIALIZED,
    .best_direction = NO_DIRECTION,
    .id = -1
};

/* 
 * Return the name of the player strategy
 */
char const *get_player_name()
{
    return "AntiBolt";
}

/**
 * Initialize the player with informations given by the server if he is not already initialize :
 * @param id : the id of the player : WHITE or BLACK
 * @param graph : the graph to play with
 * @param num_walls : the number of walls in the hand of the player
 */
void initialize(enum color_t id, struct graph_t *graph, size_t num_walls)
{
    static int is_initialized = 0;

    if (!is_initialized)
    {
        player.graph = graph_copy(graph);

        player.id = id;

        player.num_walls = num_walls;
        if (id == BLACK)
            player.best_direction = SOUTH;
        else
            player.best_direction = NORTH;
        player.position[BLACK] = graph->t->size1;
        player.position[WHITE] = graph->t->size1;
        player.neighbours_graph = get_correlated_graph(player.graph);
        graph_free(graph);
        is_initialized = 1;
    }
}

/*
 * Initialize and set a move
 * - position : the new position of the player
 * - edge1 : the first edge of the placed wall if a wall has been placed
 * - edge2 : the second edge of the wall
 * - id : the id of the player
 * - movetype : type of the move
 */
struct move_t set_move(size_t position, struct edge_t edge1, struct edge_t edge2, size_t id, enum movetype_t movetype)
{
    struct move_t move;
    move.m = position;
    move.e[0] = edge1;
    move.e[1] = edge2;
    move.c = id;
    move.t = movetype;

    return move;
}

/**
 * Return the first move for a player : the player is put on one of his own vertices
 */
struct move_t get_first_move()
{
    size_t starting_positions[player.graph->o->size2];
    size_t nb_pos = 0;

    //searching for available positions
    for (size_t index = 0; index < player.graph->o->size2; index++)
        if (is_owned(player.graph, player.id, index))
            starting_positions[nb_pos++] = index;

    //choice of an available position
    size_t random_index = (size_t)rand() % nb_pos;

    //move to the position
    return set_move(starting_positions[random_index], no_edge(), no_edge(), player.id, MOVE);
}

/**
 * Look if a player can reach a winning position while going straight
 * independantly of walls
 * @param graph Represent a board
 * @param position Current position of the player
 * @param id ID of a player
 * @return vertex' index if he can, invalid index else
 */
size_t can_fast_forward(struct graph_t *graph, size_t position, enum color_t id)
{
    size_t vertex = position;
    enum color_t other_player = get_next_player(id);

    vertex = (id == BLACK) ? player.neighbours_graph[vertex].south : player.neighbours_graph[vertex].north;
    while (is_vertex_in_graph(graph, vertex))
    {
        if (is_owned(graph, other_player, vertex))
            return vertex;
        vertex = (id == BLACK) ? player.neighbours_graph[vertex].south : player.neighbours_graph[vertex].north;
    }
    return graph->t->size1;
}

/**
 * Return a move with WALL type if the other player can win by going straight
 * a move with NO_TYPE type else
 */
struct move_t get_wall()
{
    size_t position_player_1 = player.position[player.id];
    size_t position_player_2 = player.position[get_next_player(player.id)];

    size_t place = can_fast_forward(player.graph, position_player_2, get_next_player(player.id));
    if (player.num_walls > 0 && is_vertex_in_graph(player.graph, place))
    {

        struct edge_t e;
        struct edge_t e1;
        struct edge_t e2;
        if (player.id == WHITE)
        {
            e.fr = place;
            e.to = player.neighbours_graph[place].north;
            e1.fr = player.neighbours_graph[place].east;
            e1.to = player.neighbours_graph[player.neighbours_graph[place].north].east;
            e2.fr = player.neighbours_graph[place].west;
            e2.to = player.neighbours_graph[player.neighbours_graph[place].north].west;
        }
        else
        {
            e.fr = place;
            e.to = player.neighbours_graph[place].south;
            e1.fr = player.neighbours_graph[place].east;
            e1.to = player.neighbours_graph[player.neighbours_graph[place].south].east;
            e2.fr = player.neighbours_graph[place].west;
            e2.to = player.neighbours_graph[player.neighbours_graph[place].south].west;
        }

        struct edge_t edge1[2] = {e, e1};
        struct edge_t edge2[2] = {e, e2};

        if (can_add_wall(player.graph, edge1, position_player_1, position_player_2))
        {
            player.num_walls -= 1;
            return set_move(player.position[player.id], e, e1, player.id, WALL);
        }
        if (can_add_wall(player.graph, edge2, position_player_1, position_player_2))
        {
            player.num_walls -= 1;
            return set_move(player.position[player.id], e, e2, player.id, WALL);
        }
    }

    return set_move(player.position[player.id], no_edge(), no_edge(), player.id, NO_TYPE);
}

/**
 * Complete the vertices to test around the position of the player
 * @param vertices_to_test Array of vertices to be tested
 * @param position Player's position
 */
void get_vertices_to_test(size_t vertices_to_test[], size_t position)
{
    if (player.best_direction == NORTH)
    {
        vertices_to_test[0] = player.neighbours_graph[player.neighbours_graph[position].north].north;
        vertices_to_test[1] = player.neighbours_graph[position].north;
        vertices_to_test[2] = player.neighbours_graph[player.neighbours_graph[position].north].west;
        vertices_to_test[3] = player.neighbours_graph[player.neighbours_graph[position].north].east;
        vertices_to_test[4] = player.neighbours_graph[position].west;
        vertices_to_test[5] = player.neighbours_graph[position].east;
        vertices_to_test[6] = player.neighbours_graph[player.neighbours_graph[position].west].west;
        vertices_to_test[7] = player.neighbours_graph[player.neighbours_graph[position].east].east;
        vertices_to_test[8] = player.neighbours_graph[position].south;
        vertices_to_test[9] = player.neighbours_graph[player.neighbours_graph[position].south].west;
        vertices_to_test[10] = player.neighbours_graph[player.neighbours_graph[position].south].east;
        vertices_to_test[11] = player.neighbours_graph[player.neighbours_graph[position].south].south;
    }
    else
    {
        vertices_to_test[0] = player.neighbours_graph[player.neighbours_graph[position].south].south;
        vertices_to_test[1] = player.neighbours_graph[position].south;
        vertices_to_test[2] = player.neighbours_graph[player.neighbours_graph[position].south].east;
        vertices_to_test[3] = player.neighbours_graph[player.neighbours_graph[position].south].west;
        vertices_to_test[4] = player.neighbours_graph[position].east;
        vertices_to_test[5] = player.neighbours_graph[position].east;
        vertices_to_test[6] = player.neighbours_graph[player.neighbours_graph[position].east].west;
        vertices_to_test[7] = player.neighbours_graph[player.neighbours_graph[position].west].west;
        vertices_to_test[8] = player.neighbours_graph[position].north;
        vertices_to_test[9] = player.neighbours_graph[player.neighbours_graph[position].north].west;
        vertices_to_test[10] = player.neighbours_graph[player.neighbours_graph[position].north].west;
        vertices_to_test[11] = player.neighbours_graph[player.neighbours_graph[position].north].north;
    }
}

/**
 * Return the next move to do
 */
struct move_t get_new_move()
{
    struct move_t wall = get_wall();
    if (wall.t != NO_TYPE)
        return wall;

    size_t position_player1 = player.position[BLACK];
    size_t position_player2 = player.position[WHITE];

    size_t vertices_to_test[12];
    get_vertices_to_test(vertices_to_test, player.position[player.id]);

    for (size_t i = 0; i < 12; i++)
        if (can_player_move_to(player.graph, vertices_to_test[i], player.id, position_player1, position_player2))
            return set_move(vertices_to_test[i], no_edge(), no_edge(), player.id, MOVE);

    return set_move(0, no_edge(), no_edge(), player.id, NO_TYPE);
}

/**
 * Return true if the player is not placed yet.
 */
int is_first_move()
{
    return player.position[player.id] == player.graph->t->size1;
}

/**
 * Update the player with the move of the other player and return a move
 * @param previous_move : the previous move of the other player
 */
void update(struct move_t previous_move)
{
    if (previous_move.t == MOVE)
        player.position[previous_move.c] = previous_move.m;

    else if (previous_move.t == WALL)
        add_wall(player.graph, previous_move.e);
}

/**
 * Return the move of the active player
 * @param previous_move : previous move of the other player
 */
struct move_t play(struct move_t previous_move)
{
    update(previous_move);

    struct move_t move_to_do;
    if (is_first_move())
        move_to_do = get_first_move();
    else
        move_to_do = get_new_move();

    update(move_to_do);
    return move_to_do;
}

/*
 * Free the player
 */
void finalize()
{
    free_correlation_graph(player.neighbours_graph);
    graph_free(player.graph);
}
