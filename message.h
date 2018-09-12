#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define FONT_W 10
#define FONT_H 16

#define ALPHA_SIZE 30

struct myChar
{
	SDL_Rect pos;
	SDL_Rect crop;
};

struct myChar* messageArray;
int messageSize;
int messageMaxSize;



void showMsg(char* message, int messLen);
void stopShowMsg();
void addCharToArray(char c);

#endif
