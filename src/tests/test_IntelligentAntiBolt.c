#include "string.h"
#include "utils.h"
#include "list.h"
#include "correlation_graph.h"
#include "graph.h"
#include "graph_pattern.h"
#include "test.h"

#define PATH_LIB "./install/1IntelligentAntiBolt_1.so"

struct player_functions {
	void* lib;
	char const *(*get_player_name)();
        void (*initialize)(enum color_t id, struct graph_t *graph, size_t num_walls);
  struct move_t (*set_move)(size_t position, struct edge_t edge1, struct edge_t edge2, size_t id, enum movetype_t movetype);
  struct move_t (*get_first_move)();
  size_t (*can_fast_forward)(struct graph_t *graph, size_t position, enum color_t id);
  struct move_t (*get_wall)();
  size_t (*get_dijsktra)();
  struct move_t (*get_new_move)();
  int (*is_first_move)();
  void (*update)(struct move_t previous_move);
  struct move_t (*play)(struct move_t previous_move);
  void (*finalize)();
};

void* load_player_IntelligentAntiBolt_functions(struct player_functions* player)
{
  static const char* function_name[] = { "get_player_name","initialize","set_move","get_first_move","can_fast_forward","get_wall","get_dijsktra","get_new_move","is_first_move","update","play","finalize"};

	if (!(player->get_player_name = load_function(player->lib, function_name[0])))
		return NULL;
	if (!(player->initialize = load_function(player->lib, function_name[1])))
		return NULL;
	if (!(player->set_move = load_function(player->lib, function_name[2])))
		return NULL;
	if (!(player->get_first_move = load_function(player->lib, function_name[3])))
		return NULL;
	if (!(player->can_fast_forward = load_function(player->lib, function_name[4])))
		return NULL;
	if (!(player->get_wall = load_function(player->lib, function_name[5])))
		return NULL;
	if (!(player->get_dijsktra = load_function(player->lib, function_name[6])))
		return NULL;
	if (!(player->get_new_move = load_function(player->lib, function_name[7])))
		return NULL;
	if (!(player->is_first_move = load_function(player->lib, function_name[8])))
		return NULL;
	if (!(player->update = load_function(player->lib, function_name[9])))
		return NULL;
	if (!(player->play = load_function(player->lib, function_name[10])))
		return NULL;
	if (!(player->finalize = load_function(player->lib, function_name[11])))
		return NULL;

	return player;
}

void load_player_IntelligentAntiBolt(struct player_functions* player, char* path_lib_player)
{
	if (!(player->lib = load_library(path_lib_player)))
	{
		fprintf(stderr, "Error when loading player's library : %s\n", dlerror());
		exit(1);
	}

	if (!load_player_IntelligentAntiBolt_functions(player))
	{
		fprintf(stderr, "Error when loading player's functions : %s\n", dlerror());
		exit(1);
	}
}

int test__if_the_player_name_is_right()
{
	struct player_functions* player_functions = malloc(sizeof(struct player_functions));
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);

	ASSERT_EQUAL(strcmp("IntelligentAntiBolt",player_functions->get_player_name()),0);

	dlclose(player_functions->lib);
	free(player_functions);
	return 1;
}

int test__if_the_initialization_does_not_make_crashs()
{
	struct player_functions* player_functions = malloc(sizeof(struct player_functions));
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);
	struct graph_t* graph1 = get_graph('c', 4);

	player_functions->initialize(WHITE,graph_copy(graph1),4);
	player_functions->finalize();

	dlclose(player_functions->lib);
	free(player_functions);
	graph_free(graph1);
	return 1;
}

int test__if_the_first_move_is_a_legal_placement()
{
	struct player_functions* player_functions = malloc(sizeof(struct player_functions));
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);

	struct graph_t* graph1 = get_graph('c', 9);
	player_functions->initialize(WHITE,graph_copy(graph1),4);
	struct move_t first_move1 = player_functions->get_first_move();
	ASSERT_EQUAL(1,is_owned(graph1,WHITE,first_move1.m));
	player_functions->finalize();
	graph_free(graph1);
	dlclose(player_functions->lib);
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);

	
	struct graph_t* graph2 = get_graph('s', 15);
	player_functions->initialize(BLACK,graph_copy(graph2),4);
	struct move_t first_move2 = player_functions->get_first_move();
	ASSERT_EQUAL(1,is_owned(graph2,BLACK,first_move2.m));
	player_functions->finalize();
	graph_free(graph2);
	dlclose(player_functions->lib);
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);
	
	struct graph_t* graph3 = get_graph('t', 21);
	player_functions->initialize(WHITE,graph_copy(graph3),4);
	struct move_t first_move3 = player_functions->get_first_move();
	ASSERT_EQUAL(1,is_owned(graph3,WHITE,first_move3.m));
	player_functions->finalize();
	graph_free(graph3);
	dlclose(player_functions->lib);
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);

	
	struct graph_t* graph4 = get_graph('h', 33);
	player_functions->initialize(BLACK,graph_copy(graph4),4);
	struct move_t first_move4 = player_functions->get_first_move();
	ASSERT_EQUAL(1,is_owned(graph4,BLACK,first_move4.m));
	player_functions->finalize();
	graph_free(graph4);
	

	dlclose(player_functions->lib);
	free(player_functions);
	return 1;
}

int test__can_fast_forward()
{
	struct player_functions* player_functions = malloc(sizeof(struct player_functions));
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);

	
	struct graph_t* graph1 = get_graph('t', 21);
	player_functions->initialize(BLACK,graph_copy(graph1),4);
	ASSERT_EQUAL(420,player_functions->can_fast_forward(graph1,0,BLACK));
	ASSERT_EQUAL(graph1->t->size1,player_functions->can_fast_forward(graph1,10,BLACK));
	ASSERT_EQUAL(420,player_functions->can_fast_forward(graph1,21,BLACK));
	ASSERT_EQUAL(20,player_functions->can_fast_forward(graph1,440,WHITE));
	ASSERT_EQUAL(graph1->t->size1,player_functions->can_fast_forward(graph1,430,WHITE));

	struct edge_t e1 = {.fr=19 , .to=40};
	struct edge_t e2 = {.fr=20 , .to=41};
	struct move_t move = player_functions->set_move(0,e1,e2,WHITE,WALL);
	struct edge_t e[2] = {e1,e2};
	player_functions->update(move);
	add_wall(graph1,e);
	ASSERT_EQUAL(20,player_functions->can_fast_forward(graph1,440,WHITE));
	
	graph_free(graph1);
	player_functions->finalize();
	dlclose(player_functions->lib);
	free(player_functions);
	return 1;
}

int test__strategy()
{
	struct player_functions* player_functions = malloc(sizeof(struct player_functions));
	load_player_IntelligentAntiBolt(player_functions, PATH_LIB);

	size_t size = 10;
	struct graph_t* graph1 = get_graph('c', size);
	player_functions->initialize(BLACK,graph_copy(graph1),2);

	struct move_t first_challenger_move = player_functions->set_move(size*size-1,no_edge(),no_edge(),WHITE,MOVE);
	player_functions->play(first_challenger_move);
	
	
	struct move_t reset_IntelligentAntiBolt_placement = player_functions->set_move(4,no_edge(),no_edge(),BLACK,MOVE);
	struct move_t second_IntelligentAntiBolt_move = player_functions->play(reset_IntelligentAntiBolt_placement);

	
	ASSERT_EQUAL(WALL,second_IntelligentAntiBolt_move.t);
	ASSERT_EQUAL(size-1,second_IntelligentAntiBolt_move.e[0].fr);
	ASSERT_EQUAL(2*size-1,second_IntelligentAntiBolt_move.e[0].to);
	ASSERT_EQUAL(size-2,second_IntelligentAntiBolt_move.e[1].fr);
	ASSERT_EQUAL(2*size-2,second_IntelligentAntiBolt_move.e[1].to);
	
	struct move_t third_challenger_move = player_functions->set_move(size*size-2,no_edge(),no_edge(),WHITE,MOVE);
	struct move_t third_IntelligentAntiBolt_move = player_functions->play(third_challenger_move);
	
	ASSERT_EQUAL(MOVE,third_IntelligentAntiBolt_move.t);
	ASSERT_EQUAL(reset_IntelligentAntiBolt_placement.m+size,third_IntelligentAntiBolt_move.m);
	
	struct move_t challenger_move_4 = player_functions->set_move(size*size-3,no_edge(),no_edge(),WHITE,MOVE);
	struct move_t IntelligentAntiBolt_move_4 = player_functions->play(challenger_move_4);

	ASSERT_EQUAL(WALL,IntelligentAntiBolt_move_4.t);
      	ASSERT_EQUAL(size-3,IntelligentAntiBolt_move_4.e[0].fr);
	ASSERT_EQUAL(2*size-3,IntelligentAntiBolt_move_4.e[0].to);
	ASSERT_EQUAL(size-4,IntelligentAntiBolt_move_4.e[1].fr);
	ASSERT_EQUAL(2*size-4,IntelligentAntiBolt_move_4.e[1].to);
	
	struct move_t challenger_move_5 = player_functions->set_move(size*size-4,no_edge(),no_edge(),WHITE,MOVE);
	struct move_t IntelligentAntiBolt_move_5 = player_functions->play(challenger_move_5);
	
	ASSERT_EQUAL(MOVE,IntelligentAntiBolt_move_5.t);
	ASSERT_EQUAL(IntelligentAntiBolt_move_4.m+size,IntelligentAntiBolt_move_5.m);
	
	struct move_t challenger_move_6 = player_functions->set_move(size*size-5,no_edge(),no_edge(),WHITE,MOVE);
	struct move_t IntelligentAntiBolt_move_6 = player_functions->play(challenger_move_6);
	
	ASSERT_EQUAL(MOVE,IntelligentAntiBolt_move_6.t);
	ASSERT_EQUAL(IntelligentAntiBolt_move_5.m+size,IntelligentAntiBolt_move_6.m);
	
	struct edge_t e1 = {.fr=IntelligentAntiBolt_move_6.m , .to=IntelligentAntiBolt_move_6.m+size};
	struct edge_t e2 = {.fr=IntelligentAntiBolt_move_6.m+1 , .to=IntelligentAntiBolt_move_6.m+size+1};
	
	struct move_t challenger_move_7 = player_functions->set_move(size*size-3,e1,e2,WHITE,WALL);
	struct move_t IntelligentAntiBolt_move_7 = player_functions->play(challenger_move_7);
	
	ASSERT_EQUAL(MOVE,IntelligentAntiBolt_move_7.t);
	ASSERT_EQUAL(IntelligentAntiBolt_move_6.m-1,IntelligentAntiBolt_move_7.m);
	
	struct move_t challenger_move_8 = player_functions->set_move(IntelligentAntiBolt_move_7.m+2*size,no_edge(),no_edge(),WHITE,MOVE);
	struct move_t IntelligentAntiBolt_move_8 = player_functions->play(challenger_move_8);
	
	ASSERT_EQUAL(MOVE,IntelligentAntiBolt_move_8.t);
	ASSERT_EQUAL(IntelligentAntiBolt_move_7.m+size,IntelligentAntiBolt_move_8.m);
	
	struct edge_t e1_1 = {.fr=challenger_move_8.m , .to=challenger_move_8.m+size};
	struct edge_t e2_1 = {.fr=challenger_move_8.m+1 , .to=challenger_move_8.m+size+1};
	struct move_t challenger_move_9 = player_functions->set_move(size*size-3,e1_1,e2_1,WHITE,WALL);
	struct move_t IntelligentAntiBolt_move_9 = player_functions->play(challenger_move_9);
	
	ASSERT_EQUAL(MOVE,IntelligentAntiBolt_move_9.t);
	ASSERT_EQUAL(IntelligentAntiBolt_move_8.m-1+size,IntelligentAntiBolt_move_9.m);
	

	
	graph_free(graph1);
	player_functions->finalize();
	dlclose(player_functions->lib);
	free(player_functions);
	return 1;
}


void tests__IntelligentAntiBolt_functions()
{
  TEST_FUNCTION(test__if_the_player_name_is_right);
  TEST_FUNCTION(test__if_the_initialization_does_not_make_crashs);
  //TEST_FUNCTION(test__if_the_first_move_is_a_legal_placement);
  TEST_FUNCTION(test__can_fast_forward);
  TEST_FUNCTION(test__strategy);
}
