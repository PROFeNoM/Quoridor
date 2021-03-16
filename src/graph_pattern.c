#include <stdio.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spblas.h>

#include "graph_pattern.h"

int get_direction_square(size_t i,size_t j,int m){

  if (i == j+1 && (i/m==j/m)){
    return WEST;
  }
  if (i == j-1 &&  (i/m==j/m)){
    return EAST;
  }
  if (i+m == j){
    return SOUTH;
  }
  if (i-m == j){
    return NORTH;
  }
  return NO_DIRECTION;
}
  

gsl_spmatrix * square_graph(int m){
  gsl_spmatrix * matrix = gsl_spmatrix_alloc(m*m,m*m);
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      gsl_spmatrix_set(matrix,i,j,get_direction_square(i,j,m));
    }
  }
  return matrix;
}

gsl_spmatrix * matrix_position(int m){
  size_t num_vertices = m*m;
  gsl_spmatrix * matrix = gsl_spmatrix_alloc(2, num_vertices);
  
  for (size_t i = 0 ; i < m ; i++){
    gsl_spmatrix_set(matrix, 0, i, 1);
    gsl_spmatrix_set(matrix, 1, num_vertices-1-i,1);
  }
  
  return matrix;
}

struct graph_t * initialize_graph(int m){
  gsl_spmatrix * matrix = square_graph(m);
  gsl_spmatrix * matrix_pos = matrix_position(m);
  struct graph_t * graph = malloc(sizeof(struct graph *)) ;
  graph->num_vertices = m*m;
  graph->t = matrix;
  graph->o = matrix_pos;
  return graph;
}
    
enum color_t get_next_player(enum color_t id){
  if (id == BLACK){
    return WHITE;
  }
  else{
    return BLACK;
  }
}

int is_winning(struct player_server * players, struct graph_t * graph, enum color_t id){
  return gsl_spmatrix_get(graph->o, get_next_player(id), players[id].pos);
}

void update(struct player_server * players,struct move_t move){
  players[move.c].pos = move.m;
}


