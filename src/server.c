#include <time.h>

#include "server.h"
#include "utils.h"
#include "display.h"

#define TURN_MAX 200

/**
 * Calculate the wall number from edge number of the graph :
 * @param width : graph width
 * @param type : graph type
 * @return the number of walls authorized
 */
size_t get_number_of_walls(size_t width, char type)
{
    switch (type)
    {
    case 'c':
        return 2 * (width * width - width) / 15;
    case 't':
        return (16/9 * width * width - 8/3 * width) / 15;
    case 'h':
        return (14/9 * width * width - 8/3 * width) / 15;
    case 's':
        return (26/25 * width * width - 14/5 * width) / 15;
    default:
        return 0;
    }
}

/**
 * Initializes the server graph :
 * @param width : graph width
 * @param type : graph type
 * @param graph : the server graph unitialized
 */
void initialize_graph(size_t width, char type, struct graph_server *graph)
{
    graph->type = type;
    graph->width = width;
    graph->num_wall = get_number_of_walls(width, type);
    graph->graph = get_graph(type, width);
}

/**
 * Load protocol player functions of one player  :
 * @param player : server player uninitialized
 */
void *load_player(struct player_server *player)
{
    static const char *function_name[] = {
        "get_player_name", "initialize", "play", "finalize"};

    if (!(player->get_player_name = load_function(player->lib, function_name[0])))
        return NULL;
    if (!(player->initialize = load_function(player->lib, function_name[1])))
        return NULL;
    if (!(player->play = load_function(player->lib, function_name[2])))
        return NULL;
    if (!(player->finalize = load_function(player->lib, function_name[3])))
        return NULL;
    return player;
}

/**
 * Load protocol player functions of the two players of the game  :
 * @param players : server players unititialized
 * @param path_lib_player1 : path to the first player librairy function
 * @param path_lib_player2 : path to the second player librairy function
 */
void load_players(struct player_server *players, char *path_lib_player1, char *path_lib_player2)
{
    if (!(players[BLACK].lib = load_library(path_lib_player1)) || !(players[WHITE].lib = load_library(path_lib_player2)))
    {
        fprintf(stderr, "Error when loading player's library : %s\n", dlerror());
        exit(1);
    }

    if (!load_player(&players[BLACK]) || !load_player(&players[WHITE]))
    {
        fprintf(stderr, "Error when loading player's functions : %s\n", dlerror());
        exit(1);
    }
}

/**
 * Create a initial move for the fisrt turn, its type is NO_TYPE  :
 * @return the initial move
 */
struct move_t get_initial_move()
{
    struct move_t mv;
    mv.m = 0;
    mv.t = NO_TYPE;
    mv.c = BLACK;
    return mv;
}

/**
 * Updates the server graph after a player move :
 * @param server : the server structure
 * @param move : the player move
 */
void update(struct server * server, struct move_t move)//struct graph_t *graph, struct move_t move, size_t *p_position, size_t *p_num_wall
{
    if (move.t == MOVE)
        server->players[move.c].pos = move.m;
    else {
        server->players[move.c].num_wall -= 1;
        add_wall(server->graph.graph, move.e);
    }
}

/**
 * Verifies if the first placement is right for a player, if is right the function updates the server graph :
 * @param server : the server structure
 * @param move : the initial move of server or the placement of the other player
 * @param id : the player id
 * @return the player placement
 */
struct move_t player_placement(struct server *server, struct move_t move, enum color_t id)
{
    move = server->players[id].play(move);

    if (is_owned(server->graph.graph, id, move.m))
      update(server, move);//server->graph.graph, move, &server->players[id].pos, &server->players[id].num_wall);
    else {
        display_error_move("wrong first placement", server->players[id].get_player_name(), get_name_type_player(id));
        display_move(server, move);
        display_winner(server, 0, get_next_player(id));
        free_server(server);
        exit(1);
    }
    return move;
}

/**
 * Verifies if the player move is right, if is right the function updates the server graph :
 * @param server : the server structure
 * @param move : the initial move of server or the placement of the other player
 * @param id : the player id
 * @param cheat : the cheat variable
 * @return the new move
 */
struct move_t play_player_turn(struct server *server, struct move_t move, enum color_t id, size_t *cheat)
{
    move = server->players[get_next_player(move.c)].play(move);

    if (is_move_legal(server->graph.graph, &move, server->players[BLACK].pos, server->players[WHITE].pos, server->players[id].num_wall)) {
      update(server, move);//server->graph.graph, move, &server->players[id].pos, &server->players[id].num_wall);
        *cheat = 0;
    }
    else {
        display_error_move(move.t == MOVE ? "wrong MOVE" : "wrong WALL move" , server->players[id].get_player_name(), get_name_type_player(id));
        display_move(server, move);
        *cheat = 1;
    }
    return move;
}

/**
 * Give the wall number authorized, a game graph copy and an id for initialize a player  :
 * @param id : player id
 * @param server : the server structure
 */
void initialize_player(enum color_t id, struct server *server)
{
    server->players[id].num_wall = server->graph.num_wall;
    struct graph_t *copy_graph = graph_copy(server->graph.graph);
    server->players[id].initialize(id, copy_graph, server->graph.num_wall);
}


struct server *initialize_server(char *player1_lib, char *player2_lib, size_t width, char type)
{
    struct server *server = malloc(sizeof(struct server));

    initialize_graph(width, type, &server->graph);
    load_players(server->players, player1_lib, player2_lib);

    return server;
}

void run_server(struct server *server, int print, int delay)
{
    size_t turn = 0;
    size_t has_cheat = 0;

    time_t start = time(NULL);

    // Initializing players
    initialize_player(BLACK, server);
    initialize_player(WHITE, server);

    // Players choose their first position in the board
    // exit if it is not a valid position for them to start
    struct move_t move = get_initial_move();
    move = player_placement(server, move, BLACK);
    move = player_placement(server, move, WHITE);

    if (print) {
        printf("Black position: %zd\n", server->players[BLACK].pos);
        printf("White position: %zd\n", server->players[WHITE].pos);
    }

    // Play until one player has cheat
    // or is winning
    // or the number maximum of turn is attained
    do
    {
        if (print) {
            display_game(server, turn, move.c);
            if (turn > 0) {
				//display_move(server, move);
                printf("Black position: %zd\n", server->players[BLACK].pos);
                printf("White position: %zd\n", server->players[WHITE].pos);
            }
        }
        if (delay)
            sleep(delay);
        turn++;
        move = play_player_turn(server, move, get_next_player(move.c), &has_cheat);
    } while (!has_cheat && turn < TURN_MAX && !is_winning(server->graph.graph, move.c, server->players[move.c].pos));


    // Display winner(s) of the game
    if (print) {
        if (turn == TURN_MAX)
            display_winner(server, turn, BLACK);
        else if (!has_cheat)
            display_winner(server, turn, move.c);
        else
            display_winner(server, turn, get_next_player(move.c));
    }

    printf("\nTime elapsed: %ld seconds.\n\n", time(NULL) - start);

}

void free_server(struct server *server)
{
    server->players[BLACK].finalize();
    server->players[WHITE].finalize();

    dlclose(server->players[BLACK].lib);
    dlclose(server->players[WHITE].lib);

    graph_free(server->graph.graph);

    free(server);
}
