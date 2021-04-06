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


gsl_spmatrix_uint * square_graph(size_t m)
{
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(m*m,m*m);
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      gsl_spmatrix_uint_set(matrix,i,j,get_direction_square(i,j,m));
    }
  }
  return matrix;
}

gsl_spmatrix_unint * t_graph(size_t m){
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(m*m,m*m);
  for (size_t i = 0 ; i < m  ; i++){
    for (size_t j = 0 ; j < m ; j++){
      if ( !(i>=m/3 && i<(2*m/3)) && !(j>=m/3 && j<(2*m/3))){
	gsl_spmatrix_uint_set(matrix,i,j,get_direction_square(i,j,m));
      }
      else {
	gsl_spmatrix_uint_set(matrix,i,j,NO_DIRECTION,);
      }
    }
  }
  return matrix;
}

gsl_spmatrix_unint * h_graph(size_t m){
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(m*m,m*m);
  for (size_t i = 0 ; i < m  ; i++){
    for (size_t j = 0 ; j < m ; j++){
      if ( (!(i<m/3 || i>=2*m/3) && !(j>=m/3 && j<(2*m/3)))){
	gsl_spmatrix_uint_set(matrix,i,j,get_direction_square(i,j,m));
      }
      else {
	gsl_spmatrix_uint_set(matrix,i,j,NO_DIRECTION,);
      }
    }
  }
  return matrix;
}

gsl_spmatrix_unint * s_graph(size_t m){
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(m*m,m*m);
  for (size_t i = 0 ; i < m/5  ; i++){
    for (size_t j = 0 ; j < m ; j++){
      if (j<m/5){
	gsl_spmatrix_uint_set(matrix,i,j,get_direction_square(i,j,m));
	gsl_spmatrix_uint_set(matrix,m-1-i,j,get_direction_square(m-1-i,j,m));
      }
      else {
	gsl_spmatrix_uint_set(matrix,i,j,NO_DIRECTION,);
	gsl_spmatrix_uint_set(matrix,m-1-i,j,NO_DIRECTION,);
      }
    }
  }
  for (size_t i = m/5 ; i < 2*m/5  ; i++){
    for (size_t j = 0 ; j < m ; j++){
	gsl_spmatrix_uint_set(matrix,i,j,get_direction_square(i,j,m));
	gsl_spmatrix_uint_set(matrix,m-1-i,j,get_direction_square(m-1-i,j,m));
    }
  }
  for (size_t i = 2*m/5 ; i < 3*m/5  ; i++){
    for (size_t j = 0 ; j < m ; j++){
      if (j>=4*m/5){
	gsl_spmatrix_uint_set(matrix,i,j,get_direction_square(i,j,m));
      }
      else {
	gsl_spmatrix_uint_set(matrix,i,j,NO_DIRECTION,);
      }
    }
  }
  return matrix;
}

gsl_spmatrix_uint * matrix_position(size_t m)
{
  size_t num_vertices = m*m;
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(2, num_vertices);
  
  for (size_t i = 0 ; i < m ; i++){
    gsl_spmatrix_uint_set(matrix, 0, i, 1);
    gsl_spmatrix_uint_set(matrix, 1, num_vertices-1-i,1);
  }
  
  return matrix;
}

gsl_spmatrix_uint * h_matrix_position(size_t m)
{
  size_t num_vertices = m*m;
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(2, num_vertices);
  
  for (size_t i = 0 ; i < m ; i++){
    if (i<m/3 || i >= 2*m/3){
      gsl_spmatrix_uint_set(matrix, 0, i, 1);
      gsl_spmatrix_uint_set(matrix, 1, num_vertices-1-i,1);
    }
  }
  
  return matrix;
}

gsl_spmatrix_uint * s_matrix_position(size_t m)
{
  size_t num_vertices = m*m;
  gsl_spmatrix_uint * matrix = gsl_spmatrix_uint_alloc(2, num_vertices);
  
  for (size_t i = 0 ; i < m/5 ; i++){
    if (i<m/3 || i >= 2*m/3){
      gsl_spmatrix_uint_set(matrix, 0, i, 1);
      gsl_spmatrix_uint_set(matrix, 1, num_vertices-1-4*m/5-i,1);
  }
  
  return matrix;
}

  

struct graph_t *get_graph(char type, size_t width)
{
  gsl_spmatrix_uint *matrix = NULL;
  gsl_spmatrix_uint *matrix_pos = NULL;
  if (type == 'c') {
    matrix = square_graph(width);
    matrix_pos = matrix_position(width);
  }
  if (type == 't') {
    matrix = t_graph(width);
    matrix_pos = matrix_position(width);
  }
  if (type == 'h') {
    matrix = h_graph(width);
    matrix_pos = h_matrix_position(width);
  }
  if (type == 's') {
    matrix = s_graph(width);
    matrix_pos = s_matrix_position(width);
  }
  
  struct graph_t *graph = malloc(sizeof(struct graph_t));
  graph->num_vertices = width * width;
  graph->t = matrix;
  graph->o = matrix_pos;
  return graph;
}
