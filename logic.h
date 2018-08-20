#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define PLAYER_TILE 64

enum Option {start, quit};
enum Option option;
enum PlayerStatus {left, right, still};
enum PlayerStatus playerStatus;
enum PlayerCondition {onground, jumping, falling};
enum PlayerCondition playerCondition;

SDL_Rect playerPos;
SDL_Rect playerCrop;

int playerSpeedX;
int playerSpeedY;

int initLogic();

void playerMovement();
void checkCollision();
void updateLogic();
void gameLogic();
void menuLogic();

#endif
