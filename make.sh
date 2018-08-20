CFILES="main.c game.c setup.c event.c logic.c render.c"
HFILES="game.h setup.h event.h logic.h render.h"

gcc -std=c99 $CFILES $HFILES -w -lSDL2 -lSDL2_image -o game
