#include <stdio.h>
#include "forsyth.h"

int main(int argc, char* argv[])
{
    FILE * stream;
    int FEN_status;

    if (argc < 2)
        fen_status = load_FEN(
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        );
    else
        ret_val = load_FEN(argv[1]);

    if (fen_status != FEN_OK)
    {
        fprintf(stderr, "(%i):  FEN record is corrupt!\n\n", fen_status);
        return (fen_status);
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
