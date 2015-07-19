#include <stdio.h>
#include "forsyth.h"

#include "move.h"
#include "check.h"

move_storage move_list[1];

int main(int argc, char* argv[])
{
    FILE * stream;
    int FEN_status;
    int legal_moves;
    register int i;

    if (argc < 2)
        FEN_status = load_FEN(
            "rnbqkbnr/"\
            "pppppppp/"\
            "8/"\
            "8/"\
            "8/"\
            "8/"\
            "PPPPPPPP/"
            "RNBQKBNR "

            "w KQkq - 0 1"
        );
    else
        FEN_status = load_FEN(argv[1]);

#if defined(TEST_MATE_IN_ONE)
    memcpy(&square(a8), "nk....bq", 8);
    memcpy(&square(a7), "ppp.r...", 8);
    memcpy(&square(a6), "........", 8);
    memcpy(&square(a5), "........", 8);
    memcpy(&square(a4), "........", 8);
    memcpy(&square(a3), "........", 8);
    memcpy(&square(a2), "...R.PPP", 8);
    memcpy(&square(a1), "QB....KN", 8);
#endif

    if (FEN_status != FEN_OK)
    {
        fprintf(stderr, "(%i):  FEN record is corrupt!\n\n", FEN_status);
        return (FEN_status);
    }

    stream = (argc < 3) ? stdout : fopen(argv[2], "w");
    load_Forsyth(stream);
    if (stream != stdout)
        fclose(stream);

    legal_moves = enum_moves(&move_list[0]);
    printf("Total of %i legal moves:\n", legal_moves);
    for (i = 0; i < legal_moves; i++)
        printf(
            "    % 3i.  %c%c%d:%c%d\n",
            i,
            LUT_pieces[board[move_list->origin.file][move_list->origin.rank]],
            files[move_list->origin.file], move_list->origin.rank,
            files[move_list->target.file], move_list->target.rank
        );

    if (in_check(WHITE))
        puts("White is in check.");
    if (in_check(BLACK))
        puts("Black is in check.");
    return 0;
}
