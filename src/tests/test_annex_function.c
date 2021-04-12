#include <assert.h>

#include "utils.h"

#define TESTCASE(X, Y) printf("%s ... %s\n", X, Y() ? "PASSED" : "FAILED");

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

    graph_free(graph1);
    graph_free(graph2);
    graph_free(graph3);

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
    graph_free(graph1);

    struct graph_t* graph2 = get_graph('c', 4);
    struct edge_t e1[2] = {
            { 4, 5 },
            { 8, 9 }
    };
    add_wall(graph2, e1);
    assert(can_player_move_to(graph2, 9, 0, 6, 5) == 1);
    assert(can_player_move_to(graph2, 1, 0, 6, 5) == 1);
    assert(can_player_move_to(graph2, 4, 0, 6, 5) == 0);
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

    return 1;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    TESTCASE("Test of can_add_wall", test_can_add_wall);
    TESTCASE("Test of add_wall", test_add_wall);
    TESTCASE("Test of can_player_move_to", test_can_player_move_to);
    TESTCASE("Test of is_move_legal", test_is_move_legal);
    return 0;
}