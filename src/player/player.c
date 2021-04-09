#include <string.h>

#include "utils.h"
#include "player.h"

#define UNINITIALIZED 0

struct player {
  char *name;
  struct graph_t *graph;
  size_t position[2];
  size_t num_walls;
  enum color_t id;
};

struct player player = {.name= NULL, .graph = NULL, .position = {UNINITIALIZED, UNINITIALIZED},
			.id = -1, .num_walls = UNINITIALIZED};

char const* get_player_name()
{
  player.name = "randomStrat";
  return player.name;
}

void initialize(enum color_t id, struct graph_t* graph, size_t num_walls)
{
  static int is_initialized = 0;

  if (!is_initialized) {
    player.graph = graph_copy(graph);  
  
    player.id = id;

    player.num_walls = num_walls;
  
    player.position[0] = graph->num_vertices;
    player.position[1] = graph->num_vertices;
  }
  else
    is_initialized = 1;
}


struct move_t get_first_move()
{
  size_t starting_positions[player.graph->num_vertices];
  size_t nb_pos = 0;
  for (size_t index = 0; index < player.graph->num_vertices; index++) {
    if (is_owned(player.graph, player.id, index) == 1) {
      starting_positions[nb_pos++] = index;
    }
  }
 
  size_t random_index = (size_t) rand() % nb_pos;
  player.position[player.id] = starting_positions[random_index];
  
  struct move_t first_move = {.m = player.position[player.id], .e = {{-1,-1}},
			       .t = MOVE, .c = player.id}; 
  return first_move;
}


struct move_t get_new_move(struct move_t previous_move)
{
  size_t nb_pos = 0;
  size_t pos[player.graph->num_vertices];

  size_t position_player1 = player.position[player.id];
  size_t position_player2 = player.position[previous_move.c];
  
  for (size_t j = 0; j < player.graph->num_vertices; j++) {
    if (can_player_move_to(player.graph, j, player.id, position_player1, position_player2))
      pos[nb_pos++] = j;
  }

  size_t ind = (size_t) rand() % nb_pos;
  player.position[player.id] = pos[ind];

  struct move_t new_move = {.m = position_player1, .e = {{-1,-1}},
			    .t = MOVE, .c = player.id};
  return new_move;
}

int is_first_move()
{
  return player.position[player.id] == player.graph->num_vertices;
}

void update(struct move_t previous_move)
{
  if (previous_move.t == MOVE) {
    player.position[previous_move.c] = previous_move.m;
  }

  else if (previous_move.t == WALL) {
    add_wall(player.graph, previous_move.e);
  }
}



struct move_t play(struct move_t previous_move)
{
  if (is_first_move())
    return get_first_move();

  update(previous_move);

  return get_new_move(previous_move);
}


void finalize()
{
  graph_free(player.graph);
}
