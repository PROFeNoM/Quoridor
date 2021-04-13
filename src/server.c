#include "server.h"
#include "utils.h"
#include "display.h"

#define TURN_MAX 100

void initialize_graph(size_t width, char type, struct graph_server *graph)
{
    graph->type = type;
    graph->width = width;

    graph->graph = get_graph(type, width);
}

void update(struct player_server *players, struct move_t move)
{
    players[move.c].pos = move.m;
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

int initialize_players_position(struct server *server, struct move_t move, int print)
{
    move = server->players[get_next_player(move.c)].play(move);
    if (is_owned(server->graph.graph, WHITE, move.m))
        update(server->players, move);
    else
        return 1; // Illegal move
    if (print)
        display_game(server, 0, move.c);

    move = server->players[get_next_player(move.c)].play(move);
    if (is_owned(server->graph.graph, BLACK, move.m))
        update(server->players, move);
    else
        return 1; // Illegal move
    if (print)
        display_game(server, 0, move.c);

    return 0;
}

void run_server(struct server *server, int print)
{
    size_t turn = 1;
    struct graph_t *copy_graph = graph_copy(server->graph.graph);
    server->players[BLACK].initialize(BLACK, copy_graph, 22);
    graph_free(copy_graph);
    
    copy_graph = graph_copy(server->graph.graph);
    server->players[WHITE].initialize(WHITE, copy_graph, 22);
    graph_free(copy_graph);

    struct move_t move = get_initial_move();

    if (initialize_players_position(server, move, print))
    {
        printf("Illegal move during initialization of players positions.\n");
        free_server(server);
        return;
    }

    do
    {
        move = server->players[get_next_player(move.c)].play(move);
        if (is_move_legal(server->graph.graph, &move, server->players[0].pos, server->players[1].pos))
            update(server->players, move);
        else
        {
            printf("Illegal move by player %d\n", move.c);
            break;
        }

        //updated_display(server, turn, move.c);
        if (print)
            display_game(server, turn, move.c);
        turn++;
    } while (!is_winning(server->graph.graph, move.c, server->players[move.c].pos) && turn < TURN_MAX);

    free_server(server);
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
