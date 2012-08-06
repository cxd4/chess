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
    register int file = 000; /* file A, second dimension of the game[] array */
    register int rank = 111; /* rank 8, first dimension of game array access */
    register int char_reg; /* optimization to use register access instead */

    FEN[92] =  ' '; /* to prevent the next loop from looping indefinitely */
    FEN[93] = '\r'; /* Windows new line format:  CRLF */
    FEN[94] = '\n'; /* '\r' is the carriage return; '\n' is the line feed. */
    FEN[95] = '\0'; /* standard, necessary to prevent indefinite recursion */

    /* field 1:  piece placement */
    do
    {
        char_reg = FEN[i++];
        if (char_reg == ' ')
        {
            if (rank == 000)
            { /* all eight rows of chessboard defined */
                break;
            }
            return (ERROR); /* incomplete position */
        }

        if ((char_reg & 0x0030) == '0')
        {
            register unsigned square_offset = char_reg & 0x000F;
            if (square_offset == 0)
            {
                return (ERROR); /* not an acceptable value */
            }
            if (square_offset > 8)
            {  
                return (ERROR);
            }

            if (square_offset == 8)
            {
                if (file == 000)
                {
                    if (rank == 000)
                    {
                        return (ERROR);
                    }
                    --rank;
                }
                else
                {
                    return (ERROR);
                }
            }
            else
            {
                file = file + square_offset;
                if (file >> 3 == 0x0000) /* faster than:  if (int < 8) */
                {
                }
                else
                {
                    return (ERROR);
                }
            }
        }
        else if (char_reg == '/') /* divider between chessboard ranks */
        {
            --rank;
            if (rank == 0xFFFF) /* imaginary rank after integer underflow */
            {
                return (ERROR);
            }
        }
        else
        {
            switch (char_reg & 0x00DF) /* case conversion to uppercase */
            { /* I should convert this to a lookup table some time. */
                case 'B':
                    game[file][rank] = (char_reg & 0x0020) ? 0x13 : 0x03;
                    break;
                case 'K':
                    game[file][rank] = (char_reg & 0x0020) ? 0x10 : 0x00;
                    break;
                case 'N':
                    game[file][rank] = (char_reg & 0x0020) ? 0x14 : 0x04;
                    break;
                case 'P':
                    game[file][rank] = (char_reg & 0x0020) ? 0x15 : 0x05;
                    break;
                case 'Q':
                    game[file][rank] = (char_reg & 0x0020) ? 0x11 : 0x01;
                    break;
                case 'R':
                    game[file][rank] = (char_reg & 0x0020) ? 0x12 : 0x02;
                    break;
                default:
                    return (ERROR);
            } /* inefficient and unoptimized */
        } /* I should use an array lookup method to replace a linear switch. */
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
    ++i;

    /* field 4:  en passant target square */
    if ((FEN[i] & 0xF0) == 0x60) /* valid ASCII symbol from '`' to 'o' */
    {
        file = FEN[i] & 0x0F;
        if (file == 0) /* imaginary file, to the left of file A */
        {
            return (ERROR);
        }
        --file; /* 0 through 7 instead of 1 through 8 for files 'a' to 'h' */
        if (file > 7)
        {
            return (ERROR);
        }
    }
    switch (FEN[++i])
    { /* I know there is an easy way to optimize this into a look-up table. */
        case '3':
            rank = 5; /* from Black's view of the board */
            break;
        case '6':
            rank = 2; /* from Black's view of the board */
            break;
        default:
            return (ERROR);
    } /* ... very soon, sleep first =D */
    game[file][rank] |= 0x40; /* en passant destination flag */
    if (FEN[++i] == ' ')
    {
        ++i;
    }
    else
    {
        return (ERROR);
    }

    /* field 5:  halfmove clock */
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
