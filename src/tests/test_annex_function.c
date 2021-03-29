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
    struct player_server p1 = { .pos = 0 };
    struct player_server p2 = { .pos = 1 };
    struct player_server players1[2] = { p1, p2 };
    assert(can_add_wall(graph1, e1, players1) == 1);


    struct edge_t e21[2] = {
            { 0, 3 },
            { 1, 4 }
    };

    struct graph_t *graph2 = get_graph('c', 3);
    p1.pos = 1;
    p2.pos = 7;
    struct player_server players2[2] = { p1, p2 };
    assert(can_add_wall(graph2, e21, players2) == 1);

    add_wall(graph2, e21);

    struct edge_t e22[2] = {
            { 2, 5 },
            { 1, 4 }
    };
    assert(can_add_wall(graph2, e22, players2) == 0);

    struct edge_t e23[2] = {
            { 3, 4 },
            { 6, 7 }
    };
    assert(can_add_wall(graph2, e23, players2) == 1);
    add_wall(graph2, e23);

    struct edge_t e24[2] = {
            { 1, 4 },
            { 2, 5 }
    };
    assert(can_add_wall(graph2, e24, players2) == 0);

    struct edge_t e31[2] = {
            { 4, 8 },
            { 5, 9 }
    };

    struct graph_t *graph3 = get_graph('c', 4);
    p1.pos = 0;
    p2.pos = 15;
    struct player_server players3[2] = { p1, p2 };
    assert(can_add_wall(graph3, e31, players3) == 1);
    add_wall(graph3, e31);

    return 1;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    TESTCASE("Test of can_add_wall", test_can_add_wall);
    return 0;
}