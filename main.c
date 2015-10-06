#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "forsyth.h"

#include "move.h"
#include "check.h"

move_storage * move_list = NULL;

/*
 * string input buffer to store human player chess move inputs
 *
 * should be big enough to store requests like "Nd4xb5+"
 * and possibly some special string commands as well like "undo"
 */
#define KEY_READ_BUFFER_SIZE    16
static char human_response[KEY_READ_BUFFER_SIZE];

int main(int argc, char* argv[])
{
    FILE * stream;
    unsigned long move_number;
    long who_plays_who;
    int FEN_status;
    int legal_moves;

    load_LUTs();
    if (argc < 2)
        FEN_status = load_FEN(
            "rnbqkbnr/"\
            "pppppppp/"\
            "8/"\
            "8/"\
            "8/"\
            "8/"\
            "PPPPPPPP/"\
            "RNBQKBNR "

            "w KQkq - 0 1"
        );
    else
        FEN_status = load_FEN(argv[1]);

    if (FEN_status != FEN_OK)
    {
        fprintf(stderr, "(%i):  FEN record is corrupt!\n\n", FEN_status);
        return (FEN_status);
    }
    stream = (argc < 3) ? stdout : fopen(argv[2], "w");

    load_Forsyth(stream);
    puts("Is it my turn (1) or yours (0), or are you playing a human (-1)?");
    while (fgets(&human_response[0], KEY_READ_BUFFER_SIZE, stdin) == NULL);
    who_plays_who = strtol(&human_response[0], NULL, 0);

    srand(
        time(NULL)
    );
    if (who_plays_who > 0) {
        puts("I'll start play with...");
        legal_moves = enum_moves(&move_list);
        if (legal_moves > 0) {
            move_number = rand() % legal_moves;
            execute_legal_move_by_ID(move_list, move_number);
        }
    }

    while ((legal_moves = enum_moves(&move_list)) > 0) {
        load_Forsyth(stream);
        printf("Total of %i legal moves.\n", legal_moves);
        show_moves(move_list, legal_moves);

        do {
            fputs("Your move:  ", stdout);
            fgets(&human_response[0], KEY_READ_BUFFER_SIZE, stdin);
            move_number = strtoul(&human_response[0], NULL, 0);
        } while (move_number != 0);
        --move_number; /* Convert one-based to zero-based index. */
        execute_legal_move_by_ID(move_list, move_number);

        if (who_plays_who < 0)
            continue;
        legal_moves = enum_moves(&move_list);
        if (legal_moves <= 0)
            break;
        execute_legal_move_by_ID(move_list, rand() % legal_moves);
    }

    load_Forsyth(stream);
    printf(
        "%smate.  That's the end!\n",
        in_check(game_state.player_turn) ? "Check" : "Stale"
    );
    if (stream != stdout)
        fclose(stream);
    return 0;
}
