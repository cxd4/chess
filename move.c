#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#include "move.h"
#include "check.h"

game_status game_state;

int enum_moves(move_storage * list)
{
    int x1, y1, x2, y2;
    register int legal_moves;

    legal_moves = 0;
    for (y1 = 0; y1 < BOARD_SIZE; y1++)
        for (x1 = 0; x1 < BOARD_SIZE; x1++)
            for (y2 = 0; y2 < BOARD_SIZE; y2++)
                for (x2 = 0; x2 < BOARD_SIZE; x2++)
                    if (is_legal_move(x1, y1, x2, y2)) {
                        list = realloc(
                            &list[0],
                            ++legal_moves * sizeof(move_storage)
                        );
                        assert(list != NULL);
                        list[legal_moves - 1].origin.file = x1;
                        list[legal_moves - 1].origin.rank = y1;
                        list[legal_moves - 1].target.file = x2;
                        list[legal_moves - 1].target.rank = y2;
                    }
                ;
            ;
        ;
    ;
    return (legal_moves);
}

int is_legal_move(int x1, int y1, int x2, int y2)
{
    square backup;
    double slope; /* for Bishop diagonal tests */
    int putting_yourself_in_check;
    register int xt, yt;
    const int moving_piece_color = get_player_by_square(x1, y1);
    const int target_piece_color = get_player_by_square(x2, y2);

/*
 * If either the starting or ending squares lie outside the legal boundary of
 * the board, then obviously the move cannot be legal.
 */
    if (x1 < 0 || x1 >= BOARD_SIZE)
        return 0;
    if (y1 < 0 || y1 >= BOARD_SIZE)
        return 0;
    if (x2 < 0 || x2 >= BOARD_SIZE)
        return 0;
    if (y2 < 0 || y2 >= BOARD_SIZE)
        return 0;

/*
 * "Null" moves, of course, also are not legal in chess.
 * If the starting square is the same as the ending square, there is no move.
 *
 * Maybe this sort of condition is already prevented implicitly by the rest
 * of the algorithm, but it's more readable to check this now.  It also helps
 * speed up the move search analysis due to the liklihood of the condition.
 */
    if (x1 == x2 && y1 == y2)
        return 0;

    if (board[y1][x1] == BLANK_SQUARE)
        return 0; /* can move to no squares, from an empty square */

    if (game_state.player_turn != moving_piece_color)
        return 0; /* White can't command Black pieces, and vice versa. */
    if (board[y2][x2] != BLANK_SQUARE)
        if (target_piece_color == moving_piece_color)
            return 0; /* Nobody can capture pieces of their own color. */

/*
 * Well, here comes the hard part.  :(
 */
    switch (board[y1][x1])
    {
    case WHITE_KING:
    case BLACK_KING:
        if (x2 == x1) /* off upwards or downwards by one */
            if (y2 == y1 - 1 || y2 == y1 + 1)
                break;
        if (x2 == x1 - 1 || x2 == x1 + 1)
            if (y2 == y1 - 1 || y2 == y1 + 1 || y2 == y1)
                break;
        return 0;

    case WHITE_QUEEN:
    case BLACK_QUEEN:

    case WHITE_ROOK:
    case BLACK_ROOK:
        if (y2 == y1)
            if (x2 < x1) /* destination to the left of the rook */
                for (xt = x1 - 1; xt > x2; xt--) {
                    if (board[y2][xt] == BLANK_SQUARE)
                        continue;
                    return 0;
                }
            else /* destination to the right of the rook */
                for (xt = x1 + 1; xt < x2; xt++) {
                    if (board[y2][xt] == BLANK_SQUARE)
                        continue;
                    return 0;
                }
        else if (x2 == x1)
            if (y2 < y1) /* destination downwards from the rook */
                for (yt = y1 - 1; yt > y2; yt--) {
                    if (board[yt][x2] == BLANK_SQUARE)
                        continue;
                    return 0;
                }
            else /* destination upwards from the rook */
                for (yt = y1 + 1; yt < y2; yt++) {
                    if (board[yt][x2] == BLANK_SQUARE)
                        continue;
                    return 0;
                }
        else
            return 0;

        if (board[y1][x1] != WHITE_QUEEN && board[y1][x1] != BLACK_QUEEN)
            break; /* Do not fall through to do Queen tests. */

    case WHITE_BISHOP:
    case BLACK_BISHOP:
        if (x2 == x1) /* divides by 0, so no bishop testing needed */
            return 0;
        slope =
            (double)(y2 - y1)
          / (double)(x2 - x1)
        ;
        if (slope != +1 && slope != -1)
            return 0;
        if (y2 > y1 && x2 > x1) /* quadrant I, range check to upper-right */
            for (xt = x1 + 1, yt = y1 + 1; xt < x2 && yt < y2; xt++, yt++)
                if (board[yt][xt] != BLANK_SQUARE)
                    return 0;
        if (y2 > y1 && x2 < x1) /* quadrant II, range check to upper-left */
            for (xt = x1 - 1, yt = y1 + 1; xt > x2 && yt < y2; xt--, yt++)
                if (board[yt][xt] != BLANK_SQUARE)
                    return 0;
        if (y2 < y1 && x2 < x1) /* quadrant III, range check to lower-left */
            for (xt = x1 - 1, yt = y1 - 1; xt > x2 && yt > y2; xt--, yt--)
                if (board[yt][xt] != BLANK_SQUARE)
                    return 0;
        if (y2 < y1 && x2 < x1) /* quadrant IV, range check to lower-right */
            for (xt = x1 + 1, yt = y1 - 1; xt < x2 && yt > y2; xt++, yt--)
                if (board[yt][xt] != BLANK_SQUARE)
                    return 0;
        break;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
        if (x2 == x1 - 1 || x2 == x1 + 1)
            if (y2 == y1 - 2 || y2 == y1 + 2)
                break; /* 1 o'clock, 5 o'clock, 7 o'clock, 11 o'clock */
        if (x2 == x1 - 2 || x2 == x1 + 2)
            if (y2 == y1 - 1 || y2 == y1 + 1)
                break; /* 2 o'clock, 4 o'clock, 8 o'clock, 10 o'clock */
        return 0;

    case WHITE_PAWN:
        if (x2 == x1) /* pawn advances, without capture */
            if (board[y1 + 1][x1] == BLANK_SQUARE) {
                if (y2 == y1 + 1)
                    break;
                if (y2 == y1 + 2) /* optional pawn push from start position */
                    if (y1 == 1 && board[y1 + 2][x1] == BLANK_SQUARE)
                        break;
            }
        if (y2 == y1 + 1 && x2 != x1) /* pawn captures */
            if (x2 == x1 - 1 || x2 == x1 + 1)
                if (board[y2][x1] != BLANK_SQUARE
                 || game_state.en_passant_file == x2 && y2 == 2)
                    break;
        return 0;
    case BLACK_PAWN:
        if (x2 == x1)
            if (board[y1 - 1][x1] == BLANK_SQUARE) {
                if (y2 == y1 - 1)
                    break;
                if (y2 == y1 - 2)
                    if (y1 == BOARD_SIZE-2 && board[y1 - 2][x1] == BLANK_SQUARE)
                        break;
            }
        if (y2 == y1 - 1 && x2 != x1)
            if (x2 == x1 - 1 || x2 == x1 + 1)
                if (board[y2][x1] != BLANK_SQUARE
                 || game_state.en_passant_file == x2 && y2 == BOARD_SIZE - 3)
                    break;
        return 0;
    }

/*
 * Finally, is the moving player putting himself in check with this move?
 * This we will test by temporarily executing the move on the board.
 *
 * In ancient chess, exposing one's King is a legal move, so this code could
 * be deleted actually for a theoretically faster analysis.
 */
    backup = board[y2][x2];
    board[y2][x2] = board[y1][x1];
    board[y1][x1] = BLANK_SQUARE;
    putting_yourself_in_check = in_check(moving_piece_color);

    board[y1][x1] = board[y2][x2];
    board[y2][x2] = backup;
    if (putting_yourself_in_check)
        return 0;

    return 1;
}