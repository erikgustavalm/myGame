#include "logic.h"


void checkCollision()
{
	for (int i = 0; i < sizeSolid; i++) {
		if (arraySolid[i].pos.y - 10 < playerPos.y + PLAYER_TILE < arraySolid[i].pos.y + 10) {
			if (arraySolid[i].pos.x < playerPos.x + (PLAYER_TILE/2) < arraySolid[i].pos.x + arraySolid[i].pos.w) {
				playerCondition = onground;
			}
		}
	}
}
void playerMovement()
{
	if (playerStatus == left) {
	    if (playerSpeedX > -10) {
			playerSpeedX--;
		}
	}
	if (playerStatus == right) {
		if (playerSpeedX < 10) {
			playerSpeedX++;
		}
	}
	if (playerStatus == still) {
		playerSpeedX = 0;
	}
	if (playerCondition == onground) {
		playerSpeedY = 0;
	}
	if (playerCondition == falling) {
		if (playerSpeedY < 10) {
			playerSpeedY++;
		}
	}

	playerPos.y += playerSpeedY;
	playerPos.x += playerSpeedX;
}

void gameLogic()
{
	playerMovement();
	checkCollision();
}

void menuLogic()
{
	
}

void updateLogic()
{
	if (menu) {
		menuLogic();
	} else {
		gameLogic();
	}	
	
}

int initLogic()
{
	option = start;
	playerStatus = still;
	playerCondition = falling;
	
	playerSpeedX = 0;
	
	playerCrop.x = 0;
	playerCrop.y = 0;
	playerCrop.w = 64;
	playerCrop.h = 64;

	playerPos.w = 64;
	playerPos.h = 64;
    
	return 1;
}
