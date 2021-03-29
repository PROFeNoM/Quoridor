#include <stdio.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spblas.h>

#include "graph_pattern.h"

int get_direction_square(size_t i,size_t j, size_t m)
{
  if (i == j+1 && (i/m == j/m)){
    return WEST;
  }
  if (i == j-1 && (i/m == j/m)){
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


gsl_spmatrix * square_graph(size_t m)
{
  gsl_spmatrix * matrix = gsl_spmatrix_alloc(m*m,m*m);
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      gsl_spmatrix_set(matrix,i,j,get_direction_square(i,j,m));
    }
  }
  return matrix;
}

gsl_spmatrix * matrix_position(size_t m)
{
  size_t num_vertices = m*m;
  gsl_spmatrix * matrix = gsl_spmatrix_alloc(2, num_vertices);
  
  for (size_t i = 0 ; i < m ; i++){
    gsl_spmatrix_set(matrix, 0, i, 1);
    gsl_spmatrix_set(matrix, 1, num_vertices-1-i,1);
  }
  
  return matrix;
}

struct graph_t *get_graph(char type, size_t width)
{
  gsl_spmatrix *matrix = NULL;
  gsl_spmatrix *matrix_pos = NULL;
  if (type == 'c') {
    matrix = square_graph(width);
    matrix_pos = matrix_position(width);
  }
  
  struct graph_t *graph = malloc(sizeof(struct graph_t));
  graph->num_vertices = width * width;
  graph->t = matrix;
  graph->o = matrix_pos;
  return graph;
}