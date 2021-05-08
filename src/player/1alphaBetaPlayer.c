#include <string.h>

#include <time.h>

#include "utils.h"
#include "player.h"
#include "list_dynamic.h"
#include "queue.h"
#include "correlation_graph.h"


#define UNINITIALIZED 0
#define UNINITIALIZED_MINIMAL_DISTANCE -42
//#define INF 1000000
#define CONNECTION_NOT_FOUND -1

const int INF = 1000000;

struct player
{
	struct graph_t *graph; //graph of the player
	struct near_neighbours* neighbours_graph; // Set the neighbour's index of the vertex from in a direction
	size_t position[2];    //position of the two players on the board
	size_t num_walls;      //number of walls in the hand of the player
	enum color_t id;       //id of the player
};

struct player player = {
		.graph = NULL,
		.position = {UNINITIALIZED, UNINITIALIZED},
		.id = -1,
		.num_walls = UNINITIALIZED
};

/*
 * Return the name of the player strategy
 */
char const *get_player_name()
{
	return "DansLaLegende";
}

/*
 * Initialize the player with informations given by the server if he is not already initialize :
 * - id : the id of the player : WHITE or BLACK
 * - graph : the graph to play with
 * - num_walls : the number of walls in the hand of the player
 */
void initialize(enum color_t id, struct graph_t *graph, size_t num_walls)
{
	static int is_initialized = 0;

	if (!is_initialized)
	{
		player.graph = graph_copy(graph);

		player.id = id;

		player.num_walls = num_walls;

		player.position[BLACK] = graph->num_vertices;
		player.position[WHITE] = graph->num_vertices;

		player.neighbours_graph = get_correlated_graph(graph);

		graph_free(graph);
	}
	else
		is_initialized = 1;
}

/*
 * Initialize and set a move
 * - position : the new position of the player
 * - edge1 : the first edge of the placed wall if a wall has been placed
 * - edge2 : the second edge of the wall
 * - id : the id of the player
 * - movetype : type of the move
 */
struct move_t set_move(size_t position, struct edge_t edge1, struct edge_t edge2, size_t id, enum movetype_t movetype)
{
	struct move_t move;
	move.m = position;
	move.e[0] = edge1;
	move.e[1] = edge2;
	move.c = id;
	move.t = movetype;

	return move;
}

/*
 * Return the first move for a player : the player is put on one of his own vertices
 */
struct move_t get_first_move()
{
	size_t starting_positions[player.graph->num_vertices];
	size_t nb_pos = 0;

	//searching for available positions
	for (size_t index = 0; index < player.graph->num_vertices; index++)
		if (is_owned(player.graph, player.id, index) == 1)
			starting_positions[nb_pos++] = index;

	//choice of an available position
	size_t random_index = nb_pos / 2;
	player.position[player.id] = starting_positions[random_index];

	//move to the position
	struct move_t first_move = set_move(player.position[player.id], no_edge(), no_edge(), player.id, MOVE);

	return first_move;
}


/*
 * Return true if the player is not placed yet.
 */
int is_first_move()
{
	return player.position[player.id] == player.graph->num_vertices;
}


/*
 * Free the player
 */
void finalize()
{
	free_correlation_graph(player.neighbours_graph);
	graph_free(player.graph);
}


/**
 * Get an allocated copy of a move 'a'
 * @param a Move to copy
 * @return An allocated copy of the move
 */
void* move_copy(const void* a)
{
	struct move_t* p = malloc(sizeof(struct move_t));
	*p = *((struct move_t*)a);
	return p;
}


/**
 * Release a move memory
 * @param a Move to free
 */
void free_move(void* a)
{
	free(a);
}


/**
 * Get an allocated copy of an int 'a'
 * @param a Move to copy
 * @return An allocated copy of the int
 */
void* int_copy(const void* a)
{
	int* p = malloc(sizeof(int));
	*p = *((int*)a);
	return p;
}


/**
 * Release an int memory
 * @param a Int to free
 */
void free_int(void* a)
{
	free(a);
}


/**
 * Get the minimal distance between two position (BFS)
 * @param graph Graph structure representing the game board and its information
 * @param initial_position Initial position to search the distance from
 * @param target_position Target position on the graph
 * @return The minimal distance between two position
 */
int get_distance_between_positions(struct graph_t* graph, size_t initial_position, size_t target_position)
{
	struct list* dist = list__empty(int_copy, free_int);

	for (size_t vertex = 0; vertex < graph->num_vertices; vertex++)
		list__add(dist, (void*)&INF);
	int dist_zero = 0;
	list__change(dist, initial_position, &dist_zero);

	struct queue* q = queue__empty(int_copy, free_int);
	queue__enqueue(q, &initial_position);

	while (!queue__is_empty(q))
	{
		int* u = (int*)queue__dequeue(q);

		if (*u == (int)target_position)
		{
			int distance_to_target = *((int*)list__get(dist, *u));
			list__free(dist);
			queue__free(q);
			free_int(u);
			return distance_to_target;
		}

		for (unsigned int v = 0; v < graph->num_vertices; v++)
			if (is_connected(graph, *u, v) && *((int*)list__get(dist, v)) == INF)
			{
				queue__enqueue(q, &v);
				int new_distance = *((int*)list__get(dist, *u)) + 1;
				list__change(dist, v, &new_distance);
			}

		free_int(u);
	}

	list__free(dist);
	queue__free(q);

	return INF;
}


/**
 * Get the minimal distance to the opponent area for a given player and a given graph
 * @param graph Graph structure representing the game board and its information
 * @param opponent Opponent player
 * @param player_position Position of the player on the board
 * @return The minimal distance to the opponent area
 */
int get_minimal_distance_to_opponent_area(struct graph_t* graph, int opponent, size_t player_position)
{
	int minimal_distance = UNINITIALIZED_MINIMAL_DISTANCE;
	for (size_t vertex = 0; vertex < graph->num_vertices; vertex++)
		if (is_owned(graph, opponent, vertex))
		{
			int distance = get_distance_between_positions(graph, player_position, vertex);
			if (minimal_distance == UNINITIALIZED_MINIMAL_DISTANCE || distance < minimal_distance)
				minimal_distance = distance;
		}

	return minimal_distance;
}


/**
 * Evaluate the player position in a given state of the game
 * @param graph Board to compute the score with
 * @param active_player Player to compute the score for
 * @param opponent Opponent player
 * @param player_position Position of the player on the board
 * @param opponent_position Position of the opponent player on the board
 * @return An integer corresponding to the score of the player for a given board
 */
int heuristic_evaluation(struct graph_t* graph, int active_player, int opponent, size_t active_player_position, size_t opponent_position)
{
	if (is_winning(graph, active_player, active_player_position))
		return 1000;
	if (is_winning(graph, opponent, opponent_position))
		return -1000;
	return (get_minimal_distance_to_opponent_area(graph, active_player, opponent_position) - get_minimal_distance_to_opponent_area(graph, opponent, active_player_position));
}


/**
 * Get a vertex according to the searched direction
 * @param graph Graph structure representing the game board and its information
 * @param from First vertex (Origin)
 * @param direction Direction to search
 * @return A vertex in the searched direction
 */
int get_vertex_by_connection_type(struct graph_t* graph, size_t from, int direction)
{
	for (size_t to = 0; to < graph->num_vertices; to++)
		if (get_connection_type(graph, from, to) == direction)
			return (int)to;

	return CONNECTION_NOT_FOUND;
}


/**
 * Complete the vertices to test around the position of the player
 * @param vertices_to_test Array of vertices to be tested
 * @param position Player's position
 */
void get_vertices_to_test(size_t vertices_to_test[], size_t position)
{
	vertices_to_test[0] = player.neighbours_graph[player.neighbours_graph[position].north].north;
	vertices_to_test[1] = player.neighbours_graph[position].north;
	vertices_to_test[2] = player.neighbours_graph[player.neighbours_graph[position].north].west;
	vertices_to_test[3] = player.neighbours_graph[player.neighbours_graph[position].north].east;
	vertices_to_test[4] = player.neighbours_graph[position].west;
	vertices_to_test[5] = player.neighbours_graph[position].east;
	vertices_to_test[6] = player.neighbours_graph[player.neighbours_graph[position].west].west;
	vertices_to_test[7] = player.neighbours_graph[player.neighbours_graph[position].east].east;
	vertices_to_test[8] = player.neighbours_graph[position].south;
	vertices_to_test[9] = player.neighbours_graph[player.neighbours_graph[position].south].west;
	vertices_to_test[10] = player.neighbours_graph[player.neighbours_graph[position].south].east;
	vertices_to_test[11] = player.neighbours_graph[player.neighbours_graph[position].south].south;
}


/**
 * Get an array of legal moves for a player
 * @param active_player Player for whom we want to know the possible legal moves
 * @param graph Board used during the AlphaBeta/Minimax algorithm.
 * @param active_player_position Position of the active player on the board
 * @param opponent_player_position Position of the opponent player on the board
 * @param Number of walls left
 * @return An list (of struct move_t) representing every legal moves for the player.
 */
struct list* get_legal_moves(int active_player, struct graph_t* graph, size_t active_player_position, size_t opponent_player_position, size_t num_walls)
{
	struct list* lst = list__empty(move_copy, free_move);

	size_t position_player1 = active_player == BLACK ? active_player_position : opponent_player_position;
	size_t position_player2 = active_player == WHITE ? active_player_position : opponent_player_position;
    size_t distance_opponent_to_win = get_minimal_distance_to_opponent_area(graph, active_player, opponent_player_position);

    size_t vertices_to_test_move[12] = {0};
	get_vertices_to_test(vertices_to_test_move, active_player_position);
	size_t vertices_to_test_wall[12] = {0};
	get_vertices_to_test(vertices_to_test_wall, opponent_player_position);
	vertices_to_test_wall[0] = opponent_player_position;

	for (size_t i = 0; i < 12; i++)
	{
		if (can_player_move_to(graph, vertices_to_test_move[i], active_player, position_player1, position_player2))
		{
			struct move_t move = set_move(vertices_to_test_move[i], no_edge(), no_edge(), active_player, MOVE);
			list__add(lst, &move);
		}

		// Add legal move of type WALL
		if (num_walls == 0 || distance_opponent_to_win > 6)
			continue;

		// Determine every vertices with which 'vertex' could be involved with
		if (!is_vertex_in_graph(graph, vertices_to_test_wall[i]))
			continue;

		int eastern_vertex = player.neighbours_graph[vertices_to_test_wall[i]].east;
		if (!is_connected(graph, vertices_to_test_wall[i], eastern_vertex))
			continue;

		int southern_vertex = player.neighbours_graph[vertices_to_test_wall[i]].south;
		if (!is_connected(graph, vertices_to_test_wall[i], southern_vertex))
			continue;

		int southeastern_vertex = player.neighbours_graph[southern_vertex].east;
		if (!is_connected(graph, southern_vertex, southeastern_vertex))
			continue;


		if (!(vertices_to_test_wall[i] == opponent_player_position
				|| eastern_vertex == (int)opponent_player_position
				|| southern_vertex == (int)opponent_player_position
				|| southeastern_vertex == (int)opponent_player_position))
			continue;

		struct edge_t e1[2] = {
				{ vertices_to_test_wall[i], eastern_vertex },
				{ southern_vertex, southeastern_vertex }
		};

		struct edge_t e2[2] = {
				{ vertices_to_test_wall[i], southern_vertex },
				{ eastern_vertex, southeastern_vertex }
		};

		if (can_add_wall(graph, e1, position_player1, position_player2))
		{
			struct move_t move = set_move(-1, e1[0], e1[1], active_player, WALL);
			list__add(lst, &move);
		}

		if (can_add_wall(graph, e2, position_player1, position_player2))
		{
			struct move_t move = set_move(-1, e2[0], e2[1], active_player, WALL);
			list__add(lst, &move);
		}
	}


	return lst;
}


/**
 * Get the maximum between a and b
 * @param a First number to compare
 * @param b Second number to compare
 * @return The maximum between a and b
 */
int max(int a, int b)
{
	return a > b ? a : b;
}


/**
 * Get the minimum between a and b
 * @param a First number to compare
 * @param b Second number to compare
 * @return The minimum between a and b
 */
int min(int a, int b)
{
	return a < b ? a : b;
}


/**
 * Get the updated graph after a move
 * @param graph Graph structure representing the game board and its information
 * @param move Move structure to play
 * @return A graph representing the board after the move
 */
struct graph_t* get_next_graph(struct graph_t* graph, struct move_t* move)
{
	struct graph_t* next_graph = graph_copy(graph);

	if (move->t == WALL)
		add_wall(next_graph, move->e);

	return next_graph;
}


/**
 * Get the new player position after a move
 * @param move Move structure to play
 * @param player Player id
 * @param player_position Position of the player on the board
 * @return The new position of the player
 */
size_t get_next_player_position(struct move_t* move, int player, size_t player_position)
{
	if (move->t == WALL)
		return player_position;

	if ((int)move->c == player)
		return move->m;
	else
		return player_position;
}


/**
 * Minimax w/ Alpha Beta Pruning
 * @param graph State of the board at a given node
 * @param active_player Player of the node in the tree
 * @param active_player_position Position of the active player on the board
 * @param active_player_walls Number of walls left for the active player
 * @param depth Depth of search according to the node
 * @param is_maximizing_player Boolean characterizing the min or max of active_player
 * @param opponent_player Opponent of the active player
 * @param opponent_player_position Position of the opponent player on the board
 * @param opponent_player_walls Number of walls left for the opponent player
 * @param alpha The highest utility value found so far
 * @param beta The smallest utility value found so far
 * @return Score of the active player given the state of the board
 */
int minimax_alphabeta(struct graph_t* graph, int active_player, size_t active_player_position, size_t active_player_walls,
        int depth, int is_maximizing_player, int opponent_player, size_t opponent_player_position, size_t opponent_player_walls,
		double alpha, double beta, time_t end_time)
{
	if (time(NULL) >= end_time || depth == 0 || is_winning(graph, active_player, active_player_position) || is_winning(graph, opponent_player, opponent_player_position))
        return heuristic_evaluation(graph, active_player, opponent_player, active_player_position, opponent_player_position);

	struct list* legal_moves = get_legal_moves(active_player, graph, active_player_position, opponent_player_position, active_player_walls);

	// There are always legal moves in Quoridor

	if (is_maximizing_player)
	{
		int best_score = -INF;

		for (size_t i = 0; i < list__size(legal_moves); i++)
		{
			struct move_t* move = list__get(legal_moves, i);
			struct graph_t* next_graph = get_next_graph(graph, move);
			active_player_position = get_next_player_position(move, active_player, active_player_position);
			opponent_player_position = get_next_player_position(move, opponent_player, opponent_player_position);
            active_player_walls -= move->t == WALL;
			best_score = max(best_score, minimax_alphabeta(next_graph, opponent_player, opponent_player_position, opponent_player_walls,
                                                  depth - 1, 0, active_player, active_player_position, active_player_walls,
                                                  alpha, beta, end_time));
			alpha = max(alpha, best_score);
			graph_free(next_graph);

			if (beta <= alpha)
				break; // Beta cutoff
		}

		list__free(legal_moves);
		return best_score;
	}
	else // Minimize
	{
		int best_score = INF;

		for (size_t i = 0; i < list__size(legal_moves); i++)
		{
			struct move_t* move = list__get(legal_moves, i);
			struct graph_t* next_graph = get_next_graph(graph, move);
			active_player_position = get_next_player_position(move, active_player, active_player_position);
			opponent_player_position = get_next_player_position(move, opponent_player, opponent_player_position);
            active_player_walls -= move->t == WALL;
			best_score = min(best_score, minimax_alphabeta(next_graph, opponent_player, opponent_player_position, opponent_player_walls,
                                                  depth - 1, 1, active_player, active_player_position, active_player_walls,
                                                  alpha, beta, end_time));
			beta = min(beta, best_score);
			graph_free(next_graph);
			if (beta <= alpha)
				break; // Alpha cutoff
		}

		list__free(legal_moves);
		return best_score;
	}

}


/**
 * Negamax w/ Alpha Beta Pruning
 * @param graph State of the board at a given node
 * @param active_player Player of the node in the tree
 * @param active_player_position Position of the active player on the board
 * @param active_player_walls Number of walls left for the active player
 * @param depth Depth of search according to the node
 * @param opponent_player Opponent of the active player
 * @param opponent_player_position Position of the opponent player on the board
 * @param opponent_player_walls Number of walls left for the opponent player
 * @param alpha The highest utility value found so far
 * @param beta The smallest utility value found so far
 * @return Score of the active player given the state of the board
 */
int negamax_alphabeta(struct graph_t* graph, int active_player, size_t active_player_position, size_t active_player_walls,
        int depth, int opponent_player, size_t opponent_player_position, size_t opponent_player_walls,
		double alpha, double beta, time_t end_time)
{
	if (time(NULL) >= end_time || depth == 0 || is_winning(graph, active_player, active_player_position) || is_winning(graph, opponent_player, opponent_player_position))
		return heuristic_evaluation(graph, active_player, opponent_player, active_player_position, opponent_player_position);

	int best_score = -INF;

	struct list* legal_moves = get_legal_moves(active_player, graph, active_player_position, opponent_player_position, active_player_walls);

	for (size_t i = 0; i < list__size(legal_moves); i++)
	{
		struct move_t* move = list__get(legal_moves, i);
		struct graph_t* next_graph = get_next_graph(graph, move);
		active_player_position = get_next_player_position(move, active_player, active_player_position);
		opponent_player_position = get_next_player_position(move, opponent_player, opponent_player_position);
        active_player_walls -= move->t == WALL;
		best_score = max(best_score, -negamax_alphabeta(next_graph, opponent_player, opponent_player_position, opponent_player_walls,
                                                  depth - 1, active_player, active_player_position, active_player_walls,
                                                  -beta, -alpha, end_time));
		graph_free(next_graph);

		if (best_score > alpha)
			alpha = best_score;
		if (best_score >= beta)
			break;
	}

	list__free(legal_moves);

	return alpha;
}


/**
 * Determine a move to do for the player
 * @return A move structure to play
 */
struct move_t get_new_move()
{
	int best_score = -INF;
	struct move_t best_move;
	struct list* legal_moves = get_legal_moves(player.id, player.graph, player.position[player.id], player.position[get_next_player(player.id)], player.num_walls);
    int depth = 2;
	//printf("\n\nNumber of legal moves: [%zd]\t\n", list__size(legal_moves));
	//printf("Depth: %d\n", depth);

	time_t end_time = time(NULL) + 3;
	for (size_t i = 0; i < list__size(legal_moves); i++)
	{
		struct move_t* move = list__get(legal_moves, i);
		struct graph_t* next_graph = get_next_graph(player.graph, move);
		size_t active_player_position = get_next_player_position(move, player.id, player.position[player.id]);
		size_t active_player_walls = player.num_walls - (move->t == WALL);
		size_t opponent_player_position = get_next_player_position(move, get_next_player(player.id), player.position[get_next_player(player.id)]);
		size_t opponent_player_walls = 0;

		if (is_winning(next_graph, player.id, active_player_position))
        {
            best_move = *move;
            graph_free(next_graph);
            break;
        }

		/*int score = minimax_alphabeta(next_graph, get_next_player(player.id), opponent_player_position, opponent_player_walls,
                                depth - 1, 0, player.id, active_player_position, active_player_walls,
                                best_score, INF, end_time);*/
		int score = -negamax_alphabeta(next_graph, get_next_player(player.id), opponent_player_position, opponent_player_walls,
                                 depth - 1, player.id, active_player_position, active_player_walls,
                                 -INF, -best_score, end_time);

		if (score > best_score)
		{
			best_score = score;
			best_move = *move;
		}

		graph_free(next_graph);
	}

	struct move_t new_move = set_move(best_move.m, best_move.e[0], best_move.e[1], best_move.c, best_move.t);

	list__free(legal_moves);

	return new_move;
}


/**
 * Update the graph of the player with the move
 * @param move Move done
 */
void update(struct move_t move)
{
	if (move.t == MOVE)
		player.position[move.c] = move.m;
	else if (move.t == WALL)
    {
	    add_wall(player.graph, move.e);
        if (move.c == player.id) player.num_walls -= 1;
    }
}


/**
 * Return the move of the active player
 * @param previous_move Previous move done
 * @return
 */
struct move_t play(struct move_t previous_move)
{
	if (is_first_move())
		return get_first_move();
	update(previous_move);

	struct move_t move = get_new_move();
	update(move);

	return move;
}

