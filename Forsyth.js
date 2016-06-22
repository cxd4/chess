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

    if (file > BOARD_WIDTH - 1) {
        file = BOARD_WIDTH - 1;
    }
    if (rank > BOARD_HEIGHT - 1) {
        rank = BOARD_HEIGHT - 1;
    }
    name[0] += file;
    name[1] += rank;
    return String.fromCharCode(name[0], name[1]);
}

function write_square(name, ID) {
    "use strict";
    var file_path;

    file_path = "\"engine/images/" + ID + ".gif\"";
    doc.getElementById(name).innerHTML =
            "<img src=" + file_path + "alt=\"" + figures[ID] + "\">";
    return;
}

function write_position(Forsyth) {
    "use strict";
    var i;
    var j;
    var character;
    var code_point;

    var file = 0;
    var rank = BOARD_WIDTH - 1;

    i = 0;
    while (i < Forsyth.length) {
        character = Forsyth.charAt(i);
        code_point = character.charCodeAt(0);
        j = 0;

        if (code_point >= 0x31 && code_point <= 0x38) {
            while (j < code_point - 0x30) {
                write_square(square(file + j, rank), blank);
                j += 1;
            }
            file += j;
        } else if (character === "/") {
            rank -= 1;
            file = 0;
        } else {
            while (j < blank) {
                if (figures[j] === character) {
                    write_square(square(file, rank), j);
                    break;
                }
                j += 1;
            }
            file += 1;
        }
        i += 1;
    }
}

function JS_main(ML_interface) {
    "use strict";
    var f;
    var limit;
    var needle = "f=";
    var Forsyth = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

    doc = ML_interface;
    f = doc.location.href.search(needle);
    if (f >= 0) {
        f += needle.length;
        limit = doc.location.href.length; /* To do:  Adjust if '&' occurs. */
        Forsyth = doc.location.href.substring(f, limit);
    }
    write_position(Forsyth);
    return;
}

var jc;

/* page DOM timer hack to work around synchronous applet loads */
var schedule;
var centiseconds = 0;

function jchess_new_move(move_string) {
    "use strict";
 /* alert(move + ":  " + move_string + "\n" + jc.GetMessageLineText(2)); */
    return (move_string);
}
function jchess_new_game() {
    "use strict";

    jc.SetLevel(1, 60000, 8);
    jc.SetPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    jc.FlipBoard(false);
    jc.StartCompute();

    if (jc.IsBrainBusy() === false) {
        jc.DoMove(1, 34); /* 1.  Nb1-c3 */
    } else {
        jc.MoveNow();
    }
    return;
}

function JVM_main(ML_interface, applet) {
    "use strict";
    var life_type;
    var limit = ML_interface.location.href.length;
    var f = ML_interface.location.href.search("f=");

    doc = ML_interface;
    jc = applet;
    if (f >= 0) {
        f = doc.location.href.substring(f + 2, limit) + " w - - 0 1";
        do {
            try {
                applet.SetPosition(f);
                life_type = -63;
            } catch (error) {
                life_type = +63;
            }
        } while (false);
        if (life_type !== +63 || centiseconds >= 100 * 10) {
            clearInterval(schedule);
            schedule = -1;
        } else {
            centiseconds += 1;
        }
    }
    return;
}
