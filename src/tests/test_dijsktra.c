#include "test_dijsktra.h"
#include "test.h"

int test__init_array_with_value()
{
    size_t size = 10;
    size_t array[size];
    size_t value = 10;
    init_array_with_value(value, array, size);
    for (size_t i = 0; i < size; i++)
        ASSERT_EQUAL(value, array[i]);
    
    value = 15;
    init_array_with_value(value, array, size);
    for (size_t i = 0; i < size; i++)
        ASSERT_EQUAL(value, array[i]);
    
    value = 150;
    init_array_with_value(value, array, size/2);
    for (size_t i = 0; i < size/2; i++)
        ASSERT_EQUAL(value, array[i]);

    return 1;
}

int test__init_array_with_index()
{
    size_t size_1 = 1;
    size_t array_1[size_1];
    init_array_with_index(array_1, size_1);
    for (size_t i = 0; i < size_1; i++)
        ASSERT_EQUAL(i, array_1[i]);

    size_t size_2 = 10;
    size_t array_2[size_2];
    init_array_with_index(array_2, size_2);
    for (size_t i = 0; i < size_2; i++)
        ASSERT_EQUAL(i, array_2[i]);
    return 1;
}

int test__look_for_jump()
{
    struct graph_t *graph = get_graph('c', 3);
    struct near_neighbours *neighbours = get_correlated_graph(graph);
    // 0, 1, 2
    // 3, 4, 5
    // 6, 7, 8
    size_t size = 9;
    size_t distance[] = {9, 9, 9, 9, 0, 9, 9, 9, 9};
    size_t distance_copy[] = {9, 9, 9, 9, 0, 9, 9, 9, 9};
    size_t predecessor[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    size_t predecessor_copy[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    
    size_t player = 4;
    size_t other_player = 0;
    // Players not side by side so no jump possible,
    // ASSERT that look for jump return 0 and do not modify distance and predecessor array
    ASSERT_EQUAL(0, look_for_jump(graph, neighbours, distance, predecessor, player, other_player));
    ASSERT_ARRAY_EQUAL(distance_copy, distance, size);
    ASSERT_ARRAY_EQUAL(predecessor_copy, predecessor, size);

    // Players have a side in contact, player have to move in diag
    player = 4;
    other_player = 1;
    distance_copy[0] = 1;
    distance_copy[2] = 1;
    predecessor_copy[0] = player;
    predecessor_copy[2] = player;
    // ASSERT return 1 and distance and predecessor are well modified
    ASSERT_EQUAL(1, look_for_jump(graph, neighbours, distance, predecessor, player, other_player));
    ASSERT_ARRAY_EQUAL(distance_copy, distance, size);
    ASSERT_ARRAY_EQUAL(predecessor_copy, predecessor, size);

    // Reset of the distance and predecessor array
    distance[0] = 9;
    distance[2] = 9;
    predecessor[0] = 0;
    predecessor[2] = 0;

    distance_copy[0] = 9;
    distance_copy[2] = 9;
    predecessor_copy[0] = 0;
    predecessor_copy[2] = 0;

    // Players have a side in contact, player have to move behind the other player
    player = 7;
    other_player = 4;
    distance_copy[1] = 1;
    predecessor_copy[1] = player;
    // ASSERT return 1 and distance and predecessor are well modified
    ASSERT_EQUAL(1, look_for_jump(graph, neighbours, distance, predecessor, player, other_player));
    ASSERT_ARRAY_EQUAL(distance_copy, distance, size);
    ASSERT_ARRAY_EQUAL(predecessor_copy, predecessor, size);

    graph_free(graph);
    free_correlation_graph(neighbours);
    return 1;
}

int test__is_visited()
{
    size_t size = 5;
    size_t array_1[] = { 0, 0, 0, 0, 0};
    for (size_t i = 0; i < size; i++)
        ASSERT_FALSE(is_visited(array_1, i));
    
    size_t array_2[] = { 1, 1, 1, 1, 1};
    for (size_t i = 0; i < size; i++)
        ASSERT_TRUE(is_visited(array_2, i));
    
    size_t array_3[] = { 0, 1, 0, 0, 1};
    ASSERT_FALSE(is_visited(array_3, 0));
    ASSERT_TRUE(is_visited(array_3, 1));
    ASSERT_FALSE(is_visited(array_3, 2));
    ASSERT_FALSE(is_visited(array_3, 3));
    ASSERT_TRUE(is_visited(array_3, 4));
    return 1;
}

int test__is_all_visited()
{
    size_t size = 10;
    size_t already_visited = 0;

    ASSERT_FALSE(is_all_visited(already_visited, size));
    already_visited = 2;
    ASSERT_FALSE(is_all_visited(already_visited, size));
    already_visited = 4;
    ASSERT_FALSE(is_all_visited(already_visited, size));
    already_visited = 6;
    ASSERT_FALSE(is_all_visited(already_visited, size));
    already_visited = 8;
    ASSERT_FALSE(is_all_visited(already_visited, size));
    already_visited = 10;
    ASSERT_TRUE(is_all_visited(already_visited, size));
    already_visited = 12;
    ASSERT_TRUE(is_all_visited(already_visited, size));
    return 1;
}

int test__get_dist_min_not_visited()
{
    size_t size = 5;
    size_t is_visited[] = {0, 0, 0, 0, 0};
    size_t distance[] = {0, 4, 3, 5, 2};
    ASSERT_EQUAL(0, get_dist_min_not_visited(distance, is_visited, size));
    is_visited[0] = 1;
    ASSERT_EQUAL(4, get_dist_min_not_visited(distance, is_visited, size));
    is_visited[4] = 1;
    ASSERT_EQUAL(2, get_dist_min_not_visited(distance, is_visited, size));
    is_visited[2] = 1;
    ASSERT_EQUAL(1, get_dist_min_not_visited(distance, is_visited, size));
    is_visited[1] = 1;
    ASSERT_EQUAL(3, get_dist_min_not_visited(distance, is_visited, size));
    is_visited[3] = 1;
    ASSERT_EQUAL(0, get_dist_min_not_visited(distance, is_visited, size));
    return 1;
}

int test__get_nearest_destination()
{
    size_t destination_size = 3;
    size_t destination[] = {0, 2, 3};
    
    size_t distance_1[] = {0, 4, 3, 5, 2};
    ASSERT_EQUAL(0, get_nearest_destination(distance_1, destination, destination_size));
    
    size_t distance_2[] = {4, 4, 4, 4, 4};
    size_t nearest = get_nearest_destination(distance_2, destination, destination_size);
    size_t is_in_destination = 0;
    for (size_t i = 0; i < destination_size; i++)
        is_in_destination = (is_in_destination || destination[i] == nearest);
    ASSERT_TRUE(is_in_destination);
    
    size_t distance_3[] = {5, 4, 3, 2, 1};
    ASSERT_EQUAL(3, get_nearest_destination(distance_3, destination, destination_size));
    return 1;
}

int test__get_first_step_from_shortest_path()
{
    size_t source = 1;
    size_t destination = 3;

    size_t predecessor_1[] = {2, 0, 3, 4, 1};
    ASSERT_EQUAL(4, get_first_step_from_shortest_path(predecessor_1, source, destination));
    
    size_t predecessor_2[] = {4, 1, 0, 6, 1, 1, 2, 1};
    ASSERT_EQUAL(4, get_first_step_from_shortest_path(predecessor_2, source, destination));
    
    size_t predecessor_3[] = {5, 1, 1, 1, 1};
    ASSERT_EQUAL(3, get_first_step_from_shortest_path(predecessor_3, source, destination));

    return 1;
}

int test__dijsktra()
{
    struct graph_t *graph = get_graph('t', 6);
    struct near_neighbours *neighbours = get_correlated_graph(graph);
    // 0  1  2  3  4  5
    // 6  7  8  9  10 11
    // 12 13       16 17
    // 18 19       22 23
    // 24 25 26 27 28 29
    // 30 31 32 33 34 35
    size_t destination_size = 6;
    size_t destination[] = {0, 1, 2, 3, 4, 5};
    size_t destination_copy[] = {0, 1, 2, 3, 4, 5};
    size_t player = 26;
    size_t other_player = 0;
    
    size_t move_to_vertex = dijsktra(graph, neighbours, destination, destination_size, player, other_player);
    ASSERT_EQUAL(25, move_to_vertex);
    ASSERT_ARRAY_EQUAL(destination_copy, destination, destination_size);
    
    other_player = 1;
    move_to_vertex = dijsktra(graph, neighbours, destination, destination_size, player, other_player);
    ASSERT_TRUE((move_to_vertex == 25 || move_to_vertex == 27));
    ASSERT_ARRAY_EQUAL(destination_copy, destination, destination_size);
    
    player = 13;
    other_player = 7;
    move_to_vertex = dijsktra(graph, neighbours, destination, destination_size, player, other_player);
    ASSERT_EQUAL(1, move_to_vertex);
    ASSERT_ARRAY_EQUAL(destination_copy, destination, destination_size);
    
    player = 32;
    other_player = 26;
    move_to_vertex = dijsktra(graph, neighbours, destination, destination_size, player, other_player);
    ASSERT_EQUAL(25, move_to_vertex);
    ASSERT_ARRAY_EQUAL(destination_copy, destination, destination_size);
    
    graph_free(graph);
    free_correlation_graph(neighbours);
    return 1;
}

void tests__dijsktra_functions()
{
    TEST_FUNCTION(test__init_array_with_value);
    TEST_FUNCTION(test__init_array_with_index);
    TEST_FUNCTION(test__look_for_jump);
    TEST_FUNCTION(test__is_visited);
    TEST_FUNCTION(test__is_all_visited);
    TEST_FUNCTION(test__get_dist_min_not_visited);
    TEST_FUNCTION(test__get_nearest_destination);
    TEST_FUNCTION(test__get_first_step_from_shortest_path);
    TEST_FUNCTION(test__dijsktra);
}