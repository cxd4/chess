int game_flags[4]; /* active color, halfmove clock, fullmove clock, draw data */
char game[8][8] =
{ /* QR    QN    QB     Q     K    KB    KN    KR */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 1 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 2 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 3 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 4 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 5 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 6 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, /* rank 7 */
    0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F }/* rank 8 */;
char board[128] = {
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . .\n"
    ". . . . . . . ." };

void analyze(void);
int load_FEN(char *FEN);
void load_Forsyth(void);

int load_FEN(char *FEN)
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
        char castle[128] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, -1,
            -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 32, -1, -1, -1, -1,
            -1, 32, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
        if (castle[FEN[i]] == -1) return -9;
        game[7][6] |= castle[FEN[i]];
        if (FEN[++i] == ' ') {}
        else
        {
            castle['K'] = -1;
            castle['Q'] = 0x20;
            if (castle[FEN[i]] == -1) return -10;
            game[7][2] |= castle[FEN[i]];
            if (FEN[++i] == ' ') {}
            else
            {
                castle['Q'] = -1;
                castle['k'] = 0x20;
                if (castle[FEN[i]] == -1) return -11;
                game[0][6] |= castle[FEN[i]];
                if (FEN[++i] == ' ') {}
                else
                {
                    if (FEN[i] == 'q')
                    {
                        game[0][2] |= 0x20;
                    }
                    else return -12;
                    if (FEN[++i] == ' ') {}
                    else return -13;
                }
            }
        }
    }

    /* field 4:  en passant target square */
    if (FEN[++i] == '-') {}
    else
    {
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
        file = ep_file[FEN[i]];
        if (file == -1) return -14;
        rank = ep_rank[FEN[++i]];
        if (rank == -1) return -15;
        game[file][rank] |= 0x40; /* en passant destination flag */
    }

    /* field 5:  halfmove clock */
    if (FEN[++i] == ' ') {}
    else return -16;
    do
    {
        if (dec_fig[FEN[++i]] == -1) return -17;
        if (FEN[i] == ' ')
        {
            if (game_flags[1] > 50) return -18;
            break;
        }
        game_flags[1] *= 10;
        game_flags[1] += dec_fig[FEN[i]];
    } while (i == i);

    /* field 6:  fullmove clock */
    do
    {
        if (dec_fig[FEN[++i]] == -1) return -19;
        if (FEN[i] == '\0')
        {
            if (game_flags[2] == 0) return -20;
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
        '*',  -1,  -1,  -1,  -1,  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, '*' };
    register int i = 0, file = 0, rank = 7;
    do
    {
        board[i] = pieces[game[file][rank]];
        i = i + 2;
        if (file == 7)
        {
            if (rank == 0) break;
            file ^= file;
            --rank;
        }
        else
        {
            ++file;
        }
    } while (i == i);
}