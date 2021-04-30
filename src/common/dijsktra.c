#include "utils.h"
#include "dijsktra.h"
#include <string.h>

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

size_t dijsktra(struct graph_t *graph, size_t src, size_t dst)
{
    size_t num = graph->num_vertices;
    size_t visited[num];
    size_t dist[num];
    init_array_with_value(0, visited, num);
    init_array_with_value(num+1, dist, num);
    dist[src] = 0;

    size_t index = 0;
    size_t length = 0;

    while (!all_visited(visited, num))
    {
        index = get_dist_min_not_visited(dist, visited, num);
        visited[index] = 1;
        for (size_t i = 0; i < num; i++)
        {
            if (is_connected(graph, index, i))
            {
                length = dist[index] + 1;
                if (length < dist[i])
                    dist[i] = length;
            }
        }
    }

    return dist[dst];
}
