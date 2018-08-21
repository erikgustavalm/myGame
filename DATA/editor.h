#ifndef __EDITOR_H_
#define __EDITOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TILE_SIZE 32
#define TILE_CROP_SIZE 32

struct Tile
{
	SDL_Rect crop;
	SDL_Rect pos;
	int endpos;
};

enum Selection {solid, nonsolid, enemy, animated, erase};
enum Selection selected;

int running;
int showCollection;
int mouse_x;
int mouse_y;
int corrx;
int corry;
int camLeft;
int camRight;
int camUp;
int camDown;


SDL_Window* gWindow;
SDL_Renderer* gRender;

SDL_Texture* nonSolidSprite;
SDL_Rect nonSolidWH;
SDL_Texture* solidSprite;
SDL_Rect solidWH;
SDL_Texture* enemySprite;
SDL_Rect enemyWH;
SDL_Texture* animationSprite;
SDL_Rect animationWH;
SDL_Texture* editorSprite;
SDL_Rect eraserSymbol;

SDL_Rect activeTileDest;
SDL_Rect activeTileCrop;




struct Tile* arrayNonSolid;
int nonSolidMaxSize;
int nonSolidSize;

struct Tile* arraySolid;
int solidMaxSize;
int solidSize;

struct Tile* arrayAnimated;
int animatedMaxSize;
int animatedSize;

char filepath[30];



int deleteTiles(int x, int y);

void initEditor(char* level);
void loadTextures();
void createNonSolid(int x, int y, int end, int cx, int cy);
void createSolid(int x, int y, int end, int cx, int cy);
void createAnimated(int x, int y, int speed, int cx, int cy);
void setActiveCrop(int x, int y);
void movement(int x, int y);
void HELLP();
void gameloop();
void update();
void checkevent();
void render();
void saveToFile();
void quit();

#endif
