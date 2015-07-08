#ifndef _FORSYTH_H_
#define _FORSYTH_H_

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

extern int load_FEN(char * FEN);
extern void load_Forsyth(void);

enum {
    FEN_OK,
    FEN_INVALID_SQUARE_COUNT,
    FEN_FILE_RANGE_OVERFLOW,
    FEN_INVALID_PIECE_LETTER,
    FEN_PIECE_TABLE_OUT_OF_BOUNDS,
    FEN_RANK_RANGE_OVERFLOW,
    FEN_COLOR_INVALID,
    FEN_NO_COLOR_CASTLING_DELIMITER,
    FEN_NO_CASTLING_AND_NO_DELIMITER,
    FEN_UNREADABLE_CASTLING_PRIVILEGES,
    FEN_KQ_UNREADABLE_CASTLING,
    FEN_QK_UNREADABLE_CASTLING,
    FEN_Q_UNREADABLE_CASTLING,
    FEN_NO_DELIMITER_TO_EN_PASSANT,
    FEN_FILE_EN_PASSANT_INVALID,
    FEN_RANK_EN_PASSANT_INVALID,
    FEN_NO_DELIMITER_TO_PLY,
    FEN_PLY_UNREADABLE,
    FEN_PLY_COUNT_EXCEEDS_FIFTY_MOVE_DRAW_RULE,
    FEN_NO_DELIMITER_TO_CLOCK,
    FEN_IMPOSSIBLE_CLOCK_VALUE,

    FEN_ERROR_UNKNOWN
};

#endif
