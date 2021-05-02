#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "server.h"
#include <unistd.h>
#include <string.h>

/**
 * Display the current state of the game in a server
 * (e.g to display player's name, current turn number and the board)
 * @param server Server holding the game
 * @param turn Current number of turn
 * @param current_player ID of the current player
 */
void display_game(struct server *server, size_t turn, enum color_t current_player);

/**
 * Display the winner of the game held in a server
 * (e.g to display_game then display winner)
 * @param server Server holding the game
 * @param turn Current number of turn
 * @param winner ID of the winner (NO_COLOR if it is a draw)
 */
void display_winner(struct server *server, size_t turn, enum color_t winner);

/**
 * Display the move of a player
 * @param server Server holding the game
 * @param move Move of a player
 */
void display_move(struct server *server, struct move_t move);

/**
 * Display a move error
 * @param error_type Type of error
 * @param p_name Player's name of the one who did the error
 * @param p_type Color of the player
 */
void display_error_move(const char *error_type, const char *p_name, const char *p_type);

#endif