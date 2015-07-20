#include <math.h>

#include <assert.h>
#include <malloc.h>
#include <stdlib.h>

#include "move.h"
#include "check.h"

game_status game_state;

int enum_moves(move_storage ** list_address)
{
    move_storage * list;
    void * new_address;
    int x1, y1, x2, y2;
    register int legal_moves;

    if (list_address == NULL)
        return (legal_moves = 0);
    list = *(list_address);
    if (list == NULL)
        list = (move_storage *)malloc(0);

    *(list_address) = list;
    legal_moves = 0;
    for (y1 = 0; y1 < BOARD_SIZE; y1++)
        for (x1 = 0; x1 < BOARD_SIZE; x1++)
            for (y2 = 0; y2 < BOARD_SIZE; y2++)
                for (x2 = 0; x2 < BOARD_SIZE; x2++)
                    if (is_legal_move(x1, y1, x2, y2)) {
                        new_address = realloc(
                            list,
                            ++legal_moves * sizeof(move_storage)
                        );
                        assert(new_address != NULL);
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

int show_moves(move_storage * list, int limit)
{
    register int i;

    if (limit <= 0)
        limit = enum_moves(&list);
    for (i = 0; i < limit; i++)
    {
        const int x1 = list[i].origin.file;
        const int y1 = list[i].origin.rank;
        const int x2 = list[i].target.file;
        const int y2 = list[i].target.rank;

        printf(
            "    % 3i.  %c%c%d:%c%d\n",
            i + 1,
            algebraic_prefixes[board[y1][x1]],
            files[x1], y1 + 1,
            files[x2], y2 + 1
        );
    }
    return (i);
}

int is_legal_move(int x1, int y1, int x2, int y2)
{
    square backup;
    double slope, distance;
    int a, b;
    int putting_yourself_in_check;
    register int xt, yt;
    const double infinity = (double)(BOARD_SIZE - 0) / (double)(1 - 0);
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

    if (board[y1][x1] == BLANK_SQUARE)
        return 0; /* can move to no squares, from an empty square */

    if (game_state.player_turn != moving_piece_color)
        return 0; /* White can't command Black pieces, and vice versa. */
    if (board[y2][x2] != BLANK_SQUARE)
        if (target_piece_color == moving_piece_color)
            return 0; /* Nobody can capture pieces of their own color. */

    a = x2 - x1; /* horizontal leg, if any */
    b = y2 - y1; /* vertical leg, if any */
    distance = sqrt(a*a + b*b); /* vector resultant, if not also hypotenuse */

    if (distance == 0)
        return 0; /* null move */

    if (a == 0)
        slope = (b < 0) ? -infinity : +infinity;
    else
        slope = (double)b / (double)a;

    switch (board[y1][x1])
    {
    default: /* probably intended as a blank square, except not BLANK_SQUARE */
        return 0;

    case WHITE_KING:
    case BLACK_KING:
        if (distance == 1) /* up, down, right, left by one */
            break;
        if (distance == sqrt(2)) /* diagonally by one */
            break;
        return 0;

    case WHITE_QUEEN:
    case BLACK_QUEEN:
        if (slope != +1 && slope != -1 /* bishops */
         && slope !=  0 /* rooks, horizontal */
         && slope != +infinity && slope != -infinity)
            return 0;
     /* Fall through. */

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
            if (slope != +1. && slope != -1.)
                return 0;

        if (slope == -infinity || slope == 0 || slope == +infinity)
            break; /* Do not fall through to complete Queen tests. */

    case WHITE_BISHOP:
    case BLACK_BISHOP:
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
        if (y2 < y1 && x2 > x1) /* quadrant IV, range check to lower-right */
            for (xt = x1 + 1, yt = y1 - 1; xt < x2 && yt > y2; xt++, yt--)
                if (board[yt][xt] != BLANK_SQUARE)
                    return 0;
        break;

    case WHITE_KNIGHT:
    case BLACK_KNIGHT:
        if (distance == sqrt(5))
            break;
        return 0;

    case WHITE_PAWN:
        if (distance == 2) /* optional pawn push from start position */
            if (y1 == -1 + 2 && y2 == y1 + 2)
                break;
        if (distance == 1) /* pawn advances, without capture */
            if (y2 == y1 + 1 && board[y2][x2] == BLANK_SQUARE)
                break;
        if (distance == sqrt(2)) /* pawn captures */
            if (y2 == y1 + 1)
                if (board[y2][x1] != BLANK_SQUARE
                 || game_state.en_passant_file == x2 && y2 == BOARD_SIZE - 3)
                    break;
        return 0;
    case BLACK_PAWN:
        if (distance == 2)
            if (y1 == BOARD_SIZE - 2 && y2 == y1 - 2)
                break;
        if (distance == 1)
            if (y2 == y1 - 1 && board[y2][x2] == BLANK_SQUARE)
                break;
        if (distance == sqrt(2)) /* pawn captures */
            if (y2 == y1 - 1)
                if (board[y2][x1] != BLANK_SQUARE
                 || game_state.en_passant_file == x2 && y2 == -1 + 3)
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
