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
  player.name = "random strategy";
  return player.name;
}

void initialize(enum color_t id, struct graph_t* graph, size_t num_walls)
{
  static int is_initialized = 0;

  if (!is_initialized) {
    player.graph = graph_copy(graph);  
  
    player.id = id;

    player.num_walls = num_walls;
  
    player.position[BLACK] = graph->num_vertices;
    player.position[WHITE] = graph->num_vertices;
    graph_free(graph);
  }

  else
    is_initialized = 1;
}

struct move_t get_first_move()
{
  size_t memory[player.graph->num_vertices];
  size_t inc = 0;
  for (size_t index = 0; index < player.graph->num_vertices; index++) {
    if (gsl_spmatrix_uint_get(player.graph->o, player.id, index) == 1) {
      memory[inc++] = index;
    }
  }
  size_t random_index = (size_t) rand() % inc;
  player.position[player.id] = memory[random_index];
  
  struct move_t first_move = {.m = player.position[player.id], .e = {{-1,-1}},
			       .t = MOVE, .c = player.id}; 
  return first_move;
}


struct move_t get_new_move(struct move_t previous_move)
{
  size_t nb_pos = 0;
  size_t pos[player.graph->num_vertices];

  size_t i = player.position[player.id];
  
  for (size_t j = 0; j < player.graph->num_vertices; j++) {
    if (gsl_spmatrix_uint_get(player.graph->t, i, j) > 0 && j != player.position[previous_move.c])
      pos[nb_pos++] = j;
    if (gsl_spmatrix_uint_get(player.graph->t, i, j) > 0 && j == player.position[previous_move.c])
      for (size_t k = 0; k < player.graph->num_vertices; k++) {
	if (gsl_spmatrix_uint_get(player.graph->t, i, j) == gsl_spmatrix_uint_get(player.graph->t, j, k))
	  pos[nb_pos++] = k;
	else if (gsl_spmatrix_uint_get(player.graph->t, j, k) > 0 && k != i)
	  pos[nb_pos++] = k;
      }
  }

  size_t ind = (size_t) rand() % nb_pos;
  player.position[player.id] = pos[ind];

  struct move_t new_move = {.m = player.position[player.id], .e = {{-1,-1}},
			    .t = MOVE, .c = player.id};
  return new_move;
}


struct move_t play(struct move_t previous_move)
{
  if (player.position[player.id] == player.graph->num_vertices)
    return get_first_move();

  if (previous_move.t == MOVE) {
    player.position[previous_move.c] = previous_move.m;
  }

  return get_new_move(previous_move);
}


void finalize()
{
  graph_free(player.graph);
}
