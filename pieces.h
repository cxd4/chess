#ifndef _PIECES_H_
#define _PIECES_H_

enum {
    WHITE,
    BLACK,

    NUMBER_OF_PLAYERS
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
    BLANK_SQUARE,

    WHITE_KING   = 'K',
    WHITE_QUEEN  = 'Q',
    WHITE_ROOK   = 'R',
    WHITE_BISHOP = 'B',
    WHITE_KNIGHT = 'N',
    WHITE_PAWN   = 'P',

    BLACK_KING   = 'k',
    BLACK_QUEEN  = 'q',
    BLACK_ROOK   = 'r',
    BLACK_BISHOP = 'b',
    BLACK_KNIGHT = 'n',
    BLACK_PAWN   = 'p',

    NUMBER_OF_SQUARE_VALUES
};

#endif
