#ifndef __EDITOR_H_
#define __EDITOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TILE_SIZE 32;
#define TILE_CROP_SIZE 32;

struct Tile
{
		SDL_Rect crop;
		SDL_Rect pos;
};

int running;
SDL_Window* gWindow;
SDL_Renderer* gRender;

void initEditor(char* level);
void createBg(int x, int y, int cx, int cy);
void createSolid(int x, int y, int cx, int cy);
void gameloop();
void update();
void checkevent();
void render();
void quit();

#endif
