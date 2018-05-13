#ifndef _OPENINGS_H_
#define _OPENINGS_H_

#include "move.h"

typedef struct {
    const char* FEN;
    move_storage best_move;
    unsigned int num_alts;
    const move_storage* alt_moves;
} op_pos;

#endif
