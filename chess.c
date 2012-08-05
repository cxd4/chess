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
    register unsigned RET_SLOT;

    FEN[92] =  ' '; /* to prevent the next loop from looping indefinitely */
    FEN[93] = '\r'; /* Windows new line format:  CRLF */
    FEN[94] = '\n'; /* '\r' is the carriage return; '\n' is the line feed. */
    FEN[95] = '\0'; /* standard, necessary to prevent indefinite recursion */

    /* field 1:  piece placement data */
    do
    {
        register int char_reg = FEN[i++];
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
        case 'b':
            RET_SLOT = 0x0001;
        case 'w':
            break;
        default:
            return (ERROR);
    }
    ++i;
    if (FEN[i] != ' ')
    {
        return (ERROR);
    }
    ++i;

    /* field 3:  castling */
    if (FEN[i] == '-')
    {
        ++i;
        if (FEN[i] == ' ')
        {
            ++i;
        }
        else
        {
            return (ERROR);
        }
    }
    while (i == i)
    {
        register int char_reg;
        char_reg = FEN[i++];
        switch (char_reg)
        {
            case 'K':
                game[7][6] |= 0x20;
                break;
            case 'Q':
                game[7][2] |= 0x20;
                break;
            case 'k':
                game[0][6] |= 0x20;
                break;
            case 'q':
                game[0][2] |= 0x20;
                break;
            default:
                return (ERROR);
        }
        char_reg = FEN[i++];
        if (char_reg == ' ')
        {
            break;
        }
        switch (char_reg)
        {
            case 'Q':
                game[7][2] |= 0x20;
                break;
            case 'k':
                game[0][6] |= 0x20;
                break;
            case 'q':
                game[0][2] |= 0x20;
                break;
            default:
                return (ERROR);
        }
        char_reg = FEN[i++];
        if (char_reg == ' ')
        {
            break;
        }
        switch (char_reg)
        {
            case 'k':
                game[0][6] |= 0x20;
                break;
            case 'q':
                game[0][2] |= 0x20;
                break;
            default:
                return (ERROR);
        }
        char_reg = FEN[i++];
        if (char_reg == ' ')
        {
            break;
        }
        else if (char_reg == 'q')
        {
            game[0][2] |= 0x20;
        }
        else
        {
            return (ERROR);
        }
    }
    ++i;
    /* to-do:  mask RET_SLOT with en passant, 50 move draw info... */
    return (RET_SLOT);
}
