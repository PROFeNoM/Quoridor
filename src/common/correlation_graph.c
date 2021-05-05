#include "correlation_graph.h"

/**
 * Set the neighbour's index of the vertex from in a direction
 * @param from Vertex to be completed
 * @param to Index of the neighbour
 * @param direction Direction of the neighbour
 * @param neighbour_graph Array of the vertices' neighbours
 */
void set_neighbour(size_t from, size_t to, enum direction_t direction, struct near_neighbours neighbours_graph[])
{
  if (direction == NORTH)
    neighbours_graph[from].north = to;
  else if (direction == SOUTH)
    neighbours_graph[from].south = to;
  else if (direction == WEST)
    neighbours_graph[from].west = to;
  else if (direction == EAST)
    neighbours_graph[from].east = to;
}

/**
 * Initialize the neighbours of the vertex index to a 'no neighbour' value
 * @param index Vertex to be initialized
 * @param neighbour_graph Neighbours' array
 * @param no_neighbour Initializing value
 */
void initialize_neighbours(size_t index, struct near_neighbours neighbours_graph[], size_t no_neighbour)
{
  neighbours_graph[index].north = no_neighbour;
  neighbours_graph[index].south = no_neighbour;
  neighbours_graph[index].west = no_neighbour;
  neighbours_graph[index].east = no_neighbour;
}

struct near_neighbours *get_correlated_graph(struct graph_t *graph)
{
  size_t size = graph->t->size1*graph->t->size2;
  struct near_neighbours *neighbours_graph = malloc((size+1) * sizeof(struct near_neighbours));
  for (size_t i = 0; i < graph->t->size1; i++) {
    initialize_neighbours(i, neighbours_graph, size);
    
    for (size_t j = 0; j < graph->t->size2; j++) {  
      enum direction_t direction = get_connection_type(graph, i, j);
      
      set_neighbour(i, j, direction, neighbours_graph);
    }
  }
	initialize_neighbours(size, neighbours_graph, size);
  return neighbours_graph;
}

void free_correlation_graph(struct near_neighbours *neighbours_graph)
{
  free(neighbours_graph);
}
