#include "graph_pattern.h"

int c_constraint(int size)
{
  return size > 0;
}

int t_constraint(int size)
{
  return size > 0 && size % 3 == 0;
}

int h_constraint(int size)
{
  return t_constraint(size);
}

int s_constraint(int size)
{
  return size > 0 && size % 5 == 0;
}

int get_direction_square(size_t i, size_t j, size_t m)
{
  if (i == j + 1 && (i / m == j / m))
  {
    return WEST;
  }
  if (i == j - 1 && (i / m == j / m))
  {
    return EAST;
  }
  if (i + m == j)
  {
    return SOUTH;
  }
  if (i - m == j)
  {
    return NORTH;
  }
  return NO_DIRECTION;
}

gsl_spmatrix_uint *fill_graph(size_t m, int (*is_not_connected)(size_t, size_t, size_t))
{
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(m * m, m * m);
  for (size_t i = 0; i < m; i++)
  {
    size_t x = i;
    for (size_t j = 0; j < m; j++)
    {
      if (is_not_connected(j, x, m))
      {
        for (size_t k = 0; k < m * m; k++)
        {
          gsl_spmatrix_uint_set(matrix, x * m + j, k, NOT_CONNECTED);
        }
      }
      else
      {
        for (size_t k = 0; k < m * m; k++)
        {
          if (is_not_connected(k%m, k/m, m))
            gsl_spmatrix_uint_set(matrix, x * m + j, k, NOT_CONNECTED);
          else
            gsl_spmatrix_uint_set(matrix, x * m + j, k, get_direction_square(x * m + j, k, m));
        }
      }
    }
  }
  return matrix;
}

int square(size_t x, size_t y, size_t m)
{
  return m == 0 && x != 0 && y != 0;
}

gsl_spmatrix_uint *square_graph(size_t m)
{
  return fill_graph(m, square);
}

int is_in_hole(size_t x, size_t y, size_t begin_x, size_t begin_y, size_t size_x, size_t size_y)
{
  return (begin_x <= x && x < begin_x + size_x) && (begin_y <= y && y < begin_y + size_y);
}

int toric(size_t x, size_t y, size_t m)
{
  return is_in_hole(x, y, (m / 3), (m / 3), (m / 3), (m / 3));
}

gsl_spmatrix_uint *t_graph(size_t m)
{
  return fill_graph(m, toric);
}

int h(size_t x, size_t y, size_t m)
{
  return is_in_hole(x, y, m / 3, 0 , m/3, m/3) || is_in_hole(x, y, m/3, 2*m/3, m/3, m/3);
}

gsl_spmatrix_uint *h_graph(size_t m)
{
  return fill_graph(m, h);
}

int snake(size_t x, size_t y, size_t m)
{
  return is_in_hole(x, y, m / 5, 0, 4 * m / 5, m / 5) || is_in_hole(x, y, m / 5, 4 * m / 5, 4 * m / 5, m / 5) || is_in_hole(x, y, 0 , 2 * m / 5, 4 * m / 5, m / 5);
}

gsl_spmatrix_uint *s_graph(size_t m)
{
  return fill_graph(m, snake);
}

gsl_spmatrix_uint *matrix_position(size_t m)
{
  size_t num_vertices = m * m;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(2, num_vertices);

  for (size_t i = 0; i < m; i++)
  {
    gsl_spmatrix_uint_set(matrix, 0, i, 1);
    gsl_spmatrix_uint_set(matrix, 1, num_vertices - 1 - i, 1);
  }

  return matrix;
}

gsl_spmatrix_uint *h_matrix_position(size_t m)
{
  size_t num_vertices = m * m;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(2, num_vertices);

  for (size_t i = 0; i < m; i++)
  {
    if (i < m / 3 || i >= 2 * m / 3)
    {
      gsl_spmatrix_uint_set(matrix, 0, i, 1);
      gsl_spmatrix_uint_set(matrix, 1, num_vertices - 1 - i, 1);
    }
  }

  return matrix;
}


gsl_spmatrix_uint *s_matrix_position(size_t m)
{
  size_t num_vertices = m * m;
  gsl_spmatrix_uint *matrix = gsl_spmatrix_uint_alloc(2, num_vertices);

  for (size_t i = 0; i < m / 5; i++)
  {
    if (i < m / 3 || i >= 2 * m / 3)
    {
      gsl_spmatrix_uint_set(matrix, 0, i, 1);
      gsl_spmatrix_uint_set(matrix, 1, num_vertices - 1 - 4 * m / 5 - i, 1);
    }
  }
  return matrix;
}

struct graph_t *get_graph(char type, size_t width)
{
  gsl_spmatrix_uint *matrix = NULL;
  gsl_spmatrix_uint *matrix_pos = NULL;
  if (type == 'c')
  {
    matrix = square_graph(width);
    matrix_pos = matrix_position(width);
  }
  if (type == 't')
  {
    matrix = t_graph(width);
    matrix_pos = matrix_position(width);
  }
  if (type == 'h')
  {
    matrix = h_graph(width);
    matrix_pos = h_matrix_position(width);
  }
  if (type == 's')
  {
    matrix = s_graph(width);
    matrix_pos = s_matrix_position(width);
  }

  struct graph_t *graph = malloc(sizeof(struct graph_t));
  graph->num_vertices = width * width;
  graph->t = matrix;
  graph->o = matrix_pos;
  return graph;
}
