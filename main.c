#include <malloc.h>
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
    register int i;

    load_LUTs();
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
    memcpy(&SQUARE(a8), "nk....bq", 8);
    memcpy(&SQUARE(a7), "ppp.r...", 8);
    memcpy(&SQUARE(a6), "........", 8);
    memcpy(&SQUARE(a5), "........", 8);
    memcpy(&SQUARE(a4), "........", 8);
    memcpy(&SQUARE(a3), "........", 8);
    memcpy(&SQUARE(a2), "...R.PPP", 8);
    memcpy(&SQUARE(a1), "QB....KN", 8);
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

    move_list = (move_storage *)malloc(0);
    legal_moves = enum_moves(&move_list[0]);
    printf("Total of %i legal moves:\n", legal_moves);
    for (i = 0; i < legal_moves; i++)
    {
        const int x1 = move_list[i].origin.file;
        const int y1 = move_list[i].origin.rank;
        const int x2 = move_list[i].target.file;
        const int y2 = move_list[i].target.rank;

        printf(
            "    % 3i.  %c%c%d:%c%d\n",
            i + 1,
            algebraic_prefixes[board[x1][y1]],
            files[x1], y1 + 1,
            files[x2], y2 + 1
        );
    }

    if (in_check(WHITE))
        puts("White is in check.");
    if (in_check(BLACK))
        puts("Black is in check.");
    return 0;
}
