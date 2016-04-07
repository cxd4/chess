mkdir -p obj
src="."

FLAGS_C="-masm=intel -pedantic -ansi -Wall"
OBJ_LIST="obj/main.o obj/move.o obj/check.o obj/forsyth.o"

echo Compiling C sources...
cc -S $FLAGS_C -o obj/main.asm    -Os   $src/main.c
cc -S $FLAGS_C -o obj/move.asm    -O3   $src/move.c
cc -S $FLAGS_C -o obj/check.asm   -O3   $src/check.c
cc -S $FLAGS_C -o obj/forsyth.asm -Os   $src/forsyth.c

echo Assembling compiled sources...
as -o obj/main.o                        obj/main.asm
as -o obj/move.o                        obj/move.asm
as -o obj/check.o                       obj/check.asm
as -o obj/forsyth.o                     obj/forsyth.asm

echo Linking assembled objects...
cc -s -o chess $OBJ_LIST -lm
