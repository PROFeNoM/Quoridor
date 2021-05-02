#include "display.h"

// Move cursor to the (x, y) position in the console
#define GOTO(X, Y) printf("\x1B[%d;%dH", Y, X)

// Color
#define BLACK_COLOR 0
#define GREY_COLOR 100
#define WHITE_COLOR 7

// Clear the console
#define clear() \
    system("clear");

// Set front color
#define set_front_color(color) printf("\033[3%dm", (color))
// Set background color
#define set_back_color(color) printf("\e[%dm", (color))
// Reset the console to default color
#define set_default() printf("\033[m")
// Reset the console to default color
#define set_default_foreground() printf("\033[39m")

/**
 * Width of a graph
 */
static size_t graph_width = 0;

/**
 * Predicate to know if a vertex has at least one connection
 * @param graph Graph representing the board
 * @param vertex Vertex to consider
 */
int has_more_than_zero_connection(struct graph_t *graph, size_t vertex)
{
    return (vertex < graph->num_vertices-1 && gsl_spmatrix_uint_get(graph->t, vertex, vertex + 1) != NOT_CONNECTED)
    || (vertex > 0 && gsl_spmatrix_uint_get(graph->t, vertex, vertex - 1) != NOT_CONNECTED)
    || (vertex < graph->num_vertices - graph_width && gsl_spmatrix_uint_get(graph->t, vertex, vertex + graph_width) != NOT_CONNECTED)
    || (vertex >= graph_width && gsl_spmatrix_uint_get(graph->t, vertex, vertex - graph_width) != NOT_CONNECTED);
}

/**
 * Display the right link of a vertex if there is one
 * @param from Vertex right to consider
 * @param to Vertex left to consider
 * @param graph Graph representing the board
 */
void display_right_link(size_t from, size_t to, struct graph_t *graph)
{
    if (from < graph->num_vertices - 1 && is_connected(graph, from, to))
        printf("-");
    else
        printf(" ");
}

/**
 * Display the left link of a vertex if there is one
 * @param from Vertex left to consider
 * @param to Vertex right to consider
 * @param graph Graph representing the board
 */
void display_left_link(size_t from, size_t to, struct graph_t *graph)
{
    if (from > 0 && is_connected(graph, from, to))
        printf("-");
    else
        printf(" ");
}

/**
 * Shift display by size `spaces`
 * @param size shift to do
 */
void print_shift(int size)
{
    printf("%*s", size, "");
}

/**
 * Display a new line
 */
void print_new_line()
{
    printf("\n");
}

/**
 * Display the bottom link for all vertex at `y` line position in the graph
 * @param graph Graph representing the board
 * @param y Line of the graph to consider in order to look for bottom link
 * @param shift Shift before displaying bottom link of the graph line
 * @param size_display Size allowed to display and to fill if necessary
 */
void display_bottom_link(struct graph_t *graph, size_t y, size_t shift, size_t size_display)
{
    char *bottom_link = "";
    for (size_t x = 0; x < graph_width && y < graph_width - 1; x++)
    {
        size_t vertex = y * graph_width + x;
        bottom_link = is_connected(graph, vertex, vertex + graph_width) ? "|" : " ";
        printf(" %s ", bottom_link);
    }

    if (y >= graph_width - 1)
        print_shift(size_display - shift);
    else
        print_shift(shift);
}

/**
 * Display a cell depending on predicate:
 * if it is a player position then display him,
 * else if it is a vertex owned by a player then display it with the color of the player,
 * else if it is an vertex then display it,
 * else display `space`
 * @param vertex Position of the cell to consider
 * @param graph Graph representing the board
 * @param player_one_pos Position of player one
 * @param player_two_pos Position of player two
 */
void display_cell(size_t vertex, struct graph_t *graph, size_t player_one_pos, size_t player_two_pos)
{
    if (vertex == player_one_pos)
    {
        set_front_color(WHITE_COLOR);
        printf("♖");
        set_default_foreground();
    }
    else if (vertex == player_two_pos)
    {
        set_front_color(BLACK_COLOR);
        printf("♖");
        set_default_foreground();
    }
    else if (gsl_spmatrix_uint_get(graph->o, BLACK, vertex) == 1)
    {
        set_front_color(BLACK_COLOR);
        printf("■");
        set_default_foreground();
    }
    else if ((gsl_spmatrix_uint_get(graph->o, WHITE, vertex) == 1))
    {
        set_front_color(WHITE_COLOR);
        printf("■");
        set_default_foreground();
    }
    else if (has_more_than_zero_connection(graph, vertex))
    {
        printf("⬚");
    }
    else
    {
        printf(" ");
    }
}

/**
 * Display the board of a game
 * @param graph Graph representing the board
 * @param size_display Size allowed to display and to fill if necessary
 * @param p1_pos Position of player one
 * @param p2_pos Position of player two
 */
void display_board(struct graph_t *graph, size_t size_display, size_t p1_pos, size_t p2_pos)
{
    graph_width = (size_t)sqrt(graph->num_vertices);
    size_t shift_print = ((size_display + 1) / 2) - ((3 * graph_width+1) / 2); 

    set_back_color(GREY_COLOR);
    print_shift(size_display);
    print_new_line();

    for (size_t y = 0; y < graph_width; y++)
    {
        print_shift(shift_print);
        for (size_t x = 0; x < graph_width; x++)
        {
            size_t vertex = y * graph_width + x;
            display_left_link(vertex, vertex - 1, graph);
            display_cell(vertex, graph, p1_pos, p2_pos);
            display_right_link(vertex, vertex + 1, graph);
        }

        print_shift(shift_print);
        print_new_line();
        
        print_shift(shift_print);
        display_bottom_link(graph, y, shift_print, size_display);
        print_new_line();
    }
    set_default();
}

/**
 * Display an horizontal separation
 * @param size Size of the horizontal separation wanted
 */
void display_border(size_t size)
{
    char border[size + 1];
    memset(border, '=', size);
    border[size] = '\0';

    printf("%s\n", border);
}

/**
 * Display header informations
 * (e.g to displaying current player's name and turn number)
 * @param size_display Size allowed to display and to fill if necessary
 * @param turn Current number of turn
 * @param current_player_name Current player's name
 */
void display_header(size_t size_display, size_t turn, char const *current_player_name)
{
    printf("\n%*s-- Turn %3zu --\n", (int)(size_display)/ 2 - 7, "", turn);
    printf("%*s%-15.15s\n\n", (int)((size_display + 1) / 2 - (strlen(current_player_name) + 1) / 2), "", current_player_name);

    display_border(size_display);
}

/**
 * Display footer informations
 * (e.g to displaying a separation of size size)
 * @param size 
 */
void display_footer(size_t size)
{
    display_border(size);
}

void display_game(struct server *server, size_t turn, enum color_t current_player)
{
    size_t size_display = 21 + 3 * (server->graph.width-1);
    char const *current_player_name = server->players[current_player].get_player_name();

    if (turn == 0) {
        clear();
    }

    GOTO(0, 0);
    display_header(size_display, turn, current_player_name);
    display_board(server->graph.graph, size_display, server->players[WHITE].pos, server->players[BLACK].pos);
    display_footer(size_display);
}

void display_move(struct server *server, struct move_t move)
{
    printf("Move of player %s (%s)\n", server->players[move.c].get_player_name(), get_name_type_player(move.c));
    if (move.t == MOVE)
    {
        printf("Move type : MOVE\n");
        printf("Moved from %3zu to %3zu     \n", server->players[move.c].pos, move.m);
        printf("                             \n");
    }
    else
    {
        printf("Move type : WALL\n");
        printf("First edge from %3zu to %3zu\n", move.e[0].fr, move.e[0].to);
        printf("Second edge from %3zu to %3zu\n", move.e[1].fr, move.e[1].to);
    }
}

void display_error_move(const char *error_type, const char *p_name, const char *p_type)
{
    printf("Illegal move (%s) of player %s (%s)\n", error_type, p_name, p_type);
}

void display_winner(struct server *server, size_t turn, enum color_t winner)
{
    display_game(server, turn, winner);
    if (winner == NO_COLOR)
        printf("No winner, this time is a draw...\n");
    else
        printf("The winner is %s whose name is %s\n", get_name_type_player(winner), server->players[winner].get_player_name());
}

/**
 *  @todo : Remove
 */
void display_graph_value(struct graph_t *graph)
{
    printf("      ");
    for (size_t i = 0; i < graph->num_vertices; i++)
    {
        printf("%2zu | ", i);
    }
    printf("\n");
    for (size_t i = 0; i < graph->num_vertices+1; i++)
    {
        printf("-----");
    }
    printf("\n");
    for (size_t i = 0; i < graph->num_vertices; i++)
    {
        printf("%3zu : ", i);
        for (size_t j = 0; j < graph->num_vertices; j++)
        {
            printf("%2d | ", get_connection_type(graph, i, j));
        }
        printf("\n");
        for (size_t i = 0; i < graph->num_vertices + 1; i++)
        {
            printf("-----");
        }
        printf("\n");
    }
}