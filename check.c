#include "board.h"
#include "pieces.h"
#include "check.h"

int in_check(int player)
{
    char key;
    int is_in_check;
    register int file, rank;

    key = '\0';
    switch (player)
    {
    case WHITE:
        key = WHITE_KING;
        break;
    case BLACK:
        key = BLACK_KING;
        break;
    default:
        return -1;
    }

    for (rank = 0; rank < BOARD_SIZE; rank++)
        for (file = 0; file < BOARD_SIZE; file++)
            if (board[rank][file] == key)
                break;
    if (file >= BOARD_SIZE || rank >= BOARD_SIZE)
        return -1; /* no King found--cannot interpret King safety */

    is_in_check  = 0;
    is_in_check |= test_King(file, rank);
    is_in_check |= test_rook(file, rank);
    is_in_check |= test_knight(file, rank);
    is_in_check |= test_pawn(file, rank);
    is_in_check |= test_bishop(file, rank);
    is_in_check |= test_Queen(file, rank);
    return (is_in_check);
}

int get_player_by_square(int file, int rank)
{
    switch (board[rank][file])
    {
    case WHITE_KING:
    case WHITE_QUEEN:
    case WHITE_ROOK:
    case WHITE_BISHOP:
    case WHITE_KNIGHT:
    case WHITE_PAWN:
        return WHITE;

    case BLACK_KING:
    case BLACK_QUEEN:
    case BLACK_ROOK:
    case BLACK_BISHOP:
    case BLACK_KNIGHT:
    case BLACK_PAWN:
        return BLACK;

    case BLANK_SQUARE:
    default:
        return (game_state.player_turn);
    }
}

int test_Queen(int file, int rank)
{
    if (test_rook(file, rank))
        return 1;
    if (test_bishop(file, rank))
        return 1;

    return 0;
}

int test_rook(int file, int rank)
{
    register int x, y;
    const int threatened_player = get_player_by_square();

horz_left:
    for (x = file - 1; x >= 0; x--)
        switch (board[rank][x])
        {
        case WHITE_QUEEN:
        case WHITE_ROOK:
            if (threatened_player == WHITE)
                goto horz_right;
            return 1;

        case BLACK_QUEEN:
        case BLACK_ROOK:
            if (threatened_player == BLACK)
                goto horz_right;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            goto horz_right;
        }

horz_right:
    for (x = file + 1; x < BOARD_SIZE; x++)
        switch (board[rank][x])
        {
        case WHITE_QUEEN:
        case WHITE_ROOK:
            if (threatened_player == WHITE)
                goto vert_down;
            return 1;

        case BLACK_QUEEN:
        case BLACK_ROOK:
            if (threatened_player == BLACK)
                goto vert_down;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            goto vert_down;
        }

vert_down:
    for (y = rank - 1; y >= 0; y--)
        switch (board[y][file])
        {
        case WHITE_QUEEN:
        case WHITE_ROOK:
            if (threatened_player == WHITE)
                goto vert_up;
            return 1;

        case BLACK_QUEEN:
        case BLACK_ROOK:
            if (threatened_player == BLACK)
                goto vert_up;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            goto vert_up;
        }

vert_up:
    for (y = rank + 1; y < BOARD_SIZE; y++)
        switch (board[y][file])
        {
        case WHITE_QUEEN:
        case WHITE_ROOK:
            if (threatened_player == WHITE)
                return 0;
            return 1;

        case BLACK_QUEEN:
        case BLACK_ROOK:
            if (threatened_player == BLACK)
                return 0;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            return 0;
        }
    return 0;
}

int test_bishop(int file, int rank)
{
    register int x, y;
    const int threatened_player = get_player_by_square();

lo_left:
    for (x = file - 1, y = rank - 1; x >= 0 && y >= 0; x--, y--)
        switch (board[y][x])
        {
        case WHITE_QUEEN:
        case WHITE_BISHOP:
            if (threatened_player == WHITE)
                goto lo_right;
            return 1;

        case BLACK_QUEEN:
        case BLACK_BISHOP:
            if (threatened_player == BLACK)
                goto lo_right;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            goto lo_right;
        }

lo_right:
    for (x = file + 1, y = rank - 1; x < BOARD_SIZE && y >= 0; x++, y--)
        switch (board[y][x])
        {
        case WHITE_QUEEN:
        case WHITE_BISHOP:
            if (threatened_player == WHITE)
                goto hi_left;
            return 1;

        case BLACK_QUEEN:
        case BLACK_BISHOP:
            if (threatened_player == BLACK)
                goto hi_left;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            goto hi_left;
        }

hi_left:
    for (x = file - 1, y = rank + 1; x >= 0 && y < BOARD_SIZE; x--, y++)
        switch (board[y][x])
        {
        case WHITE_QUEEN:
        case WHITE_BISHOP:
            if (threatened_player == WHITE)
                goto hi_right;
            return 1;

        case BLACK_QUEEN:
        case BLACK_BISHOP:
            if (threatened_player == BLACK)
                goto hi_right;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            goto hi_right;
        }

hi_right:
    for (x = file + 1, y = rank + 1; x < BOARD_SIZE && y < BOARD_SIZE; x++, y++)
        switch (board[y][x])
        {
        case WHITE_QUEEN:
        case WHITE_BISHOP:
            if (threatened_player == WHITE)
                return 0;
            return 1;

        case BLACK_QUEEN:
        case BLACK_BISHOP:
            if (threatened_player == BLACK)
                return 0;
            return 1;

        case BLANK_SQUARE:
            continue;
        default:
            return 0;
        }
    return 0;
}
