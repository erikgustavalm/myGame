#ifndef _RENDER_H_
#define _RENDER_H_

#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window* gWindow;
SDL_Renderer* gRender;

SDL_Texture* playerSprite;
SDL_Texture* nonSolidSprite;
SDL_Texture* solidSprite;
SDL_Texture* menusprite;
SDL_Rect menuPos;

int playerTimer;

int initRender();
int loadTextures();

SDL_Texture* loadText(char* filepath);

void killRender();
void render();
void drawPlayer();
void animatePlayerRunning();
void animatePlayerFalling();
void animatePlayerJumping();
void animatePlayerLongStill();

#endif
