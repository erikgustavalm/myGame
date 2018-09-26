#ifndef __EDITOR_H_
#define __EDITOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TILE_CROP_H 64
#define TILE_CROP_W 128

struct Tile
{
	SDL_Rect crop;
	SDL_Rect pos;
	int endpos;
};

enum Selection {erase, obj, ground};
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

SDL_Texture* groundSprite;
SDL_Rect groundWH;

SDL_Texture* houseSprite;
SDL_Rect houseWH;

SDL_Texture* editorSprite;
SDL_Rect eraserSymbol;
SDL_Rect activeEditorDest;

SDL_Rect activeTileDest;
SDL_Rect activeTileCrop;




struct Tile* arrayGround;
int groundMaxSize;
int groundSize;

struct Tile* arrayObject;
int objectMaxSize;
int objectSize;

char filepath[30];



int deleteTiles(int x, int y);

void initEditor(char* level);
void loadTextures();
void createGround(int x, int y, int cx, int cy);
void createObject(int x, int y, int cx, int cy);
void setActiveCrop(int x, int y);
void setActiveEditorCrop(int x, int y);
void movement(int x, int y);
void HELLP();
void gameloop();
void update();
void checkevent();
void render();
void saveToFile();
void sortObjects();
void quit();

#endif
