#include <assert.h>

#include "utils.h"

#define TESTCASE(X, Y) printf("%s ... %s\n", X, Y() ? "PASSED" : "FAILED");

int test__is_winning_true_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    assert(is_winning(graph1, WHITE, 0) == 1);
    graph_free(graph1);

    return 1;
}

int test__is_winning_false_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    assert(is_winning(graph1, BLACK, 4) == 0);
    graph_free(graph1);

    return 1;
}

int test__is_owned_with_owner_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    assert(is_owned(graph1, BLACK, 0) == 1);
    graph_free(graph1);
    return 1;
}

int test__is_owned_with_opponent_square_board()
{
    struct graph_t* graph1 = get_graph('c', 3);
    assert(is_owned(graph1, WHITE, 0) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_owned_with_a_vertex_in_a_hole_h_board()
{
    struct graph_t* graph1 = get_graph('h', 3);
    assert(is_owned(graph1, BLACK, 1) == 0);
    assert(is_owned(graph1, WHITE, 1) == 0);
    graph_free(graph1);
    return 1;
}

int test__get_connection_type_square_board()
{
    struct graph_t* graph1 = get_graph('c', 2);
    for (size_t i = 0; i < 4; i++)
        assert(get_connection_type(graph1, i, i) == NO_DIRECTION);
    assert(get_connection_type(graph1, 0, 1) == EAST);
    assert(get_connection_type(graph1, 0, 2) == SOUTH);
    assert(get_connection_type(graph1, 0, 3) == NO_DIRECTION);
    assert(get_connection_type(graph1, 1, 0) == WEST);
    assert(get_connection_type(graph1, 1, 2) == NO_DIRECTION);
    assert(get_connection_type(graph1, 1, 3) == SOUTH);
    assert(get_connection_type(graph1, 2, 0) == NORTH);
    assert(get_connection_type(graph1, 2, 1) == NO_DIRECTION);
    assert(get_connection_type(graph1, 2, 3) == EAST);
    assert(get_connection_type(graph1, 3, 0) == NO_DIRECTION);
    assert(get_connection_type(graph1, 3, 1) == NORTH);
    assert(get_connection_type(graph1, 3, 2) == WEST);

    graph_free(graph1);
    return 1;
}

int test__set_connection_type()
{
    struct graph_t* graph1 = get_graph('c', 2);
    set_connection_type(graph1, 0, 1, NO_DIRECTION);
    assert(get_connection_type(graph1, 0, 1) == NO_DIRECTION);
    graph_free(graph1);
    return 1;
}

int test__is_connected_with_connected_vertices()
{
    struct graph_t* graph1 = get_graph('c', 2);
    assert(is_connected(graph1, 0, 1) == 1);
    graph_free(graph1);
    return 1;
}

int test__is_connected_with_unconnected_vertices()
{
    struct graph_t* graph1 = get_graph('c', 2);
    assert(is_connected(graph1, 0, 3) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_connected_with_wall_between_vertices()
{
    struct graph_t* graph1 = get_graph('c', 3);
    struct edge_t e1[2] = {
            { 0, 1 },
            { 3, 4 }
    };
    add_wall(graph1, e1);
    assert(is_connected(graph1, 0, 1) == 0);
    graph_free(graph1);
    return 1;
}

int test__is_vertex_in_graph_with_vertex_in_graph()
{
    struct graph_t* graph1 = get_graph('c', 2);
    assert(is_vertex_in_graph(graph1, 0) == 1);
    graph_free(graph1);
    return 1;
}

int test__is_vertex_in_graph_with_vertex_out()
{
    struct graph_t* graph1 = get_graph('h', 3);
    assert(is_vertex_in_graph(graph1, 1) == 0);
    graph_free(graph1);
    return 1;
}

int test_can_add_wall()
{
    struct edge_t e1[2] = {
            { 0, 1 },
            { 2, 3 }
    };

    struct graph_t *graph1 = get_graph('c', 2);
    assert(can_add_wall(graph1, e1, 0, 1) == 1);


    struct edge_t e21[2] = {
            { 0, 3 },
            { 1, 4 }
    };

    struct graph_t *graph2 = get_graph('c', 3);
    assert(can_add_wall(graph2, e21, 1, 7) == 1);

    add_wall(graph2, e21);

    struct edge_t e22[2] = {
            { 2, 5 },
            { 1, 4 }
    };
    assert(can_add_wall(graph2, e22, 1, 7) == 0);

    struct edge_t e23[2] = {
            { 3, 4 },
            { 6, 7 }
    };
    assert(can_add_wall(graph2, e23, 1, 7) == 1);
    add_wall(graph2, e23);

    struct edge_t e24[2] = {
            { 1, 4 },
            { 2, 5 }
    };
    assert(can_add_wall(graph2, e24, 1, 7) == 0);

    struct edge_t e31[2] = {
            { 4, 8 },
            { 5, 9 }
    };

    struct graph_t *graph3 = get_graph('c', 4);
    assert(can_add_wall(graph3, e31, 0, 15) == 1);
    add_wall(graph3, e31);

    struct graph_t* graph4 = get_graph('c', 4);
    struct edge_t e41[2] = {
			{ 3, 2 },
			{ 7, 6 }
    };
    struct edge_t e42[2] = {
			{ 10, 9 },
			{ 6, 5 }
    };
    struct edge_t e43[2] = {
			{ 10, 14 },
			{ 9, 13 }
    };
	assert(can_add_wall(graph4, e41, 2, 13) == 1);
    add_wall(graph4, e41);
	assert(can_add_wall(graph4, e42, 2, 12) == 1);
    add_wall(graph4, e42);
	assert(can_add_wall(graph4, e43, 2, 13) == 1);
	add_wall(graph4, e43);

    graph_free(graph1);
    graph_free(graph2);
    graph_free(graph3);
    graph_free(graph4);

    return 1;
}


int test_add_wall()
{
    struct edge_t e1[2] = {
            { 0, 1 },
            { 2, 3 }
    };

    struct graph_t *graph1 = get_graph('c', 2);

    add_wall(graph1, e1);

    assert(get_connection_type(graph1, 0, 1) == POINT_TO_SOUTH);
    assert(get_connection_type(graph1, 2, 3) == POINT_TO_NORTH);

    struct edge_t e2[2] = {
            { 1, 3 },
            { 0, 2 }
    };

    add_wall(graph1, e2);
    assert(get_connection_type(graph1, 1, 3) == POINT_TO_WEST);
    assert(get_connection_type(graph1, 0, 2) == POINT_TO_EAST);

    struct edge_t e1bis[2] = {
            { 2, 3 },
            { 0, 1 },
    };

    struct graph_t *graph2 = get_graph('c', 2);

    add_wall(graph2, e1bis);

    assert(get_connection_type(graph2, 0, 1) == POINT_TO_SOUTH);
    assert(get_connection_type(graph2, 2, 3) == POINT_TO_NORTH);

    struct edge_t e2bis[2] = {
            { 0, 2 },
            { 1, 3 },
    };

    add_wall(graph2, e2bis);
    assert(get_connection_type(graph2, 1, 3) == POINT_TO_WEST);
    assert(get_connection_type(graph2, 0, 2) == POINT_TO_EAST);

    graph_free(graph1);
    graph_free(graph2);

    return 1;
}


int test_can_player_move_to()
{
    struct graph_t* graph1 = get_graph('c', 3);
    assert(can_player_move_to(graph1, 1, 0, 4, 2) == 1);
    assert(can_player_move_to(graph1, 0, 1, 1, 6) == 0);
    assert(can_player_move_to(graph1, 2, 0, 0, 1) == 1);

    struct graph_t* graph2 = get_graph('c', 4);
    struct edge_t e1[2] = {
            { 4, 5 },
            { 8, 9 }
    };
    add_wall(graph2, e1);
    assert(can_player_move_to(graph2, 9, 0, 6, 5) == 1);
    assert(can_player_move_to(graph2, 1, 0, 6, 5) == 1);
    assert(can_player_move_to(graph2, 4, 0, 6, 5) == 0);

    graph_free(graph1);
    graph_free(graph2);

    return 1;
}

int test_is_move_legal()
{
    struct graph_t* graph1 = get_graph('c', 3);

    struct move_t m1 = { 1, { no_edge(), no_edge() }, MOVE, BLACK };
    assert(is_move_legal(graph1, &m1, 4, 2) == 1);

    struct move_t m2 = { 0, { no_edge(), no_edge() }, MOVE, WHITE };
    assert(is_move_legal(graph1, &m2, 1, 6) == 0);

    struct move_t m3 = { 2, { no_edge(), no_edge() }, MOVE, BLACK };
    assert(is_move_legal(graph1, &m3, 0, 1) == 1);


    struct graph_t* graph2 = get_graph('c', 4);
    struct edge_t e1[2] = {
            { 4, 5 },
            { 8, 9 }
    };
    add_wall(graph2, e1);

    struct move_t m4 = { 9, { no_edge(), no_edge() }, MOVE, BLACK };
    assert(is_move_legal(graph2, &m4, 6, 5) == 1);

    struct move_t m5 = { 1, { no_edge(), no_edge() }, MOVE, BLACK };
    assert(is_move_legal(graph2, &m5, 6, 5) == 1);

    struct move_t m6 = { 4, { no_edge(), no_edge() }, MOVE, BLACK };
    assert(is_move_legal(graph2, &m6, 6, 5) == 0);

	struct graph_t* graph3 = get_graph('c', 4);
	struct move_t m7 = { -1, { { 0, 4 }, { 1, 5 } }, WALL, BLACK };
	assert(is_move_legal(graph3, &m7, 3, 14) == 1);

	struct graph_t* graph4 = get_graph('c', 2);
	struct move_t m8 = { -1, { { 0, 1 }, { 2, 3 } }, WALL, BLACK };
	assert(is_move_legal(graph4, &m8, 1, 2) == 1);
	struct move_t m9 = { -1, { { 0, 2 }, { 1, 3 } }, WALL, BLACK };
	assert(is_move_legal(graph4, &m9, 1, 2) == 0);

    graph_free(graph1);
    graph_free(graph2);
	graph_free(graph3);
	graph_free(graph4);

    return 1;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    TESTCASE("Test of is_winning where player is in opponent area on a square board",
             test__is_winning_true_square_board);
    TESTCASE("Test of is_winning where player isn't in opponent area on a square board",
             test__is_winning_false_square_board);
    TESTCASE("Test of is_owned with the vertex owner on a square board",
             test__is_owned_with_owner_square_board);
    TESTCASE("Test of is_owned with the opponent on a square board",
             test__is_owned_with_opponent_square_board);
    TESTCASE("Test of is_owned with a vertex in a hole on a 'h' type board",
             test__is_owned_with_a_vertex_in_a_hole_h_board);
    TESTCASE("Test of get_connection_type with a square board",
             test__get_connection_type_square_board);
    TESTCASE("Test of set_connection_type",
             test__set_connection_type);
    TESTCASE("Test of is_connected with connected vertices",
             test__is_connected_with_connected_vertices);
    TESTCASE("Test of is_connected with unconnected vertices",
             test__is_connected_with_unconnected_vertices);
    TESTCASE("Test of is_connected with a wall between vertices",
             test__is_connected_with_wall_between_vertices);
    TESTCASE("Test of is_vertex_in_graph with a vertex in graph",
             test__is_vertex_in_graph_with_vertex_in_graph);
    //TESTCASE("TEst of is_vertex_in_graph with a vertex out of the graph",
    //         test__is_vertex_in_graph_with_vertex_out);
    TESTCASE("Test of can_add_wall", test_can_add_wall);
    TESTCASE("Test of add_wall", test_add_wall);
    TESTCASE("Test of can_player_move_to", test_can_player_move_to);
    TESTCASE("Test of is_move_legal", test_is_move_legal);
    return 0;
}