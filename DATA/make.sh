CFILES="main.c editor.c"
HFILES="editor.h"

gcc -std=c99 $CFILES $HFILES -w -lSDL2 -lSDL2_image -o editor
