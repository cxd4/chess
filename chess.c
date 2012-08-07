#include <stdio.h>

char game[8][8] =
{ /* QR    QN    QB     Q     K    KB    KN    KR */
    0x02, 0x04, 0x23, 0x01, 0x00, 0x03, 0x24, 0x02, /* rank 1 */
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, /* rank 2 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* rank 3 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* rank 4 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* rank 5 */
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /* rank 6 */
    0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x15, /* rank 7 */
    0x12, 0x14, 0x33, 0x11, 0x10, 0x13, 0x34, 0x12  /* rank 8 */
};
int game_flags[4]; /* active color, halfmove clock, fullmove clock, draw data */

int load_FEN(char FEN[96]);
int main(int argc, char *argv[])
{
    char board[128] = { /* ASCII chess board testing */
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . k . . . . .\n"
        ". . . . . . . .\n"
        "K . . . . . . .\n"
        ". . . q . . . ." };
    printf(board);
    printf("\n\nBlack to win in two moves.\n\n");
    if (argc == 2)
    {
        printf("load_FEN(argv[1]) == %i\n", load_FEN(argv[1]));
    }
    return 0;
}

int load_FEN(char FEN[96])
{
    register int i = 0; /* array indexing loop variable */
    register int file = 0; /* file A, second dimension of the game[] array */
    register int rank = 7; /* rank 8, first dimension of game array access */

    char pieces[128] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1,003, -1, -1, -1, -1, -1, -1, -1, -1,000, -1, -1,004, -1,
       005,001,002, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1,023, -1, -1, -1, -1, -1, -1, -1, -1,020, -1, -1,024, -1,
       025,021,022, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    char ep_file[128] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    char ep_rank[128] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1,  5, -1, -1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    char dec_fig[128] = {
        -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    /* array look-up tables to instantaneously assign conditional integers */
    FEN[92] =  ' '; /* to prevent the next loop from looping indefinitely */
    FEN[93] = '\r'; /* Windows new line format:  CRLF */
    FEN[94] = '\n'; /* '\r' is the carriage return; '\n' is the line feed. */
    FEN[95] = '\0'; /* standard, necessary to prevent indefinite recursion */

    /* field 1:  piece placement */
    do
    {
        if ((FEN[i] & 0xFFF0) == '0')
        { /* number of squares before next piece or end of rank */
            register int square_offset = FEN[i] & 0x000F;
            if (square_offset == 0) return -1;
            file = file + square_offset;
            if (file > 8) return -2;
        }
        else
        { /* letter identifying a piece to assign */
            if (pieces[FEN[i]] == -1) return -3;
            game[file++][rank] = pieces[FEN[i]];
        }

        ++i;
        if (file == 8)
        { /* end of the rank */
            if (rank == 0)
            { /* all sixty-four squares of chess board defined */
                if (FEN[i] == ' ') break;
                else return -4;
            }
            if (FEN[i] == '/') /* divider between chessboard ranks */
            {
                ++i;
                file = file ^ file;
                --rank;
            }
            else return -5;
        }
    } while (i == i);

    /* field 2:  active color */
    switch (FEN[++i]) {
        case 'b':  game_flags[0] = 1; /* Black's turn to move */
        case 'w':  break;
        default:  return -6;
    }

    /* field 3:  castling availability */
    if (FEN[++i] == ' ') {}
    else return -7;
    switch (FEN[++i]) {
        case '-':
            if (FEN[++i] == ' ') break;
            return -8;
        case 'K':
            game[7][6] |= 0x20;
            switch (FEN[++i]) {
                case ' ':  break;
                case 'Q':
                    game[7][2] |= 0x20;
                    switch (FEN[++i]) {
                        case ' ':  break;
                        case 'k':
                            game[0][6] |= 0x20;
                            switch (FEN[++i]) {
                                case ' ':  break;
                                case 'q':
                                    game[0][2] |= 0x20;
                                    if (FEN[++i] == ' ') break;
                                default:  return -9;
                            }
                            break;
                        case 'q':
                            game[0][2] |= 0x20;
                            if (FEN[++i] == ' ') break;
                        default:  return -10;
                    }
                    break;
                case 'k':
                    game[0][6] |= 0x20;
                    switch (FEN[++i]) {
                        case ' ':  break;
                        case 'q':
                            game[0][2] |= 0x20;
                            if (FEN[++i] == ' ') break;
                        default:  return -11;
                    }
                    break;
                case 'q':
                    game[0][2] |= 0x20;
                    if (FEN[++i] == ' ') break;
                default:  return -12;
            }
            break;
        case 'Q':
            game[7][2] |= 0x20;
            switch (FEN[++i]) {
                case ' ':  break;
                case 'k':
                    game[0][6] |= 0x20;
                    switch (FEN[++i]) {
                        case ' ':  break;
                        case 'q':
                            game[0][2] |= 0x20;
                            if (FEN[++i] == ' ') break;
                        default:  return -13;
                    }
                    break;
                case 'q':
                    game[0][2] |= 0x20;
                    if (FEN[++i] == ' ') break;
                default:  return -14;
            }
            break;
        case 'k':
            game[0][6] |= 0x20;
            switch (FEN[++i]) {
                case ' ':  break;
                case 'q':
                    game[0][2] |= 0x20;
                    if (FEN[++i] == ' ') break;
                default:  return -15;
            }
            break;
        case 'q':
            game[0][2] |= 0x20;
            if (FEN[++i] == ' ') break;
        default:
            return -16;
    }

    /* field 4:  en passant target square */
    if (FEN[++i] == '-') {}
    else
    {
        file = ep_file[FEN[i]];
        if (file == -1) return -17;
        rank = ep_rank[FEN[++i]];
        if (rank == -1) return -18;
        game[file][rank] |= 0x40; /* en passant destination flag */
    }

    /* field 5:  halfmove clock */
    if (FEN[++i] == ' ') {}
    else return -19;
    do
    {
        if (dec_fig[FEN[++i]] == -1) return -20;
        if (FEN[i] == ' ')
        {
            if (game_flags[1] > 50) return -21;
            break;
        }
        game_flags[1] *= 10;
        game_flags[1] += dec_fig[FEN[i]];
    } while (i == i);

    /* field 6:  fullmove clock */
    do
    {
        if (dec_fig[FEN[++i]] == -1) return -22;
        if (FEN[i] == '\0')
        {
            if (game_flags[2] == 0) return -23;
            break;
        }
        game_flags[2] *= 10;
        game_flags[2] += dec_fig[FEN[i]];
    } while (i == i);
    return 0;
}
