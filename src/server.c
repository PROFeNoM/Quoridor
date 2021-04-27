#include "server.h"
#include "utils.h"
#include "display.h"

#define TURN_MAX 100

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

void initialize_graph(size_t width, char type, struct graph_server *graph)
{
    graph->type = type;
    graph->width = width;
    graph->num_wall = get_number_of_walls(width, type);
    graph->graph = get_graph(type, width);
}

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

struct server *initialize_server(char *player1_lib, char *player2_lib, size_t width, char type)
{
    struct server *server = malloc(sizeof(struct server));

    initialize_graph(width, type, &server->graph);
    load_players(server->players, player1_lib, player2_lib);

    return server;
}

struct move_t get_initial_move()
{
    struct move_t mv;
    mv.m = 0;
    mv.t = NO_TYPE;
    mv.c = BLACK;
    return mv;
}

void update(struct graph_t *graph, struct move_t move, size_t *p_position, size_t *p_num_wall)
{
    if (move.t == MOVE)
        *p_position = move.m;
    else {
        *p_num_wall -= 1;
        add_wall(graph, move.e);
    }
}

struct move_t player_placement(struct server *server, struct move_t move, enum color_t id)
{
    move = server->players[id].play(move);

    if (is_owned(server->graph.graph, id, move.m))
        update(server->graph.graph, move, &server->players[id].pos, &server->players[id].num_wall);
    else {
        display_error_move("wrong first placement", server->players[id].get_player_name(), get_name_type_player(id));
        display_move(server, move);
        display_winner(server, 0, get_next_player(id));
        free_server(server);
        exit(1);
    }
    return move;
}

struct move_t play_player_turn(struct server *server, struct move_t move, enum color_t id, size_t *cheat)
{
    move = server->players[get_next_player(move.c)].play(move);

    if (is_move_legal(server->graph.graph, &move, server->players[BLACK].pos, server->players[WHITE].pos)) {
    	update(server->graph.graph, move, &server->players[id].pos, &server->players[id].num_wall);
        *cheat = 0;
    }
    else {
        display_error_move(move.t == MOVE ? "wrong MOVE" : "wrong WALL move" , server->players[id].get_player_name(), get_name_type_player(id));
        display_move(server, move);
        *cheat = 1;
    }
    return move;
}

void initialize_player(enum color_t id, struct server *server)
{
    server->players[id].num_wall = server->graph.num_wall;
    struct graph_t *copy_graph = graph_copy(server->graph.graph);
    server->players[id].initialize(id, copy_graph, server->graph.num_wall);
}

void run_server(struct server *server, int print, int delay)
{
    size_t turn = 0;
    size_t has_cheat = 0;

    // Initializing players
    initialize_player(BLACK, server);
    initialize_player(WHITE, server);

    // Players choose their first position in the board
    // exit if it is not a valid position for them to start
    struct move_t move = get_initial_move();
    move = player_placement(server, move, BLACK);
    move = player_placement(server, move, WHITE);

	printf("Black position: %zd\n", server->players[BLACK].pos);
	printf("White position: %zd\n", server->players[WHITE].pos);

    // Play until one player has cheat
    // or is winning
    // or the number maximum of turn is attained
    do
    {
    	//printf("%s to play\n", get_next_player(move.c) == BLACK ? "Black" : "White");
        if (print) {
            display_game(server, turn, move.c);
            if (turn > 0) {
				//display_move(server, move);
                printf("Black position: %zd\n", server->players[BLACK].pos);
                printf("White position: %zd\n", server->players[WHITE].pos);
            }
            sleep(delay);
        }
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
