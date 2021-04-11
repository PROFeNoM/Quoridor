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
/*
int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    TESTCASE("Test of get_square_direction", test__get_direction_square);
    return 0;
}
*/
