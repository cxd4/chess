/******************************************************************************\
* Project:  Tournament Pairings Resource Methods                               *
* Release:  2012.09.11                                                         *
* License:  none (public domain)                                               *
* Creator:  R. J. Swedlow                                                      *
\******************************************************************************/

#include <stdio.h>

extern unsigned int count_participants(char *string);
extern unsigned int valid_pairs_count(char *string);
extern unsigned int valid_pairs_show(char *string);
extern void valid_rounds_show(char *string); /* experimental */

unsigned int valid_pairs_show(char *string)
{
    register unsigned int i = 0x0000;
    register unsigned int tables = 0x0000;
    do
    {
        register unsigned int count = i;
        ++count;
        do
        {
            char pairing[8] = "(?, ?)\n";
            pairing[1] = string[i];
            pairing[4] = string[count++];
            printf(pairing);
            ++tables;
        } while (string[count] != '\0');
        ++i;
    } while (string[i + 1] != '\0');
    return (tables);
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
    while (str_len != 0x0001)
    {
        --str_len;
        count += str_len;
    }
    return (count);
}

void valid_rounds_show(char *string)
{
    FILE *stream = fopen("pairings.txt", "w");
    char *buffer;
    register unsigned int i = 0x0000;
    do
    {
        fprintf(stream, "[");
        {
            char *buffer;
            register unsigned int count = i;
            do
            {
                buffer[i] = string[i];
                ++i;
            } while (string[i] != '\0');
            buffer[i] = '\0';

            ++count;
            do
            {
                char pairing[8] = "(?, ?),";
                while (buffer[i] == -1)
                {
                    ++i;
                    if (buffer[i] == '\0')
                    {
                        fprintf(stream, "]");
                        fprintf(stream, ";");
                        fprintf(stream, "\n");
                        fclose(stream);
                        return;
                    }
                }
                pairing[1] = buffer[i];
                buffer[i] = -1;
                while (buffer[count] == -1)
                {
                    ++count;
                    if (buffer[count] == '\0')
                    {
                        fprintf(stream, "]");
                        fprintf(stream, ";");
                        fprintf(stream, "\n");
                        fclose(stream);
                        return;
                    }
                }
                pairing[4] = buffer[count];
                buffer[count] = -1;
                ++count;
                fprintf(stream, pairing);
            } while (count == count);
        } while (0 == 0);
    } while (0 == 0);
}
