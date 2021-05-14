#include "test_server.h"
#include "test.h"

// Count the number of edge depending on a graph type and width
size_t count_edges(char type, size_t width)
{
    struct graph_t *graph = get_graph(type, width);
    size_t edge_count = 0;
    for (size_t i = 0; i < graph->t->size1; i++)
    {
        for (size_t j = 0; j < graph->t->size2; j++)
        {
            if (is_connected(graph, i, j))
                edge_count++;
        }
    }
    graph_free(graph);
    return edge_count/2;
}

// Compare a graph server with the different expected attributs
size_t compare_graph_server(struct graph_server *graph_1, struct graph_t *graph_2, size_t width, size_t num_wall, char type)
{
    if (graph_1->graph->t->size1 != graph_2->t->size1 || graph_1->graph->t->size2 != graph_2->t->size2 || graph_1->graph->o->size1 != graph_2->o->size1 || graph_1->graph->o->size2 != graph_2->o->size2 || graph_1->graph->num_vertices != graph_2->num_vertices
     || graph_1->num_wall != num_wall || graph_1->type != type || graph_1->width != width)
    {
        graph_free(graph_1->graph);
        graph_free(graph_2);
        return 0;
    }
    for (size_t i = 0; i < graph_1->graph->t->size1; i++)
    {
        for (size_t j = 0; j < graph_1->graph->t->size2; j++)
        {
            if (get_connection_type(graph_1->graph, i, j) != get_connection_type(graph_2, i, j)) {
                graph_free(graph_1->graph);
                graph_free(graph_2);
                return 0;
            }
        }
    }
    graph_free(graph_1->graph);
    graph_free(graph_2);
    return 1;
}

// Test if the number of wall is correct
int test__get_number_of_walls()
{
    size_t c_3 = get_number_of_walls(3, 'c');
    ASSERT_EQUAL(count_edges('c', 3)/15, c_3);
    size_t c_5 = get_number_of_walls(5, 'c');
    ASSERT_EQUAL(count_edges('c', 5) / 15, c_5);
    size_t c_10 = get_number_of_walls(10, 'c');
    ASSERT_EQUAL(count_edges('c', 10) / 15, c_10);

    size_t t_3 = get_number_of_walls(3, 't');
    ASSERT_EQUAL(count_edges('t', 3) / 15, t_3);
    size_t t_6 = get_number_of_walls(6, 't');
    ASSERT_EQUAL(count_edges('t', 6) / 15, t_6);

    size_t h_3 = get_number_of_walls(3, 'h');
    ASSERT_EQUAL(count_edges('h', 3) / 15, h_3);
    size_t h_6 = get_number_of_walls(6, 'h');
    ASSERT_EQUAL(count_edges('h', 6) / 15, h_6);

    size_t s_5 = get_number_of_walls(5, 's');
    ASSERT_EQUAL(count_edges('s', 5) / 15, s_5);
    size_t s_10 = get_number_of_walls(10, 's');
    ASSERT_EQUAL(count_edges('s', 10) / 15, s_10);

    size_t unknown_3 = get_number_of_walls(3, 'u');
    ASSERT_EQUAL(0, unknown_3);
    size_t unknown_8 = get_number_of_walls(8, 'a');
    ASSERT_EQUAL(0, unknown_8);
    return 1;
}

// Test if initialize graph server function works for every type of graph
int test__initialize_graph()
{
    struct graph_server graph_c_6;
    initialize_graph( 6, 'c', &graph_c_6);
    ASSERT_TRUE(compare_graph_server(&graph_c_6, get_graph('c', 6), 6, count_edges('c', 6)/15, 'c'));
    
    struct graph_server graph_c_9;
    initialize_graph( 9, 'c', &graph_c_9);
    ASSERT_TRUE(compare_graph_server(&graph_c_9, get_graph('c', 9), 9, count_edges('c', 9)/15, 'c'));
    
    struct graph_server graph_t_6;
    initialize_graph( 6, 't', &graph_t_6);
    ASSERT_TRUE(compare_graph_server(&graph_t_6, get_graph('t', 6), 6, count_edges('t', 6)/15, 't'));
    
    struct graph_server graph_h_6;
    initialize_graph( 6, 'h', &graph_h_6);
    ASSERT_TRUE(compare_graph_server(&graph_h_6, get_graph('h', 6), 6, count_edges('h', 6)/15, 'h'));
    
    struct graph_server graph_s_5;
    initialize_graph( 5, 's', &graph_s_5);
    ASSERT_TRUE(compare_graph_server(&graph_s_5, get_graph('s', 5), 5, count_edges('s', 5)/15, 's'));
    return 1;
}

// Test if the initial move matches the requirement
int test__get_initial_move()
{
    struct move_t initial_move = get_initial_move();
    ASSERT_EQUAL(BLACK, initial_move.c);
    ASSERT_EQUAL(NO_TYPE, initial_move.t);
    ASSERT_EQUAL(0, initial_move.m);
    return 1;
}

// Test the update function
int test__update()
{
    // Initialize a server (graph, and player num wall and position)
    struct server server;
    server.graph.graph = get_graph('c', 6);
    server.players[BLACK].pos = 1;
    server.players[BLACK].num_wall = 2;
    server.players[WHITE].pos = 1;
    server.players[WHITE].num_wall = 2;

    //////////// Test update for BLACK player ///////////
    struct move_t moveBLACK = {.c=BLACK, .e={no_edge(), no_edge()}, .m=2, .t=MOVE};
    update(&server, moveBLACK);
    // ASSERT move to pos 2 has been applied correctly
    ASSERT_EQUAL(2, server.players[BLACK].pos);
    ASSERT_EQUAL(1, server.players[WHITE].pos);
    ASSERT_EQUAL(2, server.players[BLACK].num_wall);
    ASSERT_EQUAL(2, server.players[WHITE].num_wall);

    moveBLACK = (struct move_t){.c=BLACK, .e={no_edge(), no_edge()}, .m=4, .t=MOVE};
    update(&server, moveBLACK);
    // ASSERT move to pos 4 has been applied correctly
    ASSERT_EQUAL(4, server.players[BLACK].pos);
    ASSERT_EQUAL(1, server.players[WHITE].pos);
    ASSERT_EQUAL(2, server.players[BLACK].num_wall);
    ASSERT_EQUAL(2, server.players[WHITE].num_wall);

    moveBLACK = (struct move_t){.c=BLACK, .e={{1,2}, {6,7}}, .m=0, .t=WALL};
    update(&server, moveBLACK);
    // ASSERT put a wall between 1-2 and 6-7 has been applied correctly
    size_t connection_1_2 = is_connected(server.graph.graph, 1, 2);
    size_t connection_6_7 = is_connected(server.graph.graph, 6, 7);
    ASSERT_EQUAL(4, server.players[BLACK].pos); // has not changed the player position
    ASSERT_EQUAL(1, server.players[WHITE].pos);
    ASSERT_EQUAL(1, server.players[BLACK].num_wall); // number of wall of the player has decrease
    ASSERT_EQUAL(2, server.players[WHITE].num_wall);
    ASSERT_TRUE(!connection_1_2 && !connection_6_7); // there is no more connection between cell 1-2 and 6-7


    //////////// Test update for WHITE player ///////////
    struct move_t moveWHITE = {.c = WHITE, .e = {no_edge(), no_edge()}, .m = 3, .t = MOVE};
    update(&server, moveWHITE);
    // ASSERT move to pos 3 has been applied correctly
    ASSERT_EQUAL(3, server.players[WHITE].pos);
    ASSERT_EQUAL(4, server.players[BLACK].pos);
    ASSERT_EQUAL(2, server.players[WHITE].num_wall);
    ASSERT_EQUAL(1, server.players[BLACK].num_wall);

    moveWHITE = (struct move_t){.c = WHITE, .e = {no_edge(), no_edge()}, .m = 5, .t = MOVE};
    update(&server, moveWHITE);
    // ASSERT move to pos 5 has been applied correctly
    ASSERT_EQUAL(5, server.players[WHITE].pos);
    ASSERT_EQUAL(4, server.players[BLACK].pos);
    ASSERT_EQUAL(2, server.players[WHITE].num_wall);
    ASSERT_EQUAL(1, server.players[BLACK].num_wall);

    moveWHITE = (struct move_t){.c = WHITE, .e = {{0, 1}, {5, 6}}, .m = 0, .t = WALL};
    update(&server, moveWHITE);
    // ASSERT put a wall between 0-1 and 5-6 has been applied correctly
    size_t connection_0_1 = is_connected(server.graph.graph, 0, 1);
    size_t connection_5_6 = is_connected(server.graph.graph, 5, 6);
    ASSERT_EQUAL(5, server.players[WHITE].pos);      // has not changed the player position
    ASSERT_EQUAL(4, server.players[BLACK].pos);      // has not changed the player position
    ASSERT_EQUAL(1, server.players[WHITE].num_wall); // number of wall of the player has decrease
    ASSERT_EQUAL(1, server.players[BLACK].num_wall); // number of wall of the player has decrease
    ASSERT_TRUE(!connection_0_1 && !connection_5_6); // there is no more connection between cell 0-1 and 5-6

    graph_free(server.graph.graph);
    return 1;
}

int test__initialize_server()
{
    struct server *server = initialize_server("./install/random_1.so", "./install/random_2.so", 6, 'c');
    // ASSERT graph is not NULL
    ASSERT_NOT_NULL(server->graph.graph);
    
    // ASSERT BLACK players functions are not NULL
    ASSERT_NOT_NULL(server->players[BLACK].lib);
    ASSERT_NOT_NULL(server->players[BLACK].initialize);
    ASSERT_NOT_NULL(server->players[BLACK].get_player_name);
    ASSERT_NOT_NULL(server->players[BLACK].play);
    ASSERT_NOT_NULL(server->players[BLACK].finalize);

    // ASSERT WHITE players functions are not NULL
    ASSERT_NOT_NULL(server->players[WHITE].lib);
    ASSERT_NOT_NULL(server->players[WHITE].initialize);
    ASSERT_NOT_NULL(server->players[WHITE].get_player_name);
    ASSERT_NOT_NULL(server->players[WHITE].play);
    ASSERT_NOT_NULL(server->players[WHITE].finalize);

    free_server(server);
    return 1;
}

// Simple test to look if a game terminate with a winner
// It is run with a game that will forcefully terminate by a winner
int test__run_server()
{
    struct server *server = initialize_server("./install/random_1.so", "./install/ForrestGump_1.so", 2, 'c');
    run_server(server, 0, 0);
    ASSERT_TRUE(is_winning(server->graph.graph, BLACK, server->players[BLACK].pos) || is_winning(server->graph.graph, WHITE, server->players[WHITE].pos));
    free_server(server);
    return 1;
}

void tests__server_functions()
{
    TEST_FUNCTION(test__get_number_of_walls);
    TEST_FUNCTION(test__initialize_graph);
    TEST_FUNCTION(test__get_initial_move);
    TEST_FUNCTION(test__update);
    TEST_FUNCTION(test__initialize_server);
    TEST_FUNCTION(test__run_server);
}