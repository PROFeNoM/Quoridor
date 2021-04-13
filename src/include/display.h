#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "server.h"
#include <unistd.h>
#include <string.h>

void display_game(struct server *server, size_t turn, size_t current_player);
void updated_display(struct server *server, size_t turn, size_t current_player);
#endif // _GRAPH_PATTERN_H_