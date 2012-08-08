#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "chess.h"

int main(int argc, char *argv[])
{
    if (argv[1] == NULL)
    {
        load_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    else
    {
        register int ret_val = load_FEN(argv[1]);
        if (ret_val == 0) {}
        else
        {
            printf("(%i):  FEN record is corrupt!\n\n", ret_val);
            return 1;
        }
    }

    load_Forsyth();
    printf("%s\n\n", board);
    if (argc == 3)
    {
        FILE *stream = fopen("position.txt", "w");
        fprintf(stream, board);
        fclose(stream);
    }
    getchar();
    return 0;
}
