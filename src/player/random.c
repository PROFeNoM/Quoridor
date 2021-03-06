#include <string.h>

#include "utils.h"
#include "player.h"

#define UNINITIALIZED 0

struct player
{
    struct graph_t *graph; //graph of the player
    size_t position[2];    //position of the two players on the board
    size_t num_walls;      //number of walls in the hand of the player
    enum color_t id;       //id of the player
};

struct player player = {
    .graph = NULL,
    .position = {UNINITIALIZED, UNINITIALIZED},
    .num_walls = UNINITIALIZED,
    .id = -1
};

/**
 * Return the name of the player strategy
 * @return the player name 
 */
char const *get_player_name()
{
    return "randomStrat";
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
        player.position[BLACK] = graph->t->size1;
        player.position[WHITE] = graph->t->size1;
        player.num_walls = num_walls;
        player.id = id;

        graph_free(graph);
        is_initialized = 1;
    }
}

/**
 * Initialize and set a move
 * @param position : the new position of the player
 * @param edge1 : the first edge of the placed wall if a wall has been placed
 * @param edge2 : the second edge of the wall
 * @param id : the id of the player
 * @param movetype : type of the move
 * @return the move set
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
 * @return the first move for a player
 */
struct move_t get_first_move()
{
    size_t starting_positions[player.graph->o->size2];
    size_t nb_pos = 0;

    //searching for available positions
    for (size_t index = 0; index < player.graph->o->size2; index++)
        if (is_owned(player.graph, player.id, index) == 1)
            starting_positions[nb_pos++] = index;

    //choice of an available position
    size_t random_index = (size_t)rand() % nb_pos;
    player.position[player.id] = starting_positions[random_index];

    //move to the position
    return set_move(player.position[player.id], no_edge(), no_edge(), player.id, MOVE);
}

/**
 * Complete the available positions to move for a player
 * @param nb_pos Number of available positions
 * @param pos Array of available positions
 * @param position_player1 Position of the player BLACK
 * @param position_player2 Position of the player WHITE
 */
void get_available_positions(size_t *nb_pos, size_t pos[], size_t position_player1, size_t position_player2)
{
  for (size_t j = 0; j < player.graph->t->size1; j++)
    if (can_player_move_to(player.graph, j, player.id, position_player1, position_player2)) {
      pos[*nb_pos] = j;
      *nb_pos += 1;
    }
}

/**
 * Update the player with the move of the other player and return a move
 * @param previous_move : the previous move of the other player
 * @return the player move 
 */
struct move_t get_new_move()
{
    size_t nb_pos = 0;
    size_t pos[player.graph->t->size1];

    size_t position_player1 = player.position[BLACK];
    size_t position_player2 = player.position[WHITE];

    //research of available positions
    get_available_positions(&nb_pos, pos, position_player1, position_player2);

    //choice of an available position
    size_t ind = (size_t)rand() % nb_pos;

    //move to the position
    return set_move(pos[ind], no_edge(), no_edge(), player.id, MOVE);
}

/**
 * Return true if the player is not placed yet.
 * @return true if the player is not placed yet.
 */
int is_first_move()
{
    return player.position[player.id] == player.graph->t->size1;
}

/**
 * Update the graph of the player with the move of the other player
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
 * @return the player move
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

/**
 * Free the player
 */
void finalize()
{
    graph_free(player.graph);
}
