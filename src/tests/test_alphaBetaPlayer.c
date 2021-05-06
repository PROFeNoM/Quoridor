#include <time.h>

#include "utils.h"
#include "list.h"
#include "correlation_graph.h"
#include "graph.h"
#include "graph_pattern.h"
#include "test.h"

#define PATH_LIB "./install/1alphaBetaPlayer_1.so"

struct player_functions {
	void* lib;
	int (*get_distance_between_positions)(struct graph_t* graph, size_t initial_position, size_t target_position);
	int (*get_minimal_distance_to_opponent_area)(struct graph_t* graph, int opponent, size_t player_position);
	int (*heuristic_evaluation)(struct graph_t* graph, int active_player, int opponent, size_t active_player_position, size_t opponent_position);
	int (*get_vertex_by_connection_type)(struct graph_t* graph, size_t from, int direction);
	//void (*get_vertices_to_test)(size_t vertices_to_test[], size_t position);
	//struct list* (*get_legal_moves)(int active_player, struct graph_t* graph, size_t active_player_position, size_t opponent_player_position, size_t num_walls);
	struct graph_t* (*get_next_graph)(struct graph_t* graph, struct move_t* move);
	size_t (*get_next_player_position)(struct move_t* move, int player, size_t player_position);
	int (*minimax_alphabeta)(struct graph_t* graph, int active_player, size_t active_player_position, size_t active_player_walls,
			int depth, int is_maximizing_player, int opponent_player, size_t opponent_player_position, size_t opponent_player_walls,
			double alpha, double beta, time_t end_time);
};

struct player_functions* player_functions;

void* load_player_alphaBeta_functions(struct player_functions* player)
{
	static const char* function_name[] = { "get_distance_between_positions", "get_minimal_distance_to_opponent_area",
										"heuristic_evaluation", "get_vertex_by_connection_type", "get_next_graph",
										"get_next_player_position", "minimax_alphabeta" };

	if (!(player->get_distance_between_positions = load_function(player->lib, function_name[0])))
		return NULL;
	if (!(player->get_minimal_distance_to_opponent_area = load_function(player->lib, function_name[1])))
		return NULL;
	if (!(player->heuristic_evaluation = load_function(player->lib, function_name[2])))
		return NULL;
	if (!(player->get_vertex_by_connection_type = load_function(player->lib, function_name[3])))
		return NULL;
	if (!(player->get_next_graph = load_function(player->lib, function_name[4])))
		return NULL;
	if (!(player->get_next_player_position = load_function(player->lib, function_name[5])))
		return NULL;
	if (!(player->minimax_alphabeta = load_function(player->lib, function_name[6])))
		return NULL;

	return player;
}
void load_player_alphaBeta(struct player_functions* player, char* path_lib_player)
{
	if (!(player->lib = load_library(path_lib_player)))
	{
		fprintf(stderr, "Error when loading player's library : %s\n", dlerror());
		exit(1);
	}

	if (!load_player_alphaBeta_functions(player))
	{
		fprintf(stderr, "Error when loading player's functions : %s\n", dlerror());
		exit(1);
	}
}

int test__get_distance_between_positions_without_obstacle()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(3, player_functions->get_distance_between_positions(graph1, 0, 12));
	graph_free(graph1);
	return 1;
}

int test__get_distance_between_positions_with_wall()
{
	struct graph_t* graph1 = get_graph('c', 4);
	struct edge_t e1[2] = {
			{ 0, 4 },
			{ 1, 5 }
	};
	add_wall(graph1, e1);
	ASSERT_EQUAL(7, player_functions->get_distance_between_positions(graph1, 0, 12));
	graph_free(graph1);
	return 1;
}

int test__get_distance_between_positions_toric_board()
{
	struct graph_t* graph1 = get_graph('t', 3);
	ASSERT_EQUAL(4, player_functions->get_distance_between_positions(graph1, 1, 7));
	graph_free(graph1);
	return 1;
}

int test__get_minimal_distance_to_opponent_area_without_obstacle()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(3, player_functions->get_minimal_distance_to_opponent_area(graph1, WHITE, 0));
	graph_free(graph1);
	return 1;
}

int test__get_minimal_distance_to_opponent_area_with_wall()
{
	struct graph_t* graph1 = get_graph('c', 4);
	struct edge_t e1[2] = {
			{ 1, 5 },
			{ 2, 6 }
	};
	add_wall(graph1, e1);
	ASSERT_EQUAL(3, player_functions->get_minimal_distance_to_opponent_area(graph1, BLACK, 10));
	graph_free(graph1);
	return 1;
}

int test__heuristic_evaluation_winning_situation()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(1000, player_functions->heuristic_evaluation(graph1, WHITE, BLACK, 0, 8));
	graph_free(graph1);
	return 1;
}

int test__heuristic_evaluation_losing_situation()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(-1000, player_functions->heuristic_evaluation(graph1, BLACK, WHITE, 8, 0));
	graph_free(graph1);
	return 1;
}

int test__heuristic_evaluation_distance_situation()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(-1, player_functions->heuristic_evaluation(graph1, BLACK, WHITE, 0, 8));
	graph_free(graph1);
	return 1;
}

int test__get_vertex_by_connection_type_connected()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(4,player_functions->get_vertex_by_connection_type(graph1, 0, SOUTH));
	graph_free(graph1);
	return 1;
}

int test__get_vertex_by_connection_type_unconnected()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(-1, player_functions->get_vertex_by_connection_type(graph1, 0, NORTH));
	graph_free(graph1);
	return 1;
}

int test__get_next_graph_wall()
{
	struct graph_t* graph1 = get_graph('c', 4);
	struct edge_t e1[2] = {
			{ 0, 4 },
			{ 1, 5 }
	};
	struct move_t move;
	move.e[0] = e1[0];
	move.e[1] = e1[1];
	move.t = WALL;
	struct graph_t* graph2 = player_functions->get_next_graph(graph1, &move);
	ASSERT_EQUAL(POINT_TO_EAST, get_connection_type(graph2, 0, 4));
	ASSERT_EQUAL(POINT_TO_WEST, get_connection_type(graph2, 1, 5));
	graph_free(graph1);
	graph_free(graph2);
	return 1;
}

int test__get_next_player_position_wall()
{
	struct edge_t e1[2] = {
			{ 0, 4 },
			{ 1, 5 }
	};
	struct move_t move;
	move.e[0] = e1[0];
	move.e[1] = e1[1];
	move.t = WALL;
	ASSERT_EQUAL(3, player_functions->get_next_player_position(&move, BLACK, 3));
	return 1;
}

int test__get_next_player_position_move_by_player()
{
	struct move_t move;
	move.m = 2;
	move.t = MOVE;
	move.c = BLACK;
	ASSERT_EQUAL(2, player_functions->get_next_player_position(&move, BLACK, 3));
	return 1;
}

int test__get_next_player_position_move_by_opponent()
{
	struct move_t move;
	move.m = 2;
	move.t = MOVE;
	move.c = WHITE;
	ASSERT_EQUAL(3, player_functions->get_next_player_position(&move, BLACK, 3));
	return 1;
}

int test__minimax_alphabeta_winning()
{
	struct graph_t* graph1 = get_graph('c', 2);
	ASSERT_EQUAL(1000, player_functions->minimax_alphabeta(graph1, WHITE, 1, 1,
			2, 1, BLACK, 2, 0,
			-1000, 1000, 4));
	graph_free(graph1);
	return 1;
}

int test__minimax_alphabeta_losing()
{
	struct graph_t* graph1 = get_graph('c', 2);
	ASSERT_EQUAL(-1000, player_functions->minimax_alphabeta(graph1, BLACK, 0, 0,
			2, 1, WHITE, 1, 1,
			-1000, 1000, 4));
	graph_free(graph1);
	return 1;
}

int test__minimax_alphabeta_even()
{
	struct graph_t* graph1 = get_graph('c', 2);
	ASSERT_EQUAL(0, player_functions->minimax_alphabeta(graph1, BLACK, 0, 0,
			2, 1, WHITE, 2, 0,
			-1000, 1000, 4));
	graph_free(graph1);
	return 1;
}

int test__minimax_alphabeta_distance_advantage()
{
	struct graph_t* graph1 = get_graph('c', 4);
	ASSERT_EQUAL(1, player_functions->minimax_alphabeta(graph1, BLACK, 5, 2,
			3, 1, WHITE, 12, 0,
			-1000, 1000, 6));
	graph_free(graph1);
	return 1;
}

void tests__alphaBetaPlayer_functions()
{
	player_functions = malloc(sizeof(struct player_functions));
	load_player_alphaBeta(player_functions, PATH_LIB);

	TEST_FUNCTION(test__get_distance_between_positions_without_obstacle);
	TEST_FUNCTION(test__get_distance_between_positions_with_wall);
	TEST_FUNCTION(test__get_distance_between_positions_toric_board);
	TEST_FUNCTION(test__get_minimal_distance_to_opponent_area_without_obstacle);
	TEST_FUNCTION(test__get_distance_between_positions_with_wall);
	TEST_FUNCTION(test__heuristic_evaluation_winning_situation);
	TEST_FUNCTION(test__heuristic_evaluation_losing_situation);
	TEST_FUNCTION(test__heuristic_evaluation_distance_situation);
	TEST_FUNCTION(test__get_vertex_by_connection_type_connected);
	TEST_FUNCTION(test__get_vertex_by_connection_type_unconnected);
	TEST_FUNCTION(test__get_next_graph_wall);
	TEST_FUNCTION(test__get_next_player_position_wall);
	TEST_FUNCTION(test__get_next_player_position_move_by_player);
	TEST_FUNCTION(test__get_next_player_position_move_by_opponent);
	TEST_FUNCTION(test__minimax_alphabeta_winning);
	TEST_FUNCTION(test__minimax_alphabeta_losing);
	TEST_FUNCTION(test__minimax_alphabeta_even);
	TEST_FUNCTION(test__minimax_alphabeta_distance_advantage);

	dlclose(player_functions->lib);
	free(player_functions);
}