#include <assert.h>

#include "board.h"
#include "pieces.h"
#include "check.h"

int in_check(int player)
{
    square key;
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
        assert(player == WHITE_KING);
    }

    rank = file = -1;
    for (rank = 0; rank < BOARD_SIZE; rank++)
        for (file = 0; file < BOARD_SIZE; file++)
            if (board[rank][file] == key)
                goto king_search_finished;

king_search_finished:
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

    default:
        assert(board[rank][file] == BLANK_SQUARE);
     /* Fall through. */
    case BLANK_SQUARE:
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
    const int threatened_player = get_player_by_square(file, rank);

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
    const int threatened_player = get_player_by_square(file, rank);

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

int test_pawn(int file, int rank)
{
    int left, right;
    const int threatened_player = get_player_by_square(file, rank);

#if (NUMBER_OF_COLORS > 2)
/* hm...sideways-moving pawns of different player colors? */
#error Only White and Black pawn directions are implemented.
#endif

#if defined(ASSUME_MEMORY_ALLOCATION_OUTSIDE_OF_BOUNDS)
    right = left = 1;
#else
    left  = (file > 0) ? 1 : 0;
    right = (file < BOARD_SIZE - 1) ? 1 : 0;
#endif

/*
 * to the lower-left and lower-right of the square (White pawn threats)
 */
    if (rank <= 0) /* Maybe (rank < 2), since White pawns can't be on rank 1. */
        goto no_wpawn_threats;
    if (board[rank - 1 + (1 - left)][file - left] == WHITE_PAWN)
        if (threatened_player != WHITE)
            return 1;
    if (board[rank - 1 + (1 - right)][file + right] == WHITE_PAWN)
        if (threatened_player != WHITE)
            return 1;

/*
 * Can the pawn vertically advance to this square without capturing?
 * We don't really care about these tests since they don't help mate checks.
 */
    if (board[rank][file] != BLANK_SQUARE) /* Pawns can't capture vertically. */
        goto no_wpawn_threats;
    if (board[rank - 1][file] == WHITE_PAWN)
        return 1;

    if (rank == 4 - 1) /* Modern FIDE rules:  Pawns can advance two squares. */
        if (board[2 - 1][file] == WHITE_PAWN) /* ... if pawn hasn't moved */
            if (board[3 - 1][file] == BLANK_SQUARE) /* if nothing interposes */
                return 1;

no_wpawn_threats:
/*
 * to the upper-left and upper-right of the square (Black pawn threats)
 */
    if (rank >= BOARD_SIZE - 1) /* maybe (BOARD_SIZE - 2) ... */
        return 0;
    if (board[rank + 1 - (1 - left)][file - left] == BLACK_PAWN)
        if (threatened_player != BLACK)
            return 1;
    if (board[rank + 1 - (1 - right)][file + right] == BLACK_PAWN)
        if (threatened_player != BLACK)
            return 1;

/*
 * Can the pawn vertically advance to this square without capturing?
 * Again, these tests are rather useless to checkmate analysis.
 */
    if (board[rank][file] != BLANK_SQUARE) /* Pawns can't capture vertically. */
        return 0;
    if (board[rank + 1][file] == BLACK_PAWN)
        return 1;

    if (rank == BOARD_SIZE - 1 - 3)
        if (board[BOARD_SIZE - 1 - 1][file] == BLACK_PAWN)
            if (board[BOARD_SIZE - 1 - 2][file] == BLANK_SQUARE)
                return 1;
    return 0;
}

int test_knight(int file, int rank)
{
    static coordinates neighbors[8];
    square enemy_knight;
    register int i;
    const int threatened_player = get_player_by_square(file, rank);

    switch (threatened_player)
    {
    case WHITE:
        enemy_knight = BLACK_KNIGHT;
        break;
    case BLACK:
        enemy_knight = WHITE_KNIGHT;
        break;
    default:
#if (NUMBER_OF_COLORS > 2)
     /* ??? */
#endif
        return (enemy_knight = -1);
    }

/*
 * 1 o'clock
 */
    i = 0;
    neighbors[i].file = file + 1;
    neighbors[i].rank = rank + 2;
    if (neighbors[i].file >= BOARD_SIZE || neighbors[i].rank >= BOARD_SIZE)
    { /* test nullified:  No piece can capture itself or take its own square. */
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 2 o'clock
 */
    ++i;
    neighbors[i].file = file + 2;
    neighbors[i].rank = rank + 1;
    if (neighbors[i].file >= BOARD_SIZE || neighbors[i].rank >= BOARD_SIZE)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 4 o'clock
 */
    ++i;
    neighbors[i].file = file + 2;
    neighbors[i].rank = rank - 1;
    if (neighbors[i].file >= BOARD_SIZE || neighbors[i].rank < 0)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 5 o'clock
 */
    ++i;
    neighbors[i].file = file + 1;
    neighbors[i].rank = rank - 2;
    if (neighbors[i].file >= BOARD_SIZE || neighbors[i].rank < 0)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 7 o'clock
 */
    ++i;
    neighbors[i].file = file - 1;
    neighbors[i].rank = rank - 2;
    if (neighbors[i].file < 0 || neighbors[i].rank < 0)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 8 o'clock
 */
    ++i;
    neighbors[i].file = file - 2;
    neighbors[i].rank = rank - 1;
    if (neighbors[i].file < 0 || neighbors[i].rank < 0)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 10 o'clock
 */
    ++i;
    neighbors[i].file = file - 2;
    neighbors[i].rank = rank + 1;
    if (neighbors[i].file < 0 || neighbors[i].rank >= BOARD_SIZE)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

/*
 * 11 o'clock
 */
    ++i;
    neighbors[i].file = file - 1;
    neighbors[i].rank = rank + 2;
    if (neighbors[i].file < 0 || neighbors[i].rank >= BOARD_SIZE)
    {
        neighbors[i].file = file;
        neighbors[i].rank = rank;
    }

    for (i = 0; i < 8; i++)
        if (board[neighbors[i].rank][neighbors[i].file] == enemy_knight)
            return 1;
    return 0;
}

int test_King(int file, int rank)
{
    static square adjacent_squares[3][3];
    int upper, right, lower, left;
    register int x, y;
    const int threatened_player = get_player_by_square(file, rank);

#if defined(ASSUME_MEMORY_ALLOCATION_OUTSIDE_OF_BOUNDS)
    upper = lower = right = left = 1;
#else
    upper = (rank < BOARD_SIZE - 1) ? 1 : 0;
    right = (file < BOARD_SIZE - 1) ? 1 : 0;
    lower = (rank > 0) ? 1 : 0;
    left  = (file > 0) ? 1 : 0;
#endif

    adjacent_squares[0][0] = board[rank - lower][file - left];
    adjacent_squares[0][1] = board[rank - lower][file];
    adjacent_squares[0][2] = board[rank - lower][file + right];

    adjacent_squares[1][0] = board[rank][file - left];
    adjacent_squares[1][1] = board[rank][file]; /* unused */
    adjacent_squares[1][2] = board[rank][file + right];

    adjacent_squares[2][0] = board[rank + upper][file - left];
    adjacent_squares[2][1] = board[rank + upper][file];
    adjacent_squares[2][2] = board[rank + upper][file + right];

/*
 * attacking King, Queen, bishop, or White pawn from lower-left or -right
 */
    for (x = 0; x < 3; x += 2)
        switch (adjacent_squares[0][x]) {
        case WHITE_PAWN:

        case WHITE_BISHOP:
        case WHITE_QUEEN:
        case WHITE_KING:
            if (threatened_player == WHITE)
                break;
            return 1;
        case BLACK_BISHOP:
        case BLACK_QUEEN:
        case BLACK_KING:
            if (threatened_player == BLACK)
                break;
            return 1;
        }

/*
 * attacking King, Queen, bishop or Black pawn from upper-left or -right
 */
    for (x = 0; x < 3; x += 2)
        switch (adjacent_squares[2][x]) {
        case WHITE_BISHOP:
        case WHITE_QUEEN:
        case WHITE_KING:
            if (threatened_player == WHITE)
                break;
            return 1;

        case BLACK_PAWN:

        case BLACK_BISHOP:
        case BLACK_QUEEN:
        case BLACK_KING:
            if (threatened_player == BLACK)
                break;
            return 1;
        }

/*
 * attacking King, Queen, or rook from the left, right, lower or upper square
 */
    for (y = 0; y < 3; y += 1)
        for (x = 1 - y%2; x < 3; x += 2) /* x = ~y & 1, if 2's cmpl. */
            switch (adjacent_squares[y][x])
            {
            case WHITE_ROOK:
            case WHITE_QUEEN:
            case WHITE_KING:
                if (threatened_player == WHITE)
                    break;
                return 1;
            case BLACK_ROOK:
            case BLACK_QUEEN:
            case BLACK_KING:
                if (threatened_player == BLACK)
                    break;
                return 1;
            }

/*
 * All sub-tests fail; nothing capable of moving like a King can move here.
 */
    return 0;
}
