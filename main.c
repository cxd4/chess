#include <stdio.h>
#include "forsyth.h"

#include "move.h"
#include "check.h"

move_storage * move_list = NULL;

int main(int argc, char* argv[])
{
    FILE * stream;
    int FEN_status;
    int legal_moves;

    load_LUTs();
    if (argc < 2)
        FEN_status = load_FEN(
            "rnbqkbnr/"\
            "pppppppp/"\
            "8/"\
            "8/"\
            "8/"\
            "8/"\
            "PPPPPPPP/"\
            "RNBQKBNR "

            "w KQkq - 0 1"
        );
    else
        FEN_status = load_FEN(argv[1]);

    if (FEN_status != FEN_OK)
    {
        fprintf(stderr, "(%i):  FEN record is corrupt!\n\n", FEN_status);
        return (FEN_status);
    }

    stream = (argc < 3) ? stdout : fopen(argv[2], "w");
    load_Forsyth(stream);
    if (stream != stdout)
        fclose(stream);

    legal_moves = enum_moves(&move_list);
    if (legal_moves > 0) {
        printf("Total of %i legal moves.\n", legal_moves);
        show_moves(move_list, legal_moves);

     /* to do:  Add player response/move execution loop to play game here? */
    } else {
        printf(
            "%smate.  That's the end!\n",
            in_check(game_state.player_turn) ? "Check" : "Stale"
        );
        return 0; /* game play finished successfully */
    }

    return 0;
}
