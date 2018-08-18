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
	int endpos;
};
enum Selection {solid, nonsolid, enemy};
enum Selection selected;

int running;
int showCollection;

SDL_Window* gWindow;
SDL_Renderer* gRender;

SDL_Texture* nonSolidSprite;
SDL_Rect nonSolidWH;
SDL_Texture* solidSprite;
SDL_Rect solidWH;
SDL_Texture* enemySprite;
SDL_Rect enemyWH;

SDL_Rect activeTileDest;
SDL_Rect activeTileCrop;


struct Tile* arrayNonSolid;
int nonSolidMaxSize;
int nonSolidSize;

struct Tile* arraySolid;
int solidMaxSize;
int solidSize;

char filepath[30];

void initEditor(char* level);
void loadTextures();
void createNonSolid(int x, int y, int end, int cx, int cy);
void createSolid(int x, int y, int end, int cx, int cy);
void gameloop();
void update();
void checkevent();
void render();
void saveToFile();
void quit();

#endif
