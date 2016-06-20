var BOARD_WIDTH = 8;
var BOARD_HEIGHT = 8;
var doc;

var white_pawn = 0;
var white_knight = 1;
var white_bishop = 2;
var white_rook = 3;
var white_queen = 4;
var white_king = 5;

var black_pawn = 6;
var black_knight = 7;
var black_bishop = 8;
var black_rook = 9;
var black_queen = 10;
var black_king = 11;

var blank = 12;
var figures = [
    "P", "N", "B", "R", "Q", "K",
    "p", "n", "b", "r", "q", "k", "."
];

function square(file, rank) {
    "use strict";
    var name = [97, 49]; /* ['a', '1'] (Except JavaScript sucks.) */

    name[0] += file;
    name[1] += rank;
    return String.fromCharCode(name[0], name[1]);
}

function write_square(name, ID) {
    "use strict";
    var file_path;

    file_path = "\"images/" + ID + ".gif\"";
    doc.getElementById(name).innerHTML =
            "<img src=" + file_path + "alt=\"" + figures[ID] + "\">";
    return;
}

function JS_main(ML_interface) {
    "use strict";
    var file;
    var rank;

    doc = ML_interface;
    rank = 3 - 1;
    while (rank < BOARD_HEIGHT - 2) {
        file = 0;
        while (file < BOARD_WIDTH) {
            write_square(square(file, rank), blank);
            file += 1;
        }
        rank += 1;
    }

    file = 0;
    while (file < BOARD_WIDTH) {
        write_square(square(file, 0 + 2 - 1), white_pawn);
        write_square(square(file, BOARD_HEIGHT - 2), black_pawn);
        file += 1;
    }

    write_square("a1", white_rook);
    write_square("b1", white_knight);
    write_square("c1", white_bishop);
    write_square("d1", white_queen);
    write_square("e1", white_king);
    write_square("f1", white_bishop);
    write_square("g1", white_knight);
    write_square("h1", white_rook);

    write_square("a8", black_rook);
    write_square("b8", black_knight);
    write_square("c8", black_bishop);
    write_square("d8", black_queen);
    write_square("e8", black_king);
    write_square("f8", black_bishop);
    write_square("g8", black_knight);
    write_square("h8", black_rook);
    return;
}
