#include "logic.h"


void checkCollision()
{
	if (playerCondition == falling) {		
	 
		for (int i = 0; i < sizeSolid; i++) {
			int left = arraySolid[i].pos.x;
			int right = arraySolid[i].pos.x + arraySolid[i].pos.w;
			int top = arraySolid[i].pos.y;
			int playerx = playerPos.x + (playerPos.w / 2);
			int playery = playerPos.y + playerPos.h;

	    
			if (left < playerx) {
				if (playerx < right) {
					if (playery > top - 10) {
						if (playery < top + 10) {
							playerCondition = onground;
							playerPos.y = top - playerPos.h;
						}
					}
				} 
			}	
		}
	}
	
}

void playerJump()
{
	maxHeight = playerPos.y - (abs(playerSpeedX) * 4) - 32;
	playerSpeedY = -15;
	playerCondition = jumping;

}

void standingStillForLong()
{
	if (stillTimer == 100) {
		playerStatus = longstill;
	} else {
		stillTimer++;
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
		standingStillForLong();
	}
	if (playerCondition == onground) {
		playerSpeedY = 0;
		
	}
	if (playerCondition == falling) {
		if (playerSpeedY < 10) {
			playerSpeedY++;
		}
	}
	if (playerCondition == jumping) {
		if (playerPos.y > maxHeight) {
			if (playerSpeedY < -1) {
				playerSpeedY++;
			}
		} else {
			playerCondition = falling;
			playerSpeedY = 0;
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

	stillTimer = 0;
	
	playerSpeedX = 0;
	
	playerCrop.x = 0;
	playerCrop.y = 0;
	playerCrop.w = 64;
	playerCrop.h = 64;

	playerPos.x = 32;
	playerPos.y = 32;
	playerPos.w = 64;
	playerPos.h = 64;
    
	return 1;
}
