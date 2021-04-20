#ifndef TEST_GRAPH_PATTERN_H_
#define TEST_GRAPH_PATTERN_H_

#include "graph_pattern.h"


// get the index of direction of j according to i
int get_direction_square(size_t i,size_t j, size_t m);
gsl_spmatrix_uint *fill_graph(size_t m, int (*is_not_connected)(size_t, size_t, size_t));
int square(size_t x, size_t y, size_t m);
gsl_spmatrix_uint *square_graph(size_t m);
int is_in_rectangle(size_t x, size_t y, size_t begin_x, size_t begin_y, size_t size_x, size_t size_y);
int toric(size_t x, size_t y, size_t m);
gsl_spmatrix_uint *t_graph(size_t m);
int h(size_t x, size_t y, size_t m);
gsl_spmatrix_uint *h_graph(size_t m);
int snake(size_t x, size_t y, size_t m);
gsl_spmatrix_uint * matrix_position(size_t m);
gsl_spmatrix_uint * h_matrix_position(size_t m);

#endif // TEST_GRAPH_PATTERN_H_
