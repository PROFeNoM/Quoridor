#include "display.h"

// Move cursor to the (x, y) position in the console
#define GOTO(X, Y) printf("\x1B[%d;%dH", Y, X)

// Font attributs
#define BOLD 1
#define DIM 2
#define UNDERLINE 4
#define FLASH 5
#define SURLINE 7

// Color
#define BLACK_COLOR 100
#define RED_COLOR 1
#define GREEN_COLOR 2
#define YELLOW_COLOR 3
#define BLUE_COLOR 4
#define MAGENTA_COLOR 5
#define CYAN_COLOR 6
#define WHITE_COLOR 7

// Clear the console
#define clear()                  \
    printf("\033[%d;%dH", 1, 1); \
    printf("\033[2J")

// Set front color
#define set_front_color(color) printf("\033[3%dm", (color))
// Set background color
#define set_back_color(color) printf("\e[%dm", (color))
// Set front and background color
#define set_color(front, back) printf("\033[3%d;4%dm", (front), (back))
// Set attributs of the font
#define set_attribut(attribut) printf("\033[%dm", (attribut));
// Reset the console to default color
#define set_default() printf("\033[m")
// Reset the console to default color
#define set_default_foreground() printf("\033[39m")
#define get_default_foreground "\033[39m"

int is_valid_graph_position(struct graph_t *graph, size_t vertex)
{
    return gsl_spmatrix_uint_get(graph->t, vertex, 0) != NOT_CONNECTED;
}

void display_right_link(size_t from, size_t to, struct graph_t *graph)
{
    if (from < graph->num_vertices - 1 && is_connected(graph, from, to))
        printf("-");
    else
        printf(" ");
}

void display_left_link(size_t from, size_t to, struct graph_t *graph)
{
    if (from > 0 && is_connected(graph, from, to))
        printf("-");
    else
        printf(" ");
}

void print_shift(int size)
{
    printf("%*s", size, "");
}

void print_new_line()
{
    printf("\n");
}

void display_bottom_link(struct graph_t *graph, size_t y, size_t graph_width, size_t shift, size_t size_display)
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
        set_front_color(0);
        printf("♖");
        set_default_foreground();
    }
    else if (gsl_spmatrix_uint_get(graph->o, BLACK, vertex) == 1)
    {
        set_front_color(0);
        printf("■");
        set_default_foreground();
    }
    else if ((gsl_spmatrix_uint_get(graph->o, WHITE, vertex) == 1))
    {
        set_front_color(WHITE_COLOR);
        printf("■");
        set_default_foreground();
    }
    else if (is_valid_graph_position(graph, vertex))
    {
        printf("⬚");
    }
    else
    {
        printf(" ");
    }
}

void display_board(struct graph_t *graph, size_t size_display, size_t p1_pos, size_t p2_pos)
{
    size_t graph_width = (size_t)sqrt(graph->num_vertices);
    size_t shift_print = ((size_display + 1) / 2) - ((3 * graph_width+1) / 2); 

    set_back_color(BLACK_COLOR);
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
        display_bottom_link(graph, y, graph_width, shift_print, size_display);
        print_new_line();
    }
    set_default();
}

void display_border(size_t size)
{
    char border[size + 1];
    memset(border, '=', size);
    border[size] = '\0';

    printf("%s\n", border);
}

void display_header(size_t size_display, size_t turn, char const *current_player_name)
{
    printf("\n%*s-- Turn %3zu --\n", (int)(size_display)/ 2 - 7, "", turn);
    printf("%*s%-15.15s\n\n", (int)((size_display + 1) / 2 - (strlen(current_player_name) + 1) / 2), "", current_player_name);

    display_border(size_display);
}

void display_footer(size_t size)
{
    display_border(size);
}

void display_game(struct server *server, size_t turn, size_t current_player)
{
    size_t size_display = 21 + 3 * (server->graph.width-1);
    char const *current_player_name = server->players[current_player].get_player_name();

    if (turn == 1) {
        clear();
    }

    GOTO(0, 0);
    display_header(size_display, turn, current_player_name);
    display_board(server->graph.graph, size_display, server->players[WHITE].pos, server->players[BLACK].pos);
    display_footer(size_display);

    sleep(1);
}

// "╔═══╗╔═══╗╔═══╗╔═══╗\n"
// "║   ║║   ║║   ║║   ║"
// "╚═══╝╚═══╝╚═══╝╚═══╝";
// "╔═══╗╔═══╗╔═══╗╔═══╗\n"
// "║   ║║   ║║   ║║ 2 ║"
// "╚═══╝╚═══╝╚═══╝╚═══╝";
// "╔═══╗╔═══╗╔═══╗╔═══╗\n"
// "║   ║║   ║║ 1 ║║   ║"
// "╚═══╝╚═══╝╚═══╝╚═══╝";
void updated_display_board(struct graph_t *graph, size_t size_display, size_t p1_pos, size_t p2_pos)
{
    size_t graph_width = (size_t)sqrt(graph->num_vertices);
    size_t shift_print = (size_display + 1) / 2 - 4 * ((graph_width + 1) / 2);

    char *top_side_cell = "";
    char *bottom_side_cell = "";
    char *left_side_cell = "";
    char *middle_side_cell = "";
    char *right_side_cell = "";

    set_back_color(BLACK_COLOR);
    print_shift(size_display);

    printf("\n");
    for (size_t y = 0; y < graph_width; y++)
    {
        for (size_t d = 0; d < 3; d++)
        {
            print_shift(shift_print);
            for (size_t x = 0; x < graph_width; x++)
            {
                size_t vertex = y*graph_width + x;
                size_t is_valid_position = is_valid_graph_position(graph, vertex);

                if (d == 0) {
                    if (is_valid_position) {
                        if (y > 0 && is_connected(graph, vertex, vertex - graph_width))
                            top_side_cell = "╔   ╗";
                        else
                            top_side_cell = "╔═══╗";
                    } else
                        top_side_cell = "     ";
                    printf("%s", top_side_cell);
                } else if (d == 1) {
                    if (is_valid_position) {
                        if (x > 0 && is_connected(graph, vertex, vertex - 1))
                            left_side_cell = " ";
                        else
                            left_side_cell = "║";

                        if (vertex == p1_pos)
                            middle_side_cell = "\033[37m♖\033[39m";
                        else if (vertex == p2_pos)
                            middle_side_cell = "\033[30m♖\033[39m";
                        else if (gsl_spmatrix_uint_get(graph->o, WHITE, vertex) == 1) {
                            //set_front_color(WHITE_COLOR);
                            middle_side_cell = "\033[37m■\033[39m";
                            //set_default_foreground();
                        } else if (gsl_spmatrix_uint_get(graph->o, BLACK, vertex) == 1) {
                            //set_front_color(0);
                            middle_side_cell = "\033[30m■\033[39m";
                            //set_default_foreground();
                        } else
                            middle_side_cell = " ";
                        
                        if (x < graph_width - 1 && is_connected(graph, vertex, vertex + 1))
                            right_side_cell = " ";
                        else
                            right_side_cell = "║";
                    } else {
                        left_side_cell = " ";
                        right_side_cell = " ";
                        middle_side_cell = " ";
                    }
                    printf("%s %s %s", left_side_cell, middle_side_cell, right_side_cell);
                } else {
                    if (is_valid_position) {
                        if (y < graph_width - 1 && is_connected(graph, vertex, vertex + graph_width))
                            bottom_side_cell = "╚   ╝";
                        else
                            bottom_side_cell = "╚═══╝";
                    } else
                        bottom_side_cell = "     ";
                    printf("%s", bottom_side_cell);
                }
            }
            print_shift(shift_print);
            printf("\n");
        }
    }
    print_shift(size_display);
    printf("\n");

    set_default();
}



void updated_display(struct server *server, size_t turn, size_t current_player)
{
    size_t graph_width = server->graph.width;
    size_t size_display = 20 + 5 * graph_width;
    char const *current_player_name = server->players[current_player].get_player_name();

    if (turn == 1) {
        clear();
    }

    GOTO(0, 0);
    display_header(size_display, turn, current_player_name);
    updated_display_board(server->graph.graph, size_display, server->players[WHITE].pos, server->players[BLACK].pos);
    display_footer(size_display);
    sleep(1);
}

void display_graph_value(struct graph_t *graph)
{
    for (size_t i = 0; i < graph->num_vertices; i++)
    {
        printf("%3zu : ", i);
        for (size_t j = 0; j < graph->num_vertices; j++)
        {
            printf("%d ", get_connection_type(graph, i, j));
        }
        printf("\n");
    }
}