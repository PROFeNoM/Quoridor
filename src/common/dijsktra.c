#include "utils.h"
#include "dijsktra.h"
#include <string.h>
#include  <math.h>

/**
 * Fill `size` cells of `array` with `value`
 * @param value Value used to filled the array
 * @param array Array to fill
 * @param size Size of the array
 */
void init_array_with_value(size_t value, size_t array[], size_t size)
{
    for (size_t i = 0; i < size; i++)
        array[i] = value;
}

/**
 * Fill `array` with one's cell's index value
 * @param array Array to fill
 * @param size Size of the array
 */
void init_array_with_index(size_t array[], size_t size)
{
    for (size_t i = 0; i < size; i++)
        array[i] = i;
}

/**
 * Look if the player is adjacent of the other player
 * If it is, look for possible particular position case
 * and set distances and predecessors to the right value
 * @param graph Graph representing the board
 * @param distance Array indicating distances from player position
 * @param predecessor Array indicating the predecessor of a vertex when using Dijkstra
 * @param player Position of the player
 * @param other_player Position of the other player
 * @return 1 if the player is adjacent to the other player, 0 else
 */
size_t look_for_jump(struct graph_t *graph, size_t distance[], size_t predecessor[], size_t player, size_t other_player)
{
    if (!is_connected(graph, player, other_player))
        return 0;

    size_t m = sqrt(graph->num_vertices);
    size_t possibilities[8] = {
        player + 2,     // jump to the right
        player - 2,     // jump to the left
        player + 2 * m, // jump below
        player - 2 * m, // jump up
        player + 1 + m, // jump right diagonal below
        player + 1 - m, // jump right diagonal up
        player - 1 + m, // jump left diagonal below
        player - 1 - m  // jump left diagonal up
    };

    for (int i = 0; i < 8; i++)
    {
        if (can_player_move_to(graph, possibilities[i], 0, player, other_player))
        {
            //printf("possibility : %ld\n", possibilities[i]);
            distance[possibilities[i]] = 1;
            predecessor[possibilities[i]] = player;
        }
    }

    return 1;
}

/**
 * Predicate determining if a vertex has already been visited
 * @param visited Array of boolean: 1 if true, 0 else
 * @param index Index of the vertex to consider
 * @return non-zero if it has already been visited, 0 else
 */
int is_visited(size_t visited[], size_t index)
{
    return visited[index] != 0;
}

/**
 * Predicate determining if all vertex has been visited once
 * @param already number of vertex already visited
 * @param size Size of the array
 * @return 1 if true, 0 else
 */
int is_all_visited(size_t already, size_t size)
{
    return already >= size;
}

/**
 * Return the cell not visited with the minimum distance
 * @param distance Array representing the distance from a source
 * @param visited Array of boolean: 1 if it already has been visited, 0 else
 * @param size Size of distance and visited array
 */
size_t get_dist_min_not_visited(size_t distance[], size_t visited[], size_t size)
{
    size_t distance_min = size * size;
    size_t index_min = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (distance[i] < distance_min && !is_visited(visited, i))
        {
            distance_min = distance[i];
            index_min = i;
        }
    }
    return index_min;
}

/**
 * Return the nearest possible destination vertex
 * @param distance Array containing the distance from the player position related to the vertices i
 * @param dst Array of possible destination
 * @param dst_size Size of dst
 */
size_t get_nearest_destination(size_t distance[], size_t dst[], size_t dst_size)
{
    size_t nearest_destination = dst[0];
    size_t longer = distance[0];
    for (size_t i = 1; i < dst_size; i++)
    {
        if (distance[dst[i]] < longer)
        {
            longer = distance[dst[i]];
            nearest_destination = dst[i];
        }
    }
    return nearest_destination;
}

/**
 * Return the vertex to move to in order to be on the shortest path from src to dst
 * @param predecessor Array of predecessors (predecessor of the ith vertex on the path is predecessor[i])
 * @param src Vertex indicating the source
 * @param dst Vertex representing the destination to ultimately attain
 */
size_t get_first_step_from_shortest_path(size_t predecessor[], size_t src, size_t dst)
{
    size_t origin = dst;
    size_t previous = dst;

    while (origin != src)
    {
        previous = origin;
        origin = predecessor[origin];
    }
    return previous;
}

size_t dijsktra(struct graph_t *graph, size_t dst[], size_t dst_size, size_t player, size_t other_player)
{
    size_t num = graph->num_vertices;
    size_t visited[num];
    size_t distance[num];
    size_t predecessor[num];

    init_array_with_value(0, visited, num);
    init_array_with_value(num + 1, distance, num);
    init_array_with_index(predecessor, num);

    // Player position is the source,so it has 0 distance
    distance[player] = 0;
    // If there is a jump possible, then other player position is not attainable
    visited[other_player] = look_for_jump(graph, distance, predecessor, player, other_player);

    size_t index = 0;
    size_t number_visited = visited[other_player];
    while (!is_all_visited(number_visited, num))
    {
        index = get_dist_min_not_visited(distance, visited, num);
        //printf("Before\nIndex : %ld, distance %ld\nChangement ?\n", index, distance[index]);
        visited[index] = 1;
        number_visited++;
        for (size_t i = 0; i < num; i++)
        {
            if (is_connected(graph, index, i))
            {
                if (distance[index] + 1 < distance[i])
                {
                    //printf(" -> Index : %ld, i : %ld, distance from %ld to %ld\n", index, i, distance[i], length);
                    distance[i] = distance[index] + 1;
                    predecessor[i] = index;
                }
            }
        }
        //printf("\n");
    }

    return get_first_step_from_shortest_path(predecessor, player, get_nearest_destination(distance, dst, dst_size));
}
