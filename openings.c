#include "openings.h"

static const move_storage startclassic[] = {
    { {g,1},{f,3} },
    { {e,2},{e,4} },
};
static const move_storage op_QP[] = {
    { {d,7},{d,5} },
    { {g,8},{f,6} },
};
static const move_storage op_KP[] = {
    { {e,7},{e,5} },
    { {e,7},{e,6} },
};
static const move_storage op_KKt[] = {
    { {g,8},{f,6} },
    { {d,7},{d,5} },
};

static const op_pos op_moves[] = {
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        {{g,1},{f,3}}, 1, &startclassic[0]
    }, { /* A00:  Grob's Attack (1.g4) */
        "rnbqkbnr/pppppppp/8/8/6P1/8/PPPPPP1P/RNBQKBNR b KQkq - 0 1",
        {{d,7},{d,5}},
    }, { /* A00:  Amar Opening (1.Nh3) */
        "rnbqkbnr/pppppppp/8/8/8/7N/PPPPPPPP/RNBQKB1R b KQkq - 0 1",
        {{d,7},{d,5}},
    }, { /* A00:  Barnes Opening (1.f3) */
        "rnbqkbnr/pppppppp/8/8/8/5P2/PPPPP1PP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Desprez Opening (1.h4) */
        "rnbqkbnr/pppppppp/8/8/7P/8/PPPPPPP1/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Ware Opening (1.a4) */
        "rnbqkbnr/pppppppp/8/8/P7/8/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Durkin's Opening (1.Na3) */
        "rnbqkbnr/pppppppp/8/8/8/N7/PPPPPPPP/R1BQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Clemenz Opening (1.h3) */
        "rnbqkbnr/pppppppp/8/8/8/7P/PPPPPPP1/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Anderssen's Opening (1.a3) */
        "rnbqkbnr/pppppppp/8/8/8/P7/1PPPPPPP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Saragossa Opening (1.c3) */
        "rnbqkbnr/pppppppp/8/8/8/2P5/PP1PPPPP/RNBQKBNR b KQkq - 0 1",
        {{d,7},{d,5}},
    }, { /* A00:  Mieses Opening (1.d3) */
        "rnbqkbnr/pppppppp/8/8/8/3P4/PPP1PPPP/RNBQKBNR b KQkq - 0 1",
        {{d,7},{d,5}},
    }, { /* A00:  Van't Kruijs Opening (1.e3) */
        "rnbqkbnr/pppppppp/8/8/8/4P3/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
        {{g,7},{g,6}},
    },

    { /* A00:  Sokolsky Opening (1.b4) */
        "rnbqkbnr/pppppppp/8/8/1P6/8/P1PPPPPP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A00:  Dunst Opening (1.Nc3) */
        "rnbqkbnr/pppppppp/8/8/8/2N5/PPPPPPPP/R1BQKBNR b KQkq - 0 1",
        {{d,7},{d,5}},
    }, { /* A00:  Benko's Opening (1.g3) */
        "rnbqkbnr/pppppppp/8/8/8/6P1/PPPPPP1P/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A01:  Larsen's Attack (1.b3) */
        "rnbqkbnr/pppppppp/8/8/8/1P6/P1PPPPPP/RNBQKBNR b KQkq - 0 1",
        {{g,8},{f,6}},
    }, { /* A02:  Bird's Opening (1.f4) */
        "rnbqkbnr/pppppppp/8/8/5P2/8/PPPPP1PP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A10:  Staunton's Opening (1.c4) */
        "rnbqkbnr/pppppppp/8/8/2P5/8/PP1PPPPP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}},
    }, { /* A40:  Queen's Pawn Opening (1.d4) */
        "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1",
        {{d,7},{d,5}}, 2, &op_QP[0]
    }, { /* B00:  King's Pawn Opening (1.e4) */
        "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1",
        {{e,7},{e,5}}, 2, &op_KP[0]
    }, { /* A04:  Zukertort System (1.Nf3) */
        "rnbqkbnr/pppppppp/8/8/8/5N2/PPPPPPPP/RNBQKB1R b KQkq - 0 1",
        {{g,8},{f,6}}, 2, &op_KKt[0]
    }
};
