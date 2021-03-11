#include <string.h>

#include "annex_function.h"
#include "player.h"

struct player {
  char *name;
  struct graph_t *graph;
  enum color_t id;
};

struct player player = {.name= NULL, .graph = NULL, .id = -1};

char const* get_player_name()
{
  player.name = "random strategy";
  return player.name;
}

void initialize_graph(struct graph_t* graph)
{
  if (player.graph == NULL)
    player.graph = graph_copy(graph);
}

void initialize_color(enum color_t id)
{
  if (player.id == -1)
    player.id = id;
}

struct move_t get_first_move()
{
  size_t memory[player.graph->num_vertices];
  size_t inc = 0;
  for (size_t index = 0; index < player.graph->num_vertices; index++) {
    if (gsl_spmatrix_get(player.graph->o, player.id, index) == 1) {
      memory[inc++] = index;
    }
  }
  size_t random_index = (size_t) rand() % inc;
  player.graph->p[player.id] = memory[random_index];
  
  struct move_t first_move = {.m = player.graph->p[player.id], .e = {{-1,-1}},
			       .t = NO_TYPE, .c = player.id}; 
  return first_move;
}


struct move_t get_new_move(struct move_t previous_move)
{
  size_t nb_pos = 0;
  size_t pos[player.graph->num_vertices];

  for (size_t j = 0; j < player.graph->num_vertices; j++) {
    size_t i = player.graph->p[player.id];
    if (gsl_spmatrix_get(player.graph->t, i, j) > 0 && j != player.graph->p[previous_move.c])
      pos[nb_pos++] = j;
  }

  size_t ind = (size_t) rand() % nb_pos;
  player.graph->p[player.id] = pos[ind];

  struct move_t new_move = {.m = player.graph->p[player.id], .e = {{-1,-1}},
			    .t = MOVE, .c = player.id};
  return new_move;
}


struct move_t play(struct move_t previous_move)
{
  if (player.graph->p[player.id] == player.graph->num_vertices)
    return get_first_move();

  if (previous_move.t == MOVE) {
    player.graph->p[previous_move.c] = previous_move.m;
  }

  return get_new_move(previous_move);
}


void finalize()
{
  printf("The game is finished");
  gsl_spmatrix_free(player.graph->t);
  gsl_spmatrix_free(player.graph->o);
  free(player.graph);
}
