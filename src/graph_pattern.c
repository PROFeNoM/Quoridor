#include <stdio.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spblas.h>

#include "graph_pattern.h"


int get_number_edge_square(size_t i,size_t j,int m){

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
      gsl_spmatrix_set(matrix,i,j,get_number_edge_square(i,j,m));
    }
  }
  return matrix;
}

gsl_spmatrix * matrix_position(int m){
  gsl_spmatrix * matrix = gsl_spmatrix_alloc(2*m,m*m);
  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      gsl_spmatrix_set(matrix,i,j,get_number_edge_square(i,j,m));
    }
  }
  return matrix;
}
  


void main(void){

  int m = 2;
  gsl_spmatrix * matrix = square_graph(m);

  for (size_t i = 0 ; i < m*m ; i++){
    for (size_t j = 0 ; j < m*m ; j++){
      printf(" %f ",gsl_spmatrix_get(matrix,i,j));
    }
    printf("\n");
  }
}
    







