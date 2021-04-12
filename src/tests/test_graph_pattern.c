#include <assert.h>

#include "graph_pattern.h"

#define TESTCASE(X, Y) printf("%s ... %s\n", X, Y() ? "PASSED" : "FAILED");

int test__get_direction_square()
{

  size_t i_1 = 0;
  size_t j_1 = 0;
  size_t m_1 = 5;
  
  assert(get_direction_square(i_1,j_1,m_1)==NO_DIRECTION);

  size_t i_2 = 0;
  size_t j_2 = 5;
  size_t m_2 = 5;

  assert(get_direction_square(i_2,j_2,m_2)==SOUTH);

  size_t i_3 = 4;
  size_t j_3 = 3;
  size_t m_3 = 5;

  assert(get_direction_square(i_3,j_3,m_3)==WEST);

  size_t i_4 = 3;
  size_t j_4 = 4;
  size_t m_4 = 5;

  assert(get_direction_square(i_4,j_4,m_4)==EAST);

  size_t i_5 = 7;
  size_t j_5 = 2;
  size_t m_5 = 5;

  assert(get_direction_square(i_5,j_5,m_5)==NORTH);

  size_t i_6 = 7;
  size_t j_6 = 2;
  size_t m_6 = 6;

  assert(get_direction_square(i_6,j_6,m_6)==NO_DIRECTION);

  size_t i_7 = 5;
  size_t j_7 = 6;
  size_t m_7 = 6;

  assert(get_direction_square(i_7,j_7,m_7)==NO_DIRECTION);

  return 1;
}

int test__square(){

  size_t x_1 = 5;
  size_t y_1 = 18;
  size_t m_1 = 5;

  assert(square(x_1,y_1,m_1)!=0);

  size_t x_2 = 22;
  size_t y_2 = 9;
  size_t m_2 = 18;

  assert(square(x_1,y_1,m_1)!=0);

  return 1;
}

int test__graph_square()
{
  size_t m = 3;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);

  int * vertex_1 = {0,2,4,5,6,7,8};

  for (int i = 0 ; i<7 ; i++){
    gsl_spmatrix_uint_set(matrix,0,vertex_1[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,0,1,EAST);
  gsl_spmatrix_uint_set(matrix,0,3,SOUTH);

  int * vertex_2 = {1,3,5,6,7,8};

  for (int i = 0 ; i<6 ; i++){
    gsl_spmatrix_uint_set(matrix,1,vertex_2[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,1,0,WEST);
  gsl_spmatrix_uint_set(matrix,1,2,EAST);
  gsl_spmatrix_uint_set(matrix,1,4,SOUTH);

  int * vertex_3 = {0,2,3,4,6,7,8};

  for (int i = 0 ; i<7 ; i++){
    gsl_spmatrix_uint_set(matrix,2,vertex_3[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,2,1,WEST);
  gsl_spmatrix_uint_set(matrix,2,5,SOUTH);

  int * vertex_4 = {1,2,3,7,8};

  for (int i = 0 ; i<5 ; i++){
    gsl_spmatrix_uint_set(matrix,3,vertex_4[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,3,0,NORTH);
  gsl_spmatrix_uint_set(matrix,3,6,SOUTH);
  gsl_spmatrix_uint_set(matrix,3,4,EAST);

  int * vertex_5 = {0,3,6,8};
  
  for (int i = 0 ; i<4 ; i++){
    gsl_spmatrix_uint_set(matrix,4,vertex_5[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,4,1,NORTH);
  gsl_spmatrix_uint_set(matrix,4,7,SOUTH);
  gsl_spmatrix_uint_set(matrix,4,5,EAST);
  gsl_spmatrix_uint_set(matrix,4,3,WEST);

  int * vertex_6 = {0,1,3,6,7};
  
  for (int i = 0 ; i<5 ; i++){
    gsl_spmatrix_uint_set(matrix,5,vertex_6[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,5,2,NORTH);
  gsl_spmatrix_uint_set(matrix,5,8,SOUTH);
  gsl_spmatrix_uint_set(matrix,5,4,WEST);

  int * vertex_7 = {0,1,2,4,5,8};
  
  for (int i = 0 ; i<6 ; i++){
    gsl_spmatrix_uint_set(matrix,6,vertex_7[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,6,3,NORTH);
  gsl_spmatrix_uint_set(matrix,6,7,EAST);

  int * vertex_8 = {0,1,2,3,5};
  
  for (int i = 0 ; i<5 ; i++){
    gsl_spmatrix_uint_set(matrix,7,vertex_8[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,7,4,NORTH);
  gsl_spmatrix_uint_set(matrix,7,6,WEST);
  gsl_spmatrix_uint_set(matrix,7,8,EAST);

  int * vertex_9 = {0,1,2,3,4};
  
  for (int i = 0 ; i<5 ; i++){
    gsl_spmatrix_uint_set(matrix,8,vertex_9[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,8,5,NORTH);
  gsl_spmatrix_uint_set(matrix,8,7,WEST);

  gsl_spmatrix_uint matrix_test = square_graph(m);

  assert(gsl_spmatrix_uint_equal(matrix,matrix_test)==1);

  gsl_spmatrix_uint_free(matrix);
  gsl_spmatrix_uint_free(matrix_test);

  return 1;
}

int test__is_in_hole()
{
  size_t x_1 = 5;
  size_t y_1 = 3;
  size_t x_begin_1 = 0;
  size_t y_begin_1 = 2;
  size_t size_x_1 = 6;
  size_t size_y_1 = 3;

  assert(is_in_hole(x_1,y_1,begin_x_1,begin_y_1,size_x_1,size_y_1));

  size_t x_2 = 5;
  size_t y_2 = 3;
  size_t x_begin_2 = 0;
  size_t y_begin_2 = 2;
  size_t size_x_2 = 2;
  size_t size_y_2 = 3;

  assert(! is_in_hole(x_2,y_2,begin_x_2,begin_y_2,size_x_2,size_y_2));

  size_t x_2 = 4;
  size_t y_2 = 7;
  size_t x_begin_2 = 0;
  size_t y_begin_2 = 2;
  size_t size_x_2 = 6;
  size_t size_y_2 = 1;

  assert(! is_in_hole(x_2,y_2,begin_x_2,begin_y_2,size_x_2,size_y_2));

  return 1; 
}

int test__toric()
{

  size_t x_1 = 1;
  size_t y_1 = 1;
  size_t m_1 = 3;
  assert (toric(x_1,y_1,m_1)!=0);

  size_t x_2 = 2;
  size_t y_2 = 2;
  size_t m_2 = 12;
  assert (toric(x_2,y_2,m_2)==0);

  size_t x_3 = 6;
  size_t y_3 = 6;
  size_t m_3 = 12;
  assert (toric(x_3,y_3,m_3)==0);

  return 1;
}


/*
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    TESTCASE("Test of get_square_direction", test__get_direction_square);
    return 0;
}
*/
