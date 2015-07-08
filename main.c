#include <stdio.h>
#include "forsyth.h"

int main(int argc, char* argv[])
{
    FILE * stream;
    int FEN_status;

    if (argc < 2)
        FEN_status = load_FEN(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
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

    load_Forsyth();
    if (argc < 3)
        stream = stdout;
    else
        stream = fopen(argv[2], "w");
    fprintf(stream, "%s\n\n", board_ASCII);
    fclose(stream);
    return 0;
}
