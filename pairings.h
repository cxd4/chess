/******************************************************************************\
* Project:  Tournament Pairings Resource Methods                               *
* Release:  2012.09.11                                                         *
* License:  none (public domain)                                               *
* Creator:  R. J. Swedlow                                                      *
\******************************************************************************/

#include <stdio.h>

extern unsigned int count_participants(char *string);
extern unsigned int valid_pairs_count(char *string);
extern unsigned int valid_pairs_count_Thibault(char *string); /* his idea xD */
extern void valid_pairs_show(char *string);

void valid_pairs_show(char *figures)
{
    register unsigned int i = 0x0000;
    do
    {
        register unsigned int count = i;
        ++count;
        do
        {
            char pairing[8] = "(?, ?)\n";
            pairing[1] = figures[i];
            pairing[4] = figures[count++];
            printf(pairing);
        } while (figures[count] != '\0');
        ++i;
    } while (figures[i + 1] != '\0');
}

unsigned int count_participants(char *string)
{
    register unsigned int i = 0x0000;
    do
    {
        if (string[i] == '\0') return (i);
        ++i;
        continue;
    } while (i == i);
}

unsigned int valid_pairs_count(char *string)
{
    register unsigned int str_len = count_participants(string);
    register unsigned int count = 0x0000;
    while (str_len != 0)
    {
        --str_len; /* (n - 1) */
        count += str_len;
    }
    return (count);
}

unsigned int valid_pairs_count_Thibault(char *string)
{
    register unsigned int str_len = count_participants(string);
    if (str_len = 0x0000) return 0;
    else
    {
        register unsigned int count = 0x0001;
        register unsigned int inc = 0x0001;
        while (inc < str_len)
        {
            inc += 2;
            count *= inc;
        }
        return (count);
    }
}
