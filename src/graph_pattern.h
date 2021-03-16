#ifndef _GRAPH_PATTERN_H_
#define _GRAPH_PATTERN_H_

#include "graph.h"


#define FIRST_MOVE (struct move_t)({.m = 0 , .e = {-1,-1} ,.t = NO_TYPE , .c=NO_COLOR})

enum direction_t {NO_DIRECTION=0,NORTH,SOUTH,WEST,EAST};

struct player_server {
  char const * (* get_player_name)(void);
  void (*initialize)(enum color_t id,struct graph_t *graph,size_t num_walls);
  struct move_t (*play)(struct move_t previous_move);
  size_t pos;
};


// get the index of direction of j according to i
int get_direction_square(size_t i,size_t j,int m);
// return the spmatrix of a square graph of size m by m
gsl_spmatrix * square_graph(int m);
gsl_spmatrix * matrix_position(int m);
struct graph_t * initialize_graph(int m);
enum color_t get_next_player(enum color_t);
int is_winning(struct player_server, struct graph_t * graph, enum color_t id);
void update(struct player_server * players,struct move_t move);


#endif // _GRAPH_PATTERN_H_
