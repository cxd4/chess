#include <stdio.h>

#include "board.h"
#include "pieces.h"

#include "forsyth.h"

square board[BOARD_SIZE][BOARD_SIZE];
game_status game_state;

/*
 * 9/11/2012
 * to do:  This code sucks and is very old. :) Delete all the things.
 */

static const char LUT_pieces[128] = {
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1,
        WHITE_BISHOP,
                 -1, -1, -1, -1, -1, -1, -1, -1,
        WHITE_KING,
                                                     -1, -1,
        WHITE_KNIGHT,
                                                                 -1,
    WHITE_PAWN, WHITE_QUEEN, WHITE_ROOK,
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     -1, -1,
        BLACK_BISHOP,
                 -1, -1, -1, -1, -1, -1, -1, -1,
        BLACK_KING,
                                                     -1, -1,
        BLACK_KNIGHT,
                                                                 -1,
    BLACK_PAWN, BLACK_QUEEN, BLACK_ROOK,
                 -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

void load_Forsyth(FILE * stream)
{
    static char pieces[128];
    register int file, rank;

    pieces[WHITE_KING]   = 'K'; pieces[BLACK_KING]   = 'k';
    pieces[WHITE_QUEEN]  = 'Q'; pieces[BLACK_QUEEN]  = 'q';
    pieces[WHITE_ROOK]   = 'R'; pieces[BLACK_ROOK]   = 'r';
    pieces[WHITE_BISHOP] = 'B'; pieces[BLACK_BISHOP] = 'p';
    pieces[WHITE_KNIGHT] = 'N'; pieces[BLACK_KNIGHT] = 'n';
    pieces[WHITE_PAWN]   = 'P'; pieces[BLACK_PAWN]   = 'p';
    pieces[BLANK_SQUARE] = '.'; /* compatible formatting to GNU Chess */

    rank = BOARD_SIZE;
    while (--rank >= 0)
    {
        for (file = 0; file < BOARD_SIZE; file++)
        {
            fputc(pieces[board[rank][file]], stream);
            fputc(' ', stream);
        }
        fputc('\n', stream);
    }
    return;
}

/*
 * 9/11/2012
 * Don't worry!  It gets worse!  Thank me later for the warning.
 *
 * This reeks far too much of iterative, assembly-style C code and needs to
 * be redone to something more flexible and readable.
 */

int load_FEN(char * FEN)
{
    register int i;
    register int file, rank;

    /* field 1:  piece placement */
    i = 0;

    file = 0;
    rank = BOARD_SIZE - 1; /* Forsyth notation always starts at Black's end. */
    do {
        if (FEN[i] >= '0' && FEN[i] <= '9')
        { /* number of squares before next piece or end of rank */
            int square_offset;

            square_offset = (FEN[i] - '0') % 10;
            if (square_offset == 0)
                return -FEN_INVALID_SQUARE_COUNT;
            do {
                board[rank][file++] = BLANK_SQUARE;
                --square_offset;
            } while (square_offset != 0);
            if (file > 8)
                return -FEN_FILE_RANGE_OVERFLOW;
        }
        else
        { /* letter identifying a piece to assign */
            if (LUT_pieces[FEN[i]] < 0)
                return -FEN_INVALID_PIECE_LETTER;
            board[rank][file++] = LUT_pieces[FEN[i]];
        }

        ++i;
        if (file == BOARD_SIZE)
        { /* end of the rank */
            if (rank == 0)
            { /* all sixty-four squares of chess board defined */
                if (FEN[i] == ' ')
                    break;
                return -FEN_PIECE_TABLE_OUT_OF_BOUNDS;
            }
            if (FEN[i] != '/') /* divider between chessboard ranks */
                return -FEN_RANK_RANGE_OVERFLOW;
            ++i;
            file = 0;
            --rank;
        }
    } while (i == i);

    /* field 2:  active color */
    ++i;

    switch (FEN[i])
    {
        case 'b':  game_state.player_turn = BLACK; break;
        case 'w':  game_state.player_turn = WHITE; break;
        default:  return -FEN_COLOR_INVALID;
    }

    /* field 3:  castling availability */
    ++i;
    if (FEN[i] != ' ')
        return -FEN_NO_COLOR_CASTLING_DELIMITER;

    if (FEN[++i] == '-')
    {
        if (FEN[++i] != ' ')
        {
            return -FEN_NO_CASTLING_AND_NO_DELIMITER;
        }
    }
    else
    {
        signed char LUT_castling[128] = { -1 };

        LUT_castling['K'] = 32;
        LUT_castling['Q'] = 32;
        LUT_castling['k'] = 32;
        LUT_castling['q'] = 32;
        if (LUT_castling[FEN[i]] < 0)
            return -FEN_UNREADABLE_CASTLING_PRIVILEGES;

        SQUARE(g8) |= LUT_castling[FEN[i]];
        if (FEN[++i] != ' ')
        {
            LUT_castling['K'] = -1;
            LUT_castling['Q'] = 0x20;
            if (LUT_castling[FEN[i]] < 0)
                return -FEN_KQ_UNREADABLE_CASTLING;
            game_state.castling.q = LUT_castling[FEN[i]];
            if (FEN[++i] != ' ')
            {
                LUT_castling['Q'] = -1;
                LUT_castling['k'] = 0x20;
                if (LUT_castling[FEN[i]] < 0)
                    return -FEN_QK_UNREADABLE_CASTLING;
                SQUARE(g1) |= LUT_castling[FEN[i]];
                if (FEN[++i] != ' ')
                {
                    if (FEN[i] != 'q')
                        return -FEN_Q_UNREADABLE_CASTLING;
                    game_state.castling.K |= 0x20;
                    if (FEN[++i] != ' ')
                        return -FEN_NO_DELIMITER_TO_EN_PASSANT;
                }
            }
        }
    }

    /* field 4:  en passant target square */
    ++i;

    if (FEN[i] != '-')
    {
        file = FEN[i] - 'a';
        if (file < a || file > h)
            return -FEN_FILE_EN_PASSANT_INVALID;
        rank = FEN[++i] - '0';
        if (rank != 2 && rank != 6)
            return -FEN_RANK_EN_PASSANT_INVALID;
        game_state.en_passant_file = file;
    }

    /* field 5:  halfmove clock */
    ++i;

    if (FEN[i++] != ' ')
        return -FEN_NO_DELIMITER_TO_PLY;
    do {
        if (FEN[i] < '0' || FEN[i] > '9')
            return -FEN_PLY_UNREADABLE;
        game_state.Ply *= 10;
        game_state.Ply += (FEN[i++] - '0') % 10;
        if (FEN[i] == ' ')
        {
            if (game_state.Ply <= 50)
                break;
            return -FEN_PLY_COUNT_EXCEEDS_FIFTY_MOVE_DRAW_RULE;
        }
    } while (i == i);

    /* field 6:  fullmove clock */
    ++i;

    do {
        if (FEN[i] < '0' || FEN[i] > '9')
            return -FEN_NO_DELIMITER_TO_CLOCK;
        game_state.fullmove_clock *= 10;
        game_state.fullmove_clock += (FEN[i++] - '0') % 10;
        if (FEN[i] == '\0')
            break;
    } while (i == i);

    if (game_state.fullmove_clock == 0)
        return -FEN_IMPOSSIBLE_CLOCK_VALUE;
    return FEN_OK;
}
