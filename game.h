#ifndef _GAME_H_
#define _GAME_H_

#include "setup.h"
#include "event.h"
#include "render.h"
#include "logic.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_W 1280
#define SCREEN_H 640


int running;
int menu;
int levelNumber;

void initGame();
void mainloop();

#endif
