#ifndef _CHECK_H_
#define _CHECK_H_

#include "board.h"
#include "pieces.h"

/*
 * needed in case of checking an unoccupied square for control by a color
 */
#include "forsyth.h"

extern int in_check(int player);
extern int get_player_by_square(int file, int rank);

/*
 * Does any piece that can move like a Queen threaten this square?
 * (Rooks and bishops can also move like queens, so they also pass.)
 */
extern int test_Queen(int file, int rank);

/*
 * Does any piece that can move like a rook threaten this square?
 * (Queens also move like rooks, but this test is a faster subset.)
 */
extern int test_rook(int file, int rank);

/*
 * Does any piece that can move like a bishop threaten this square?
 * (Queens also move like bishops, but this test is a faster subset.)
 */
extern int test_bishop(int file, int rank);

/*
 * Do any pawns threaten this square?
 * (Due to the risk of non-parallel algorithm, only pawn threats count.)
 */
extern int test_pawn(int file, int rank);

/*
 * Does any piece (or pawn) that can move like a King threaten this square?
 */
extern int test_King(int file, int rank);

/*
 * Can any knights move to this square?
 */
extern int test_knight(int file, int rank);

#endif
