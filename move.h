#ifndef _MOVE_H_
#define _MOVE_H_

/* size_t */
#include <stddef.h>

#include "board.h"

typedef struct {
    coordinates origin;
    coordinates target;
} move_storage;

typedef struct {
    int K, Q, k, q;
} castling_capabilities;

typedef struct {
    int player_turn;
    castling_capabilities castling;
    int en_passant_file;
    int Ply; /* halfmove_clock */
    int fullmove_clock;
} game_status;

extern game_status game_state;

extern int is_legal_move(int x1, int y1, int x2, int y2);
extern int enum_moves(move_storage ** list_address);
extern int show_moves(move_storage * list, int limit);

/*
 * After enum_moves() enumerates the full list of legal moves, one method to
 * expose a way to execute a move is to input the index into the moves array.
 */
extern void execute_legal_move_by_ID(move_storage * list, size_t index);

/*
 * Another way to execute a move is just by knowing the from- and to-squares.
 * This function is low-level and does not check the move for legality.
 */
extern void execute_move(int x1, int y1, int x2, int y2);

#endif
