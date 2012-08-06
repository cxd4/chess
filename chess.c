#define ERROR   0xFFFF
#define ERRORDW 0xFFFFFFFF
#define WHITE   0
#define BLACK   1
#include<stdio.h>

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

unsigned load_FEN(char FEN[96]);
int main(void)
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
    return 0;
}

unsigned load_FEN(char FEN[96])
{
    register int i = 0; /* array indexing loop variable */
    register int file = 0; /* file A, second dimension of the game[] array */
    register int rank = 7; /* rank 8, first dimension of game array access */
    register int char_reg; /* optimization to use register access instead */

    char pieces[64] = {
        -1, -1, 03, -1, -1, -1, -1, -1, -1, -1, -1, 00, -1, -1, 04, -1,
        05, 01, 02, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, 23, -1, -1, -1, -1, -1, -1, -1, -1, 20, -1, -1, 24, -1,
        25, 21, 22, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
    int ep_file[16] = {-1,0,1,2,3,4,5,6,7,-1,-1,-1,-1,-1,-1,-1};
    int ep_rank[16] = {-1,-1,-1,5,-1,-1,2,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    /* array look-up tables to instantaneously assign conditional integers */

    FEN[92] =  ' '; /* to prevent the next loop from looping indefinitely */
    FEN[93] = '\r'; /* Windows new line format:  CRLF */
    FEN[94] = '\n'; /* '\r' is the carriage return; '\n' is the line feed. */
    FEN[95] = '\0'; /* standard, necessary to prevent indefinite recursion */

    /* field 1:  piece placement */
    do
    {
        char_reg = FEN[i++];
        if ((char_reg & 0xFFF0) == '0')
        { /* number of squares before next piece or end of rank */
            register int square_offset = char_reg & 0x000F;
            if (square_offset == 0)
            {
                return (ERROR); /* not an acceptable value */
            }

            file = file + square_offset;
            if ((file & 0xFFF8) == 0x0000) /* faster than if (int < 8) */
            { /* Meh...BNE should be, like, illegal; why define a negative? */
            } /* This presumes the positive is more likely, so branch out. */
            else
            {
                return (ERROR);
            }
        }
        else
        { /* letter identifying a piece to assign */
            char_reg = pieces[char_reg ^ 0x40];
            if (char_reg == -1)
            {
                return (ERROR);
            }
            game[file++][rank] = (char) char_reg;
        }

        if (file == 8)
        { /* end of the rank */
            if (rank == 0)
            { /* all sixty-four squares of chess board defined */
                if (FEN[i] == ' ')
                { /* required separator between the FEN record data fields */
                    break; /* done with this otherwise infinite loop */
                }
                else
                {
                    return (ERROR);
                }
            } /* else if is useless here, as this is unreachable if rank == 0 */
            if (FEN[i] == '/') /* divider between chessboard ranks */
            {
                file = file ^ file; /* MOV (file, 0) */
                --rank;
            }
            else
            {
                return (ERROR);
            }
        }
    } while (i == i);

    /* field 2:  active color */
    switch (FEN[i])
    {
        case 'b':  game_flags[0] = 1; /* Black's turn to move */
        case 'w':  break;
        default:  return (ERROR);
    }
    ++i;
    if (FEN[i] == ' ')
    {
        ++i;
    }
    else
    {
        return (ERROR);
    }

    /* field 3:  castling availability */
    switch (char_reg = FEN[i])
    {
        case '-':
            ++i;
            if (FEN[i] == ' ')
            {
                break;
            }
            else
            {
                return (ERROR);
            }
        case 'K':
            game[7][6] |= 0x20;
            switch (char_reg = FEN[++i])
            {
                case ' ':
                    break;
                case 'Q':
                    game[7][2] |= 0x20;
                    switch (char_reg = FEN[++i])
                    {
                        case ' ':
                            break;
                        case 'k':
                            game[0][6] |= 0x20;
                            switch (char_reg = FEN[++i])
                            {
                                case ' ':
                                    break;
                                case 'q':
                                    game[0][2] |= 0x20;
                                    if (FEN[++i] == ' ')
                                    {
                                        break;
                                    }
                                default:
                                    return (ERROR);
                            }
                        case 'q':
                            game[0][2] |= 0x20;
                            if (FEN[++i] == ' ')
                            {
                                break;
                            }
                        default:
                            return (ERROR);
                    }
                    break;
                case 'k':
                    game[0][6] |= 0x20;
                    switch (char_reg = FEN[++i])
                    {
                        case ' ':
                            break;
                        case 'q':
                            game[0][2] |= 0x20;
                            if (FEN[++i] == ' ')
                            {
                                break;
                            }
                        default:
                            return (ERROR);
                    }
                case 'q':
                    game[0][2] |= 0x20;
                    if (FEN[++i] == ' ')
                    {
                        break;
                    }
                default:
                    return (ERROR);
            }
            break;
        case 'Q':
            game[7][2] |= 0x20;
            switch (char_reg = FEN[++i])
            {
                case ' ':
                    break;
                case 'k':
                    game[0][6] |= 0x20;
                    switch (char_reg = FEN[++i])
                    {
                        case ' ':
                            break;
                        case 'q':
                            game[0][2] |= 0x20;
                            if (FEN[++i] == ' ')
                            {
                                break;
                            }
                        default:
                            return (ERROR);
                    }
                case 'q':
                    game[0][2] |= 0x20;
                    if (FEN[++i] == ' ')
                    {
                        break;
                    }
                default:
                    return (ERROR);
            }
            break;
        case 'k':
            game[0][6] |= 0x20;
            switch (char_reg = FEN[++i])
            {
                case ' ':
                    break;
                case 'q':
                    game[0][2] |= 0x20;
                    if (FEN[++i] == ' ')
                    {
                        break;
                    }
                default:
                    return (ERROR);
            }
            break;
        case 'q':
            game[0][2] |= 0x20;
            if (FEN[++i] == ' ')
            {
                break;
            }
        default:
            return (ERROR);
    }

    /* field 4:  en passant target square */
    char_reg = FEN[++i] ^ 0x60; /* potential lowercase ASCII letter */
    file = ep_file[char_reg];
    if (file == -1)
    {
        return (ERROR);
    }
    char_reg = FEN[++i] ^ 0x30; /* valid ASCII decimal digit */
    rank = ep_rank[char_reg];
    if (rank == -1)
    {
        return (ERROR);
    }
    game[file][rank] |= 0x40; /* en passant destination flag */

    /* field 5:  halfmove clock */
    if (FEN[++i] == ' ')
    {
        ++i;
    }
    else
    {
        return (ERROR);
    }

    if ((FEN[++i] & 0xF0) == 0x30)
    {
        register int ply = FEN[i] & 0x0F;
        if (ply > 9)
        {
            return (ERROR);
        }

        if (FEN[++i] == ' ')
        {
            game_flags[2] = ply;
            ++i;
        }
        else if ((FEN[i] & 0xF0) == 0x30)
        {
            char_reg = FEN[i] & 0x0F;
            if (char_reg > 9)
            {
                return (ERROR);
            }
            else
            {
                ply = ply * 10;
                ply = ply + char_reg; /* cannot assume:  ply |= char_reg */
                if (ply > 50)
                { /* illegal position:  Fifty Move Draw Rule */
                    return (ERROR);
                }

                if (FEN[++i] == ' ')
                {
                    game_flags[1] = ply;
                    ++i;
                }
                else
                { /* A third digit means it's greater than 50. */
                    return (ERROR);
                }
            }
        }
    }

    /* field 6:  fullmove clock */
    if (FEN[i] == '0')
    {
        return (ERROR);
    }
    else
    {
        int move = 0;
        do
        {
            move = move * 10;
            char_reg = FEN[i];
            if ((char_reg & 0xF0) == 0x30)
            {
                char_reg = char_reg & 0x0F;
                if (char_reg > 9)
                {
                    return (ERROR);
                }
                move = move + char_reg;
                if (FEN[++i] == ' ')
                {
                    break;
                }
            }
            else
            {
                return (ERROR);
            }
        } while (i == i);
        return (move);
    }
}
