#include "server.h"
#include "utils.h"

void initialize_graph(size_t width, char type, struct graph_server *graph)
{
    graph->type = type;
    graph->width = width;

    gsl_spmatrix *matrix = square_graph(width);
    gsl_spmatrix *matrix_pos = matrix_position(width);
    graph->graph = malloc(sizeof(struct graph_t));
    graph->graph->num_vertices = width * width;
    graph->graph->t = matrix;
    graph->graph->o = matrix_pos;
}

void update(struct player_server *players, struct move_t move)
{
    players[move.c].pos = move.m;
}

void display_graph(struct graph_t *graph, size_t m, struct player_server *players)
{
    size_t player_one_pos = players[WHITE].pos;
    size_t player_two_pos = players[BLACK].pos;
    char *to_print = "";
    for (size_t i = 0; i < m; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            size_t vertex = i * m + j;
            to_print = (vertex == player_one_pos) ? "1" : 
            (vertex == player_two_pos ? "2" : 
            (gsl_spmatrix_get(graph->o, BLACK, vertex) == 1 ? "◻" : 
            (gsl_spmatrix_get(graph->o, WHITE, vertex) == 1 ? "◼" : 
            "⬚")));
            printf(" %s ", to_print);
        }
        printf("\n");
    }
    printf("\n");
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
        fprintf(stderr, "Error load player library : %s\n", dlerror());
        exit(1);
    }

    if (!load_player(&players[BLACK]) || !load_player(&players[WHITE]))
    {
        fprintf(stderr, "Error load player function : %s\n", dlerror());
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

void run_server(struct server *server)
{
    server->players[BLACK].initialize(BLACK, graph_copy(server->graph.graph), 22);
    server->players[WHITE].initialize(WHITE, graph_copy(server->graph.graph), 22);

    struct move_t move = {.m = 0, .e = {{-1, -1}}, .t = NO_TYPE, .c = WHITE};
    while (1)
    {
        move = server->players[get_next_player(move.c)].play(move);
        update(server->players, move);
        display_graph(server->graph.graph, server->graph.width, server->players);
        if (is_winning(server->graph.graph, move.c, server->players[move.c].pos)){
            break;
        }
    }
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