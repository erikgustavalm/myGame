CFILES="main.c game.c setup.c event.c logic.c render.c message.c"
HFILES="game.h setup.h event.h logic.h render.h message.h"

gcc -std=c99 $CFILES $HFILES -w -lSDL2 -lSDL2_image -o game
