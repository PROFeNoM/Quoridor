#include <math.h>
#include <string.h>

#include "utils.h"
#include "player.h"
#include "correlation_graph.h"

#define UNINITIALIZED 0

struct player
{
  struct graph_t *graph; //graph of the player
  struct near_neighbours *neighbours_graph;
  size_t position[2];    //position of the two players on the board
  size_t num_walls;      //number of walls in the hand of the player
  enum direction_t best_direction;    //the direction of the sprint for the player
  enum color_t id;       //id of the player
};

struct player player = {.graph = NULL, .best_direction = NO_DIRECTION, .position = {UNINITIALIZED, UNINITIALIZED}, .id = -1, .num_walls = UNINITIALIZED};

/**
 * Return the name of the player strategy
 */
char const *get_player_name()
{
    return "ForrestGump";
}

/**
 * Initialize the player with informations given by the server if he is not already initialize
 * @param id The id of the player : WHITE or BLACK
 * @param graph The graph to play with
 * @param num_walls The number of walls in the hand of the player
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
	
        player.position[BLACK] = graph->num_vertices;
        player.position[WHITE] = graph->num_vertices;

	player.neighbours_graph = get_correlated_graph(player.graph);
	
        graph_free(graph);
    }
    else
        is_initialized = 1;
}

/**
 * Initialize and set a move
 * @param position The new position of the player
 * @param edge1 The first edge of the placed wall if a wall has been placed
 * @param edge2 The second edge of the wall
 * @param id  The id of the player
 * @param movetype  Type of the move
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
    size_t starting_positions[player.graph->num_vertices];
    size_t nb_pos = 0;

    //searching for available positions
    for (size_t index = 0; index < player.graph->num_vertices; index++)
    {
        if (is_owned(player.graph, player.id, index))
        {
            starting_positions[nb_pos++] = index;
        }
    }

    //choice of an available position
    size_t random_index = (size_t)rand() % nb_pos;
    player.position[player.id] = starting_positions[random_index];

    //move to the position
    struct move_t first_move = set_move(player.position[player.id], no_edge(), no_edge(), player.id, MOVE);

    return first_move;
}

/**
 * Return an int symbolizingthe direction of the player
 * @param best_direction The direction of the player to win the game
 * @return 1 if the direction is SOUTH, -1 otherwise
 */
int sign_direction(enum direction_t best_direction)
{
  return (1 ? best_direction == SOUTH : -1);
}

/**
 * Return if available positions exists in the indexes tabular given in parameter
 * @param nb_ind The size of the indexes tabular
 * @param indexes The tabular with position to be tested
 * @param pos The tabular to be update with available positions
 * @param nb_pos The number of available positions in pos to be update
 */
int exist_available_indexes(int nb_ind, size_t indexes[], size_t pos[], size_t *nb_pos)
{
  size_t position_player1 = player.position[BLACK];
  size_t position_player2 = player.position[WHITE];

  for (int i = 0; i < nb_ind; i++) {
    if (can_player_move_to(player.graph, indexes[i], player.id, position_player1, position_player2)) {
      pos[*nb_pos] = indexes[i];
      *nb_pos += 1;
    }
  }

  if (*nb_pos > 0)
    return 1;

  else
    return 0;
}

/**
 * Complete the vertices to test around the position of the player
 * @param vertices_to_test Array of vertices to be tested
 * @param position Player's position
 */
void get_vertices_to_test(size_t vertices_to_test[], size_t position)
{
  if (player.best_direction == NORTH) {
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

  else {
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
 * Return a move for the current player
 */
struct move_t get_new_move()
{
  size_t position = player.position[player.id];

  size_t position_player1 = player.position[BLACK];
  size_t position_player2 = player.position[WHITE];

  size_t vertices_to_test[12];
  get_vertices_to_test(vertices_to_test, position);

  for (size_t i = 0; i < 12; i++) {
    if (can_player_move_to(player.graph, vertices_to_test[i], player.id, position_player1, position_player2)) {
      return set_move(vertices_to_test[i], no_edge(), no_edge(), player.id, MOVE);
    }
  }
  return set_move(0, no_edge(), no_edge(), player.id, NO_TYPE);
}


/**
 * Return true if the player is not placed yet, false otherwise
 */
int is_first_move()
{
    return player.position[player.id] == player.graph->num_vertices;
}

/*
 * Update the graph of the player with the move of the other player
 */
void update(struct move_t previous_move)
{
    if (previous_move.t == MOVE)
    {
        player.position[previous_move.c] = previous_move.m;
    }

    else if (previous_move.t == WALL)
    {
        add_wall(player.graph, previous_move.e);
    }
}

/*
 * Return the move of the active player
 * - previous_move : previous move of the other player
 */
struct move_t play(struct move_t previous_move)
{
    if (is_first_move())
        return get_first_move();

    update(previous_move);

    struct move_t move = get_new_move();
    update(move);
    
    return move;
}

/*
 * Free the player
 */
void finalize()
{
    graph_free(player.graph);
}
