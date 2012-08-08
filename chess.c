#include <stdio.h>

char board[128] = { /* ASCII chess board testing */
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . k . . . . .\n"
    ". . . . . . . .\n"
    "K . . . . . . .\n"
    ". . . q . . . ." };
char game[8][8] =
{ /* QR    QN    QB     Q     K    KB    KN    KR */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 1 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 2 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 3 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 4 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 5 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 6 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 7 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F  /* rank 8 */
};
int game_flags[4]; /* active color, halfmove clock, fullmove clock, draw data */
int load_FEN(char FEN[96]);
void load_Forsyth(void);

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (load_FEN(argv[1]) == 0) {}
        else
        {
            printf("(%i):  FEN record is corrupt!\n\n", load_FEN(argv[1]));
            return 1;
        }
    }
    else
    {
        load_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    }
    load_Forsyth();
    printf(board);
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
            do
            {
                game[file++][rank] = 0x0F;
                --square_offset;
                if (square_offset == 0) break;
            } while (square_offset == square_offset);
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
    if (FEN[++i] == '-')
    {
        if (FEN[++i] == ' ') {}
        else return -8;
    }
    else
    {
        char castle_K[128] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, -1,
            -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1,
            -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        char castle_Q[128] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1,
            -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        char castle_k[128] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, -1,
            -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        char castle_q[128] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        if (castle_K[FEN[i]] == -1) return -8;
        game[7][6] |= castle_K[FEN[i]];
        if (FEN[++i] == ' ') {}
        else
        {
            if (castle_Q[FEN[i]] == -1) return -9;
            game[7][2] |= castle_Q[FEN[i]];
            if (FEN[++i] == ' ') {}
            else
            {
                if (castle_k[FEN[i]] == -1) return -10;
                game[0][6] |= castle_k[FEN[i]];
                if (FEN[++i] == ' ') {}
                else
                {
                    if (castle_q[FEN[i]] == -1) return -11;
                    game[0][2] |= castle_q[FEN[i]];
                    if (FEN[++i] == ' ') {}
                    else return -12;
                }
            }
        }
    }

    /* field 4:  en passant target square */
    if (FEN[++i] == '-') {}
    else
    {
        file = ep_file[FEN[i]];
        if (file == -1) return -13;
        rank = ep_rank[FEN[++i]];
        if (rank == -1) return -14;
        game[file][rank] |= 0x40; /* en passant destination flag */
    }

    /* field 5:  halfmove clock */
    if (FEN[++i] == ' ') {}
    else return -15;
    do
    {
        if (dec_fig[FEN[++i]] == -1) return -16;
        if (FEN[i] == ' ')
        {
            if (game_flags[1] > 50) return -17;
            break;
        }
        game_flags[1] *= 10;
        game_flags[1] += dec_fig[FEN[i]];
    } while (i == i);

    /* field 6:  fullmove clock */
    do
    {
        if (dec_fig[FEN[++i]] == -1) return -18;
        if (FEN[i] == '\0')
        {
            if (game_flags[2] == 0) return -19;
            break;
        }
        game_flags[2] *= 10;
        game_flags[2] += dec_fig[FEN[i]];
    } while (i == i);
    return 0;
}
void load_Forsyth(void)
{
    char pieces[80] = {
        'K', 'Q', 'R', 'B', 'N', 'P', -1, -1, -1, -1, -1, -1, -1, -1, -1, '.',
        'k', 'q', 'r', 'b', 'n', 'p', -1, -1, -1, -1, -1, -1, -1, -1, -1, '.',
        'K', 'Q', 'R', 'B', 'N', 'P', -1, -1, -1, -1, -1, -1, -1, -1, -1, '.',
        'k', 'q', 'r', 'b', 'n', 'p', -1, -1, -1, -1, -1, -1, -1, -1, -1, '.',
        'K', 'Q', 'R', 'B', 'N', 'P', -1, -1, -1, -1, -1, -1, -1, -1, -1, '.' };
    board  [0] = pieces[game[0][7]]; /* a8 */
    board  [2] = pieces[game[1][7]]; /* b8 */
    board  [4] = pieces[game[2][7]]; /* c8 ... */
    board  [6] = pieces[game[3][7]];
    board  [8] = pieces[game[4][7]];
    board [10] = pieces[game[5][7]];
    board [12] = pieces[game[6][7]];
    board [14] = pieces[game[7][7]]; /* ... h8 */
    board [16] = pieces[game[0][6]]; /* a7 */
    board [18] = pieces[game[1][6]]; /* b7 ... */
    board [20] = pieces[game[2][6]];
    board [22] = pieces[game[3][6]];
    board [24] = pieces[game[4][6]];
    board [26] = pieces[game[5][6]];
    board [28] = pieces[game[6][6]];
    board [30] = pieces[game[7][6]];
    board [32] = pieces[game[0][5]];
    board [34] = pieces[game[1][5]];
    board [36] = pieces[game[2][5]];
    board [38] = pieces[game[3][5]];
    board [40] = pieces[game[4][5]];
    board [42] = pieces[game[5][5]];
    board [44] = pieces[game[6][5]];
    board [46] = pieces[game[7][5]];
    board [48] = pieces[game[0][4]];
    board [50] = pieces[game[1][4]];
    board [52] = pieces[game[2][4]];
    board [54] = pieces[game[3][4]];
    board [56] = pieces[game[4][4]];
    board [58] = pieces[game[5][4]];
    board [60] = pieces[game[6][4]];
    board [62] = pieces[game[7][4]];
    board [64] = pieces[game[0][3]];
    board [66] = pieces[game[1][3]];
    board [68] = pieces[game[2][3]];
    board [70] = pieces[game[3][3]];
    board [72] = pieces[game[4][3]];
    board [74] = pieces[game[5][3]];
    board [76] = pieces[game[6][3]];
    board [78] = pieces[game[7][3]];
    board [80] = pieces[game[0][2]];
    board [82] = pieces[game[1][2]];
    board [84] = pieces[game[2][2]];
    board [86] = pieces[game[3][2]];
    board [88] = pieces[game[4][2]];
    board [90] = pieces[game[5][2]];
    board [92] = pieces[game[6][2]];
    board [94] = pieces[game[7][2]];
    board [96] = pieces[game[0][1]];
    board [98] = pieces[game[1][1]];
    board[100] = pieces[game[2][1]];
    board[102] = pieces[game[3][1]];
    board[104] = pieces[game[4][1]];
    board[106] = pieces[game[5][1]];
    board[108] = pieces[game[6][1]];
    board[110] = pieces[game[7][1]];
    board[112] = pieces[game[0][0]];
    board[114] = pieces[game[1][0]];
    board[116] = pieces[game[2][0]];
    board[118] = pieces[game[3][0]];
    board[120] = pieces[game[4][0]];
    board[122] = pieces[game[5][0]]; /* ... f1 */
    board[124] = pieces[game[6][0]]; /* g1 */
    board[126] = pieces[game[7][0]]; /* h1 */
    return;
}
