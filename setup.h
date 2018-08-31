#ifndef _SETUP_H_
#define _SETUP_H_

#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>

#define TILE_W 32
#define TILE_H 32
#define TILE_CROP_W 32
#define TILE_CROP_H 32

struct Solid
{
	SDL_Rect pos;
	SDL_Rect crop;
	int endpos;
};

struct NonSolid
{
	SDL_Rect pos;
	SDL_Rect crop;

};

struct Animated
{
	SDL_Rect pos;
	SDL_Rect crop;
	int timer;
	int speed;
};

struct NonSolid messageBox;

struct Animated* arrayAnimated;
int sizeAnimated;
int maxSizeAnimated;

struct Solid* arraySolid;
int sizeSolid;
int maxSizeSolid;

struct NonSolid* arrayNonSolid;
int sizeNonSolid;
int maxSizeNonSolid;

int levelLoaded;

int setupSDL();
int loadLevel();

void helpFunction();
void setupQuit();
void createNonSolid(int x, int y, int cx, int cy);
void createSolid(int x, int y, int end, int cx, int cy);
void createAnimated(int x, int y, int speed, int cx, int cy);

void initMessageBox();

#endif
