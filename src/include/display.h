#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "server.h"
#include <unistd.h>
#include <string.h>

void display_game(struct server *server, size_t turn, enum color_t);
void display_winner(struct server *server, size_t turn, enum color_t);
void display_move(struct server *server, struct move_t move);
void display_error_move(const char *error_type, const char *p_name, const char *p_type);
#endif // _GRAPH_PATTERN_H_