#include "utils.h"
#include "list.h"
#include "correlation_graph.h"
#include "graph.h"
#include "graph_pattern.h"
#include "test.h"

#define PATH_LIB "./install/random_1.so"

struct player_functions {
  void *lib;
  struct move_t (*set_move)(size_t position, struct edge_t edge1, struct edge_t edge2, size_t id, enum movetype_t movetype);
  struct move_t (*get_first_move)();
  struct move_t (*get_new_move)();
  void (*update)(struct move_t previous_move);
  struct move_t (*play)(struct move_t previous_move);
  void (*initialize)(enum color_t id, struct graph_t *graph, size_t num_walls);
  void (*get_available_positions)(size_t *nb_pos, size_t pos[], size_t position_player1, size_t position_player2);
};

void* load_player_random_functions(struct player_functions* player)
{
  static const char* function_name[] = {"set_move", "get_first_move", "get_new_move", "update", "play", "initialize","get_available_positions"};

  if (!(player->set_move = load_function(player->lib, function_name[0])))
    return NULL;
  if (!(player->get_first_move = load_function(player->lib, function_name[1])))
    return NULL;
  if (!(player->get_new_move = load_function(player->lib, function_name[2])))
    return NULL;
  if (!(player->update = load_function(player->lib, function_name[3])))
    return NULL;
  if (!(player->play = load_function(player->lib, function_name[4])))
    return NULL;
  if(!(player->initialize = load_function(player->lib, function_name[5])))
    return NULL;
  if(!(player->get_available_positions = load_function(player->lib, function_name[6])))
    return NULL;

  return player;
}

void load_player_random(struct player_functions* player, char* path_lib_player)
{
	if (!(player->lib = load_library(path_lib_player)))
	{
		fprintf(stderr, "Error when loading player's library : %s\n", dlerror());
		exit(1);
	}

	if (!load_player_random_functions(player))
	{
		fprintf(stderr, "Error when loading player's functions : %s\n", dlerror());
		exit(1);
	}
}

int test__get_available_positions()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_random(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  
  struct edge_t e1[2] = {
			{2,7},
			{3,8}
  };
  struct edge_t e2[2] = {
			{12,17},
			{13,18}
  };
  add_wall(graph, e1);
  add_wall(graph, e2);
  
  player_functions->initialize(BLACK, graph_copy(graph), 2);
  
  struct move_t move = player_functions->set_move(12, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(move);
  move = player_functions->set_move(7, no_edge(), no_edge(), WHITE, MOVE);
  player_functions->update(move);

  size_t nb_pos = 0;
  size_t pos[12];
  
  size_t position_player1 = 12;
  size_t position_player2 = 7;

  player_functions->get_available_positions(&nb_pos, pos, position_player1, position_player2);

  size_t expected[4] = {6,8,11,13};
  ASSERT_EQUAL(4, nb_pos);
  ASSERT_ARRAY_EQUAL(expected, pos, 4);
  
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__play_in_a_corner()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_random(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  player_functions->initialize(BLACK, graph_copy(graph), 2);
  struct move_t move = player_functions->set_move(4, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(move);

  struct move_t other_player_move = player_functions->set_move(16, no_edge(), no_edge(), WHITE, MOVE);

  struct move_t new_move = player_functions->play(other_player_move);
  ASSERT_TRUE(can_player_move_to(graph, new_move.m, BLACK, 4, 16));

  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__play_around_walls()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_random(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);

  struct edge_t e1[2] = {
			{6,11},
			{7,12}
  };
  struct edge_t e2[2] = {
			{12,13},
			{17,18}
  };
  struct edge_t e3[2] = {
			 {16,11},
			 {12,17}
  };
  struct edge_t e4[2] = {
			 {10,11},
			 {15,16}
  };
  
  add_wall(graph, e1);
  add_wall(graph, e2);
  add_wall(graph, e3);
  add_wall(graph, e4);
  
  player_functions->initialize(BLACK, graph_copy(graph), 2);
  struct move_t move = player_functions->set_move(12, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(move);

  struct move_t other_player_move = player_functions->set_move(16, no_edge(), no_edge(), WHITE, MOVE);

  struct move_t new_move = player_functions->play(other_player_move);
  ASSERT_EQUAL(11, new_move.m);

  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__play_around_walls_with_jump()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_random(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);

  struct edge_t e1[2] = {
			{6,11},
			{7,12}
  };
  struct edge_t e2[2] = {
			{12,13},
			{17,18}
  };
  struct edge_t e3[2] = {
			 {16,11},
			 {12,17}
  };
  
  add_wall(graph, e1);
  add_wall(graph, e2);
  add_wall(graph, e3);
  
  player_functions->initialize(BLACK, graph_copy(graph), 2);
  struct move_t move = player_functions->set_move(12, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(move);

  struct move_t other_player_move = player_functions->set_move(11, no_edge(), no_edge(), WHITE, MOVE);

  struct move_t new_move = player_functions->play(other_player_move);
  ASSERT_EQUAL(10, new_move.m);

  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

void tests__random_functions()
{
  TEST_FUNCTION(test__get_available_positions);
  TEST_FUNCTION(test__play_in_a_corner);
  TEST_FUNCTION(test__play_around_walls);
  TEST_FUNCTION(test__play_around_walls_with_jump);
}
