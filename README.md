Currently a dummy chess engine template that implements all move tests enough to play a full, random game (either human vs. human, White vs. human or human vs. Black).

As such, it does not yet:
* Analyze the board recursively for in-depth strategy (requires node storage/allocation?).
* Apply any analysis to (or predict the consequences of) any of its moves.
* Enumerate pawn under-promotion (analyze promoting to knight, bishop or rook--fundamentally infatuates the directness of the current legal moves detection loop).

It does however appear to play correctly and legally so far every time I've run the program.

It also accepts a custom FEN string in the command-line arguments at startup for beginning play from custom positions (as opposed to the normal one chess usually starts with).
