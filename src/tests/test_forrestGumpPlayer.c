#include "utils.h"
#include "list.h"
#include "correlation_graph.h"
#include "graph.h"
#include "graph_pattern.h"
#include "test.h"

#define PATH_LIB "./install/ForrestGump_1.so"

struct player_functions {
  void *lib;
  struct move_t (*set_move)(size_t position, struct edge_t edge1, struct edge_t edge2, size_t id, enum movetype_t movetype);
  struct move_t (*get_first_move)();
  void (*get_vertices_to_test)(size_t vertices_to_test[], size_t position);
  struct move_t (*get_new_move)();
  int (*is_first_move)();
  void (*update)(struct move_t previous_move);
  struct move_t (*play)(struct move_t previous_move);
  void (*finalize)();
  void (*initialize)(enum color_t id, struct graph_t *graph, size_t num_walls);
};

void* load_player_ForrestGump_functions(struct player_functions* player)
{
  static const char* function_name[] = {"set_move", "get_first_move", "get_vertices_to_test", "get_new_move", "is_first_move", "update", "play", "finalize", "initialize"};

  if (!(player->set_move = load_function(player->lib, function_name[0])))
    return NULL;
  if (!(player->get_first_move = load_function(player->lib, function_name[1])))
    return NULL;
  if (!(player->get_vertices_to_test = load_function(player->lib, function_name[2])))
    return NULL;
  if (!(player->get_new_move = load_function(player->lib, function_name[3])))
    return NULL;
  if (!(player->is_first_move = load_function(player->lib, function_name[4])))
    return NULL;
  if (!(player->update = load_function(player->lib, function_name[5])))
    return NULL;
  if (!(player->play = load_function(player->lib, function_name[6])))
    return NULL;
  if (!(player->finalize = load_function(player->lib, function_name[7])))
    return NULL;
  if(!(player->initialize = load_function(player->lib, function_name[8])))
    return NULL;

  return player;
}

void load_player_ForrestGump(struct player_functions* player, char* path_lib_player)
{
	if (!(player->lib = load_library(path_lib_player)))
	{
		fprintf(stderr, "Error when loading player's library : %s\n", dlerror());
		exit(1);
	}

	if (!load_player_ForrestGump_functions(player))
	{
		fprintf(stderr, "Error when loading player's functions : %s\n", dlerror());
		exit(1);
	}
}

int test__set_move()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct move_t move = player_functions->set_move(2, no_edge(), no_edge(), BLACK, MOVE);
  struct edge_t noedge = no_edge();
  ASSERT_EQUAL(2, move.m);
  ASSERT_EQUAL(noedge.fr, move.e[0].fr);
  ASSERT_EQUAL(noedge.to, move.e[1].to);
  ASSERT_EQUAL(move.c, BLACK);
  ASSERT_EQUAL(move.t, MOVE);

  dlclose(player_functions->lib);
  free(player_functions);

  return 1;
}

int test__is_first_move_true()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 3);
  player_functions->initialize(BLACK, graph_copy(graph), 2);

  ASSERT_TRUE(player_functions->is_first_move());

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__is_first_move_false()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 3);
  player_functions->initialize(BLACK, graph_copy(graph), 2);
  struct move_t first_move = player_functions->get_first_move();
  player_functions->update(first_move);

  ASSERT_FALSE(player_functions->is_first_move());

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__get_first_move()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 3);
  player_functions->initialize(BLACK, graph_copy(graph), 2);
  struct move_t first_move = player_functions->get_first_move();
  
  ASSERT_TRUE(is_owned(graph, first_move.c, first_move.m));

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__get_vertices_to_test_BLACK_player()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  player_functions->initialize(BLACK, graph_copy(graph), 2);

  size_t position = 12;
  size_t vertices_to_test[12];

  player_functions->get_vertices_to_test(vertices_to_test, position);

  size_t expected[] = {22,17,18,16,13,11,10,14,7,6,8,2};

  ASSERT_ARRAY_EQUAL(expected, vertices_to_test, 12);

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__get_vertices_to_test_WHITE_player()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  player_functions->initialize(WHITE, graph_copy(graph), 2);

  size_t position = 12;
  size_t vertices_to_test[12];

  player_functions->get_vertices_to_test(vertices_to_test, position);

  size_t expected[] = {2,7,6,8,11,13,10,14,17,16,18,22};

  ASSERT_ARRAY_EQUAL(expected, vertices_to_test, 12);

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}


int test__get_new_move_without_other_player()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  player_functions->initialize(BLACK, graph_copy(graph), 2);

  struct move_t initial_move = player_functions->set_move(2, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(initial_move);

  struct move_t next_move =player_functions->get_new_move();

  ASSERT_EQUAL(BLACK, next_move.c);
  ASSERT_EQUAL(7, next_move.m);
  ASSERT_EQUAL(MOVE, next_move.t);

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__get_new_move_with_jump()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  player_functions->initialize(BLACK, graph_copy(graph), 2);

  struct move_t move = player_functions->set_move(2, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(move);

  move = player_functions->set_move(7, no_edge(), no_edge(), WHITE, MOVE);
  player_functions->update(move);

  struct move_t next_move = player_functions->get_new_move();

  ASSERT_EQUAL(BLACK, next_move.c);
  ASSERT_EQUAL(12, next_move.m);
  ASSERT_EQUAL(MOVE, next_move.t);

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__get_new_move_diagonal()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

  struct graph_t *graph = get_graph('c', 5);
  struct edge_t e[2] = {
			{6,11},
			{7,12}
  };
  add_wall(graph, e);
  player_functions->initialize(BLACK, graph_copy(graph), 2);

  struct move_t move = player_functions->set_move(2, no_edge(), no_edge(), BLACK, MOVE);
  player_functions->update(move);
  move = player_functions->set_move(7, no_edge(), no_edge(), WHITE, MOVE);
  player_functions->update(move);

  struct move_t next_move = player_functions->get_new_move();

  ASSERT_EQUAL(BLACK, next_move.c);
  ASSERT_EQUAL(8, next_move.m);
  ASSERT_EQUAL(MOVE, next_move.t);

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

int test__get_new_move_on_impossible_move()
{
  struct player_functions *player_functions = malloc(sizeof(struct player_functions));
  load_player_ForrestGump(player_functions, PATH_LIB);

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
  move = player_functions->set_move(11, no_edge(), no_edge(), WHITE, MOVE);
  player_functions->update(move);
  
  struct move_t next_move = player_functions->get_new_move();

  ASSERT_EQUAL(BLACK, next_move.c);
  ASSERT_EQUAL(12, next_move.m);
  ASSERT_EQUAL(NO_TYPE, next_move.t);

  player_functions->finalize();
  dlclose(player_functions->lib);
  free(player_functions);
  graph_free(graph);

  return 1;
}

void tests__forrestGump_functions()
{
  TEST_FUNCTION(test__set_move);
  TEST_FUNCTION(test__is_first_move_true);
  TEST_FUNCTION(test__is_first_move_false);
  TEST_FUNCTION(test__get_first_move);
  TEST_FUNCTION(test__get_vertices_to_test_BLACK_player);
  TEST_FUNCTION(test__get_vertices_to_test_WHITE_player);
  TEST_FUNCTION(test__get_new_move_without_other_player);
  TEST_FUNCTION(test__get_new_move_with_jump);
  TEST_FUNCTION(test__get_new_move_diagonal);
  TEST_FUNCTION(test__get_new_move_on_impossible_move);
}
