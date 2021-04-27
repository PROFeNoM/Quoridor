#include <string.h>
#include <math.h>
#include "utils.h"
#include "player.h"
#include "dijsktra.h"

#define UNINITIALIZED 0

struct player
{
  struct graph_t *graph; //graph of the player
  size_t position[2];    //position of the two players on the board
  size_t num_walls;      //number of walls in the hand of the player
  size_t max_walls ;
  size_t col;
  enum color_t id;       //id of the player
};

struct player player = {.graph = NULL, .position = {UNINITIALIZED, UNINITIALIZED}, .id = -1, .num_walls = UNINITIALIZED};

/* 
 * Return the name of the player strategy
 */
char const *get_player_name()
{
    return "IntelligentAntiBolt";
}

/*
 * Initialize the player with informations given by the server if he is not already initialize :
 * - id : the id of the player : WHITE or BLACK
 * - graph : the graph to play with
 * - num_walls : the number of walls in the hand of the player
 */
void initialize(enum color_t id, struct graph_t *graph, size_t num_walls)
{
    static int is_initialized = 0;

    if (!is_initialized)
    {
        player.graph = graph_copy(graph);

        player.id = id;

        player.num_walls = num_walls;
	player.max_walls = num_walls;
	player.col = graph->num_vertices;
        player.position[BLACK] = graph->num_vertices;
        player.position[WHITE] = graph->num_vertices;
        graph_free(graph);
    }
    else
        is_initialized = 1;
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

/*
 * Return the first move for a player : the player is put on one of his own vertices
 */
struct move_t get_first_move()
{
    size_t starting_positions[player.graph->num_vertices];
    size_t nb_pos = 0;

    //searching for available positions
    for (size_t index = 0; index < player.graph->num_vertices; index++)
    {
        if (is_owned(player.graph, player.id, index) == 1)
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

struct move_t get_wall(){

  size_t position_player_1;
  size_t position_player_2;
  size_t m = sqrt(player.graph->t->size1);

  position_player_1 = player.position[BLACK];
  position_player_2 = player.position[WHITE];
  
  
  if (player.num_walls != 0 && ((player.max_walls-player.num_walls) < m)){
    
    if (player.num_walls == player.max_walls || player.col != position_player_2 % m){

      player.col = player.position[get_next_player(player.id)] % m;
      size_t num = player.position[get_next_player(player.id)] % m;
      struct edge_t e;
      struct edge_t e1;
      struct edge_t e2;
      size_t place;
      if (player.id == WHITE){
	place = (m*m)-(m-num);
	e.fr = place;
	e.to = place-m;	
	e1.fr = place+1;
	e1.to = place-m+1;
	e2.fr = place-1;
	e2.to = place-m-1;
      }
      else {
	place = num;
	e.fr = place;
	e.to = place+m;
	e1.fr = place+1;
	e1.to = place+m+1;
	e2.fr = place-1;
	e2.to = place+m-1;
      }
      
      struct edge_t edge1[2]={e,e1};
      struct edge_t edge2[2]={e,e2};
      //printf("\n\n\n can %d = \n",can_add_wall(player.graph,edge1,position_player_1,position_player_2));
      //printf("%ld to %ld | %ld to %ld \n",e.fr,e.to,e1.fr,e1.to);
      if (can_add_wall(player.graph,edge1,position_player_1,position_player_2) && (e.fr/m == e1.fr/m)){
	struct move_t new_move = set_move(player.position[player.id], e, e1, player.id, WALL);
	player.num_walls -= 1;
	
	
	return new_move;
      }
      if (can_add_wall(player.graph,edge2,position_player_1,position_player_2) && (e.fr/m == e2.fr/m)){
	struct move_t new_move = set_move(player.position[player.id], e, e2, player.id, WALL);
	player.num_walls -= 1;
	return new_move;
      }
    }
  }
    
  struct move_t new_move = set_move(player.position[player.id], no_edge(), no_edge(), player.id, NO_TYPE);
  return new_move;
}
	

  

/*
 * Update the player with the move of the other player and return a move
 * - previous_move : the previous move of the other player
 */
struct move_t get_new_move()
{
  
  
  struct move_t possibility = get_wall();
  if (possibility.t != NO_TYPE){
    return possibility;
  }

  
  size_t vertice = mini_dijsktra(player.graph,player.position[player.id],player.id);

    player.position[player.id] = vertice;

    //move to the position
    struct move_t new_move = set_move(player.position[player.id], no_edge(), no_edge(), player.id, MOVE);

    return new_move;
}

/*
 * Return true if the player is not placed yet.
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

    struct move_t newmove = get_new_move();

    

    update(newmove);

    return newmove;
}

/*
 * Free the player
 */
void finalize()
{
    graph_free(player.graph);
}
