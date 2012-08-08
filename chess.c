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
    printf("game_flags[0] == %i\n", game_flags[0]);
    printf("game_flags[1] == %i\n", game_flags[1]);
    printf("game_flags[2] == %i\n", game_flags[2]);
    printf("game_flags[3] == %i\n", game_flags[3]);
    getchar();
    return 0;
}
