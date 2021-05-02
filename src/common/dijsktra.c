#include "utils.h"
#include "dijsktra.h"
#include <string.h>
#include  <math.h> 

//-------------
//| 2 | 3 | 4 |
//-------------
//| 1 | 2 | 3 |
//-------------
//| . | 1 | 2 |
//-------------

//-------------
//|   | 5 |   |
//-------------
//| 4 | 1 | 6 |
//-------------
//| 2 | . | 3 |
//-------------

//-------------
//| 3 | 6 |   |
//-------------
//| . | 2 | 5 |
//-------------
//| 1 | 4 |   |
//-------------


void init_array_with_value(size_t value, size_t visited[], size_t num)
{
    for (size_t i = 0; i < num; i++)
        visited[i] = value;
}

void init_array_prec(size_t prec[], size_t num)
{
    for (size_t i = 0; i < num; i++)
        prec[i] = i;
}

int all_visited(size_t visited[], size_t size)
{
    for (size_t i = 0; i < size; i++)
        if (!visited[i])
            return 0;
    return 1;
}

int is_visited(size_t visited[], size_t index)
{
    return visited[index] != 0;
}

size_t get_dist_min_not_visited(size_t *dist, size_t *visited, size_t size)
{
    size_t dist_min = size * size;
    size_t index_min = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (dist[i] < dist_min && !is_visited(visited, i))
        {
            dist_min = dist[i];
            index_min = i;
        }
    }
    return index_min;
}



size_t dijsktra(struct graph_t *graph, size_t src, size_t other_player,  size_t dst[],size_t dst_number)
{
    size_t num = graph->num_vertices;
    size_t m = sqrt(num);
    size_t visited[num];
    size_t dist[num];
    size_t prec[num];
    init_array_with_value(0, visited, num);
    init_array_with_value(num+1, dist, num);
    init_array_prec(prec, num);
    dist[src] = 0;

    size_t index = 0;
    size_t length = 0;

    //int index_pos = 0;
    size_t possibilities[12] = {src+1,src-1,src-m,src+m,src+2,src-2,src+2*m,src-2*m,src+1+m,src+1-m,src-1+m,src-1-m};

    for (int i = 0 ; i < 12 ; i++){
      if (can_player_move_to(graph,possibilities[i],0,src,other_player)){
	dist[possibilities[i]] = 1;
	prec[possibilities[i]] = src;
      }
    }

    if (is_connected(graph,src,other_player)){
	visited[other_player]=1;
    }
    while (!all_visited(visited, num))
      {
	index = get_dist_min_not_visited(dist, visited, num);
	visited[index] = 1;
	for (size_t i = 0; i < num; i++)
	  {
	    if (is_connected(graph, index, i))
	      {
		
		length = dist[index] + 1;
		if (length < dist[i]){
		  dist[i] = length;
		  prec[i] = index;
		  
		}
		
	      }
	  }
      }

    size_t vertex;
    size_t longer = num * num;
    for (size_t i = 0 ; i < dst_number ; i++){
      
      if (dist[dst[i]] < longer){
	longer = dist[dst[i]];
	vertex = dst[i];
      }
    }
    size_t origin = vertex ;
    size_t previous;

    while (origin != src){
      
      previous = origin;
      origin = prec[origin];
    }
    
    return previous;
}
