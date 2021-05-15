#include "correlation_graph.h"
#include "test.h"

int test__get_correlated_graph()
{
    struct graph_t *graph = get_graph('t', 6);
    struct near_neighbours *neighbours = get_correlated_graph(graph);

    for (size_t i = 0; i < graph->t->size1; i++)
    {
        for (size_t j = 0; j < graph->t->size2; j++)
        {
            int connection_type = get_connection_type(graph, i, j);
            if (connection_type > NO_DIRECTION && connection_type < MAX_DIRECTION) {
                switch (connection_type)
                {
                case NORTH:
                    ASSERT_EQUAL(j, neighbours[i].north);
                    break;
                case SOUTH:
                    ASSERT_EQUAL(j, neighbours[i].south);
                    break;
                case WEST:
                    ASSERT_EQUAL(j, neighbours[i].west);
                    break;
                case EAST:
                    ASSERT_EQUAL(j, neighbours[i].east);
                    break;
                default:
                    ASSERT_FALSE(1);
                    break;
                }
            }
        }
    }
    graph_free(graph);
    free_correlation_graph(neighbours);
    return 1;
}

void tests__correlation_graph_functions()
{
    TEST_FUNCTION(test__get_correlated_graph);
}