#include "test.h"
#include "graph_pattern.h"
#include "test_graph_pattern.h"

int test__c_constraint()
{
  ASSERT_TRUE(c_constraint(-2)==0);
  ASSERT_TRUE(c_constraint(0)==0);
  ASSERT_TRUE(c_constraint(2)!=0);
  ASSERT_TRUE(c_constraint(22)!=0);

  return 1;
}

int test__t_constraint()
{
  ASSERT_TRUE(t_constraint(-2)==0);
  ASSERT_TRUE(t_constraint(0)==0);
  ASSERT_TRUE(t_constraint(2)==0);
  ASSERT_TRUE(t_constraint(33)!=0);

  return 1;
}

int test__h_constraint()
{
  ASSERT_TRUE(h_constraint(-2)==0);
  ASSERT_TRUE(h_constraint(0)==0);
  ASSERT_TRUE(h_constraint(2)==0);
  ASSERT_TRUE(h_constraint(33)!=0);

  return 1;
}

int test__s_constraint()
{
  ASSERT_TRUE(s_constraint(-2)==0);
  ASSERT_TRUE(s_constraint(0)==0);
  ASSERT_TRUE(s_constraint(2)==0);
  ASSERT_TRUE(s_constraint(5)!=0);
  ASSERT_TRUE(s_constraint(36)==0);

  return 1;
}

int test__abide_graph_constraint()
{
  ASSERT_TRUE(abide_graph_constraint('c', 3));
  ASSERT_TRUE(abide_graph_constraint('c', 5));
  ASSERT_FALSE(abide_graph_constraint('c', 0));
  ASSERT_FALSE(abide_graph_constraint('c', -3));
  
  ASSERT_TRUE(abide_graph_constraint('t', 3));
  ASSERT_TRUE(abide_graph_constraint('t', 6));
  ASSERT_TRUE(abide_graph_constraint('t', 15));
  ASSERT_FALSE(abide_graph_constraint('t', 0));
  ASSERT_FALSE(abide_graph_constraint('t', -1));
  ASSERT_FALSE(abide_graph_constraint('t', -3));
  ASSERT_FALSE(abide_graph_constraint('t', -6));

  ASSERT_TRUE(abide_graph_constraint('h', 3));
  ASSERT_TRUE(abide_graph_constraint('h', 6));
  ASSERT_TRUE(abide_graph_constraint('h', 15));
  ASSERT_FALSE(abide_graph_constraint('h', 0));
  ASSERT_FALSE(abide_graph_constraint('h', -1));
  ASSERT_FALSE(abide_graph_constraint('h', -3));
  ASSERT_FALSE(abide_graph_constraint('h', -6));

  ASSERT_TRUE(abide_graph_constraint('s', 5));
  ASSERT_TRUE(abide_graph_constraint('s', 10));
  ASSERT_TRUE(abide_graph_constraint('s', 15));
  ASSERT_FALSE(abide_graph_constraint('s', 0));
  ASSERT_FALSE(abide_graph_constraint('s', -1));
  ASSERT_FALSE(abide_graph_constraint('s', -5));
  ASSERT_FALSE(abide_graph_constraint('s', -15));

  ASSERT_FALSE(abide_graph_constraint('m', 15));
  ASSERT_FALSE(abide_graph_constraint('m', 3));
  ASSERT_FALSE(abide_graph_constraint('r', 15));
  ASSERT_FALSE(abide_graph_constraint('r', 3));
  ASSERT_FALSE(abide_graph_constraint('p', 15));
  ASSERT_FALSE(abide_graph_constraint('p', 3));

  return 1;
}

int test__get_direction_square()
{

  size_t i_1 = 0;
  size_t j_1 = 0;
  size_t m_1 = 5;
  
  ASSERT_TRUE(get_direction_square(i_1,j_1,m_1)==NO_DIRECTION);

  size_t i_2 = 0;
  size_t j_2 = 5;
  size_t m_2 = 5;

  ASSERT_TRUE(get_direction_square(i_2,j_2,m_2)==SOUTH);

  size_t i_3 = 4;
  size_t j_3 = 3;
  size_t m_3 = 5;

  ASSERT_TRUE(get_direction_square(i_3,j_3,m_3)==WEST);

  size_t i_4 = 3;
  size_t j_4 = 4;
  size_t m_4 = 5;

  ASSERT_TRUE(get_direction_square(i_4,j_4,m_4)==EAST);

  size_t i_5 = 7;
  size_t j_5 = 2;
  size_t m_5 = 5;

  ASSERT_TRUE(get_direction_square(i_5,j_5,m_5)==NORTH);

  size_t i_6 = 7;
  size_t j_6 = 2;
  size_t m_6 = 6;

  ASSERT_TRUE(get_direction_square(i_6,j_6,m_6)==NO_DIRECTION);

  size_t i_7 = 5;
  size_t j_7 = 6;
  size_t m_7 = 6;

  ASSERT_TRUE(get_direction_square(i_7,j_7,m_7)==NO_DIRECTION);

  return 1;
}

int test__square(){

  size_t x_1 = 5;
  size_t y_1 = 18;
  size_t m_1 = 5;

  ASSERT_TRUE(square(x_1, y_1, m_1));

  size_t x_2 = 22;
  size_t y_2 = 9;
  size_t m_2 = 18;

  ASSERT_TRUE(square(x_2,y_2,m_2));

  return 1;
}

int test__square_graph()
{
  size_t m = 3;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);

  int vertex_1[] = {0,2,4,5,6,7,8};

  for (int i = 0 ; i<7 ; i++){
    gsl_spmatrix_uint_set(matrix,0,vertex_1[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,0,1,EAST);
  gsl_spmatrix_uint_set(matrix,0,3,SOUTH);

  int vertex_2[] = {1,3,5,6,7,8};

  for (int i = 0 ; i<6 ; i++){
    gsl_spmatrix_uint_set(matrix,1,vertex_2[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,1,0,WEST);
  gsl_spmatrix_uint_set(matrix,1,2,EAST);
  gsl_spmatrix_uint_set(matrix,1,4,SOUTH);

  int vertex_3[] = {0,2,3,4,6,7,8};

  for (int i = 0 ; i<7 ; i++){
    gsl_spmatrix_uint_set(matrix,2,vertex_3[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,2,1,WEST);
  gsl_spmatrix_uint_set(matrix,2,5,SOUTH);

  int vertex_4[] = {1,2,3,5,7,8};

  for (int i = 0 ; i<6 ; i++){
    gsl_spmatrix_uint_set(matrix,3,vertex_4[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,3,0,NORTH);
  gsl_spmatrix_uint_set(matrix,3,6,SOUTH);
  gsl_spmatrix_uint_set(matrix,3,4,EAST);

  int vertex_5[] = {0,2,4,6,8};
  
  for (int i = 0 ; i<5 ; i++){
    gsl_spmatrix_uint_set(matrix,4,vertex_5[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,4,1,NORTH);
  gsl_spmatrix_uint_set(matrix,4,7,SOUTH);
  gsl_spmatrix_uint_set(matrix,4,5,EAST);
  gsl_spmatrix_uint_set(matrix,4,3,WEST);

  int vertex_6[] = {0,1,3,5,6,7};
  
  for (int i = 0 ; i<6 ; i++){
    gsl_spmatrix_uint_set(matrix,5,vertex_6[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,5,2,NORTH);
  gsl_spmatrix_uint_set(matrix,5,8,SOUTH);
  gsl_spmatrix_uint_set(matrix,5,4,WEST);

  int vertex_7[] = {0,1,2,4,5,6,8};
  
  for (int i = 0 ; i<7 ; i++){
    gsl_spmatrix_uint_set(matrix,6,vertex_7[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,6,3,NORTH);
  gsl_spmatrix_uint_set(matrix,6,7,EAST);

  int vertex_8[] = {0,1,2,3,5,7};
  
  for (int i = 0 ; i<6 ; i++){
    gsl_spmatrix_uint_set(matrix,7,vertex_8[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,7,4,NORTH);
  gsl_spmatrix_uint_set(matrix,7,6,WEST);
  gsl_spmatrix_uint_set(matrix,7,8,EAST);

  int vertex_9[] = {0,1,2,3,4,6,8};
  
  for (int i = 0 ; i<7 ; i++){
    gsl_spmatrix_uint_set(matrix,8,vertex_9[i],NO_DIRECTION);
  }
  gsl_spmatrix_uint_set(matrix,8,5,NORTH);
  gsl_spmatrix_uint_set(matrix,8,7,WEST);

  gsl_spmatrix_uint *matrix_test = square_graph(m);

  ASSERT_TRUE(gsl_spmatrix_uint_equal(matrix,matrix_test));

  gsl_spmatrix_uint_free(matrix);
  gsl_spmatrix_uint_free(matrix_test);

  return 1;
}

int test__is_in_rectangle()
{
  size_t x_1 = 5;
  size_t y_1 = 3;
  size_t x_begin_1 = 0;
  size_t y_begin_1 = 2;
  size_t x_size_1 = 6;
  size_t y_size_1 = 3;

  ASSERT_TRUE(is_in_rectangle(x_1,y_1,x_begin_1,y_begin_1,x_size_1,y_size_1));

  size_t x_2 = 5;
  size_t y_2 = 3;
  size_t x_begin_2 = 0;
  size_t y_begin_2 = 2;
  size_t x_size_2 = 2;
  size_t y_size_2 = 3;

  ASSERT_TRUE(! is_in_rectangle(x_2,y_2,x_begin_2,y_begin_2,x_size_2,y_size_2));

  size_t x_3 = 4;
  size_t y_3 = 7;
  size_t x_begin_3 = 0;
  size_t y_begin_3 = 2;
  size_t x_size_3 = 6;
  size_t y_size_3 = 1;

  ASSERT_TRUE(! is_in_rectangle(x_3,y_3,x_begin_3,y_begin_3,x_size_3,y_size_3));

  return 1; 
}

int test__toric()
{

  size_t x_1 = 1;
  size_t y_1 = 1;
  size_t m_1 = 3;
  ASSERT_FALSE(toric(x_1,y_1,m_1));

  size_t x_2 = 2;
  size_t y_2 = 2;
  size_t m_2 = 12;
  ASSERT_TRUE(toric(x_2,y_2,m_2));

  size_t x_3 = 6;
  size_t y_3 = 6;
  size_t m_3 = 12;
  ASSERT_FALSE(toric(x_3,y_3,m_3));

  return 1;
}

int test__t_graph()
{
  size_t m = 3;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);

  int line_1[] = {NO_DIRECTION, EAST, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_2[] = {WEST, NO_DIRECTION, EAST, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_3[] = {NO_DIRECTION, WEST, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_4[] = {NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION};
  int line_5[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_6[] = {NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH};
  int line_7[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, EAST, NO_DIRECTION};
  int line_8[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, WEST, NO_DIRECTION, EAST};
  int line_9[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, WEST, NO_DIRECTION};

  for (int i = 0 ; i<9 ; i++){
    gsl_spmatrix_uint_set(matrix,0,i, line_1[i]);
    gsl_spmatrix_uint_set(matrix,1,i, line_2[i]);
    gsl_spmatrix_uint_set(matrix,2,i, line_3[i]);
    gsl_spmatrix_uint_set(matrix,3,i, line_4[i]);
    gsl_spmatrix_uint_set(matrix,4,i, line_5[i]);
    gsl_spmatrix_uint_set(matrix,5,i, line_6[i]);
    gsl_spmatrix_uint_set(matrix,6,i, line_7[i]);
    gsl_spmatrix_uint_set(matrix,7,i, line_8[i]);
    gsl_spmatrix_uint_set(matrix,8,i, line_9[i]);
  }

  gsl_spmatrix_uint *matrix_test = t_graph(m); 

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      ASSERT_EQUAL(gsl_spmatrix_uint_get(matrix, i, j), gsl_spmatrix_uint_get(matrix_test, i, j));
    }
  }

  gsl_spmatrix_uint_free(matrix);
  gsl_spmatrix_uint_free(matrix_test);

  return 1;
}

int test__h()
{
  size_t x_1 = 1;
  size_t y_1 = 1;
  size_t m_1 = 3;
  ASSERT_TRUE (h(x_1,y_1,m_1));

  size_t x_2 = 0;
  size_t y_2 = 1;
  size_t m_2 = 3;
  ASSERT_TRUE (h(x_2,y_2,m_2));

  size_t x_3 = 11;
  size_t y_3 = 6;
  size_t m_3 = 12;
  ASSERT_TRUE (h(x_3,y_3,m_3));

  return 1;
}

int test__h_graph()
{
  size_t m = 3;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);

  int line_1[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_2[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_3[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_4[] = {NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, EAST, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION};
  int line_5[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, WEST, NO_DIRECTION, EAST, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_6[] = {NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, WEST, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH};
  int line_7[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_8[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_9[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};

  for (int i = 0; i < 9; i++)
  {
    gsl_spmatrix_uint_set(matrix, 0, i, line_1[i]);
    gsl_spmatrix_uint_set(matrix, 1, i, line_2[i]);
    gsl_spmatrix_uint_set(matrix, 2, i, line_3[i]);
    gsl_spmatrix_uint_set(matrix, 3, i, line_4[i]);
    gsl_spmatrix_uint_set(matrix, 4, i, line_5[i]);
    gsl_spmatrix_uint_set(matrix, 5, i, line_6[i]);
    gsl_spmatrix_uint_set(matrix, 6, i, line_7[i]);
    gsl_spmatrix_uint_set(matrix, 7, i, line_8[i]);
    gsl_spmatrix_uint_set(matrix, 8, i, line_9[i]);
  }

  gsl_spmatrix_uint *matrix_test = h_graph(m);

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      ASSERT_EQUAL(gsl_spmatrix_uint_get(matrix, i, j), gsl_spmatrix_uint_get(matrix_test, i, j));
    }
  }

  gsl_spmatrix_uint_free(matrix);
  gsl_spmatrix_uint_free(matrix_test);

  return 1;
}

int test__snake()
{
  size_t x_1 = 2;
  size_t y_1 = 0;
  size_t m_1 = 15;
  ASSERT_TRUE (snake(x_1,y_1,m_1));

  size_t x_2 = 7;
  size_t y_2 = 7;
  size_t m_2 = 15;
  ASSERT_TRUE (snake(x_2,y_2,m_2)==0);

  size_t x_3 = 7;
  size_t y_3 = 14;
  size_t m_3 = 15;

  ASSERT_TRUE (snake(x_3,y_3,m_3)==0);

  return 1;
}

int test__snake_graph()
{
  size_t m = 3;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);

  int line_1[] = {NO_DIRECTION, EAST, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_2[] = {WEST, NO_DIRECTION, EAST, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_3[] = {NO_DIRECTION, WEST, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION};
  int line_4[] = {NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, EAST, NO_DIRECTION, SOUTH, NO_DIRECTION, NO_DIRECTION};
  int line_5[] = {NO_DIRECTION, NORTH, NO_DIRECTION, WEST, NO_DIRECTION, EAST, NO_DIRECTION, SOUTH, NO_DIRECTION};
  int line_6[] = {NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, WEST, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, SOUTH};
  int line_7[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, EAST, NO_DIRECTION};
  int line_8[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, WEST, NO_DIRECTION, EAST};
  int line_9[] = {NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NO_DIRECTION, NORTH, NO_DIRECTION, WEST, NO_DIRECTION};

  for (int i = 0; i < 9; i++)
  {
    gsl_spmatrix_uint_set(matrix, 0, i, line_1[i]);
    gsl_spmatrix_uint_set(matrix, 1, i, line_2[i]);
    gsl_spmatrix_uint_set(matrix, 2, i, line_3[i]);
    gsl_spmatrix_uint_set(matrix, 3, i, line_4[i]);
    gsl_spmatrix_uint_set(matrix, 4, i, line_5[i]);
    gsl_spmatrix_uint_set(matrix, 5, i, line_6[i]);
    gsl_spmatrix_uint_set(matrix, 6, i, line_7[i]);
    gsl_spmatrix_uint_set(matrix, 7, i, line_8[i]);
    gsl_spmatrix_uint_set(matrix, 8, i, line_9[i]);
  }

  gsl_spmatrix_uint *matrix_test = s_graph(m);

  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      ASSERT_EQUAL(gsl_spmatrix_uint_get(matrix, i, j), gsl_spmatrix_uint_get(matrix_test, i, j));
    }
  }

  gsl_spmatrix_uint_free(matrix);
  gsl_spmatrix_uint_free(matrix_test);

  return 1;
}

int test__matrix_position()
{

  size_t m_1 = 3;
  gsl_spmatrix_uint *matrix_1 = gsl_spmatrix_uint_alloc(2,9);
  for (size_t i = 0 ; i< m_1 ; i++){
    gsl_spmatrix_uint_set(matrix_1,0,i,1);
    gsl_spmatrix_uint_set(matrix_1,1,8-i,1);
  }

  gsl_spmatrix_uint *matrix_test_1 = matrix_position(m_1);
  ASSERT_TRUE(gsl_spmatrix_uint_equal(matrix_1,matrix_test_1) == 1);

  size_t m_2 = 9;
  gsl_spmatrix_uint *matrix_2 = gsl_spmatrix_uint_alloc(2,81);
  for (size_t i = 0 ; i< m_2 ; i++){
    gsl_spmatrix_uint_set(matrix_2,0,i,1);
    gsl_spmatrix_uint_set(matrix_2,1,80-i,1);
  }

  gsl_spmatrix_uint *matrix_test_2 = matrix_position(m_2);
  ASSERT_TRUE(gsl_spmatrix_uint_equal(matrix_2,matrix_test_2) == 1);
  gsl_spmatrix_uint_free(matrix_test_1);
  gsl_spmatrix_uint_free(matrix_1);
  gsl_spmatrix_uint_free(matrix_test_2);
  gsl_spmatrix_uint_free(matrix_2);
  return 1;
}

int test__h_matrix_position()
{

  size_t m_1 = 3;
  gsl_spmatrix_uint *matrix_1 = gsl_spmatrix_uint_alloc(2,9);
  for (size_t i = 0 ; i< m_1 ; i++){
    if (i < 3 / 3 || i >= 2 * 3 / 3){
      gsl_spmatrix_uint_set(matrix_1,0,i,1);
      gsl_spmatrix_uint_set(matrix_1,1,8-i,1);
    }
  }

  gsl_spmatrix_uint *matrix_test_1 = h_matrix_position(m_1);
  ASSERT_TRUE(gsl_spmatrix_uint_equal(matrix_1,matrix_test_1) == 1);
  
  size_t m_2 = 9;
  gsl_spmatrix_uint *matrix_2 = gsl_spmatrix_uint_alloc(2,81);
  for (size_t i = 0 ; i< m_2 ; i++){
    if (i < 9 / 3 || i >= 2 * 9 / 3){
      gsl_spmatrix_uint_set(matrix_2,0,i,1);
      gsl_spmatrix_uint_set(matrix_2,1,80-i,1);
    }
  }

  gsl_spmatrix_uint *matrix_test_2 = h_matrix_position(m_2);
  ASSERT_TRUE(gsl_spmatrix_uint_equal(matrix_2,matrix_test_2) == 1);
  
  gsl_spmatrix_uint_free(matrix_test_1);
  gsl_spmatrix_uint_free(matrix_1);
  gsl_spmatrix_uint_free(matrix_test_2);
  gsl_spmatrix_uint_free(matrix_2);
  return 1;
}
  
void tests__graph_pattern_functions()
{
    TEST_FUNCTION(test__c_constraint);
    TEST_FUNCTION(test__t_constraint);
    TEST_FUNCTION(test__h_constraint);
    TEST_FUNCTION(test__s_constraint);
    TEST_FUNCTION(test__abide_graph_constraint);
    TEST_FUNCTION(test__get_direction_square);
    TEST_FUNCTION(test__square);
    TEST_FUNCTION(test__square_graph);
    TEST_FUNCTION(test__is_in_rectangle);
    TEST_FUNCTION(test__toric);
    TEST_FUNCTION(test__t_graph);
    TEST_FUNCTION(test__h);
    TEST_FUNCTION(test__h_graph);
    TEST_FUNCTION(test__snake);
    TEST_FUNCTION(test__snake_graph);
    TEST_FUNCTION(test__matrix_position);
    TEST_FUNCTION(test__h_matrix_position);
}

