#ifndef _BOARD_H_
#define _BOARD_H_

enum {
    a, b, c, d, e, f, g, h,
    BOARD_SIZE
};

static const char files[BOARD_SIZE] = {
    'a', /* QR */
    'b', /* QKt */
    'c', /* QB */
    'd', /* Q */
    'e', /* K */
    'f', /* KB */
    'g', /* KKt */
    'h', /* KR */
};

typedef signed char square;

extern square board[BOARD_SIZE][BOARD_SIZE];
#define COORDS(file, rank)      board[(rank) - 1][(file)]

enum {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8,
    NUMBER_OF_SQUARES
};
#define SQUARE(name)            \
    board[((name)/BOARD_SIZE) % BOARD_SIZE][(name) % BOARD_SIZE]

#ifdef BITS_PER_COORDINATE
#error BITS_PER_COORDINATE defined prematurely.
#endif

typedef struct {
#if (BOARD_SIZE == 8)
#define BITS_PER_COORDINATE 3
#else
#define BITS_PER_COORDINATE 16
#endif
    unsigned file:  BITS_PER_COORDINATE;
    unsigned rank:  BITS_PER_COORDINATE;
#undef BITS_PER_COORDINATE
} coordinates;

#endif
