#ifndef _PIECES_H_
#define _PIECES_H_

#if ('K' > 'k')
#define COLOR_CASE      ('K' - 'k')
#else
#define COLOR_CASE      ('k' - 'K')
#endif

enum {
    WHITE,
    BLACK,

    NUMBER_OF_PLAYERS
};

static const char colors[NUMBER_OF_PLAYERS][6] = {
    "White",
    "Black",
};

enum {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,

    NUMBER_OF_CHARACTERS
};

enum {
    WHITE_KING   = 'K',
    WHITE_QUEEN  = 'Q',
    WHITE_ROOK   = 'R',
    WHITE_BISHOP = 'B',
    WHITE_KNIGHT = 'N',
    WHITE_PAWN   = 'P',

    BLACK_KING   = WHITE_KING ^ COLOR_CASE,
    BLACK_QUEEN  = WHITE_QUEEN ^ COLOR_CASE,
    BLACK_ROOK   = WHITE_ROOK ^ COLOR_CASE,
    BLACK_BISHOP = WHITE_BISHOP ^ COLOR_CASE,
    BLACK_KNIGHT = WHITE_KNIGHT ^ COLOR_CASE,
    BLACK_PAWN   = WHITE_PAWN ^ COLOR_CASE,

	BLANK_SQUARE = '.'
};

#endif
