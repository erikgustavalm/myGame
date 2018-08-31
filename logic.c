
#include "logic.h"


void checkCollision()
{
	if (playerCondition == onground) {
		for (int i = 0; i < sizeSolid; i++) {
			int left = arraySolid[i].pos.x;
			int right = arraySolid[i].pos.x + arraySolid[i].pos.w;
			int top = arraySolid[i].pos.y;
			int playerx = playerPos.x + (playerPos.w / 2 );
			int playery = playerPos.y + playerPos.h;

			if (playerx - 2 > right || playerx + 2 < left) {
				if (playery == top) {
					playerCondition = falling;
				}
			}
		}
	}
	
	if (playerCondition == falling) {		
	 
		for (int i = 0; i < sizeSolid; i++) {
			int left = arraySolid[i].pos.x;
			int right = arraySolid[i].pos.x + arraySolid[i].pos.w;
			int top = arraySolid[i].pos.y;
			int playerx = playerPos.x + (playerPos.w / 2);
			int playery = playerPos.y + playerPos.h;

	    
			if (left <= playerx) {
				if (playerx <= right) {
					if (playery >= top - 8) {
						if (playery <= top + 8) {
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
	playerSpeedY = 15;
	playerCondition = jumping;
}

void standingStillForLong()
{
	if (stillTimer == 100) {
		playerStatus = longstill;
		stillTimer = 0;
	} else {
		stillTimer++;
	}
}

void playerMovement()
{
	if (playerStatus == left) {
	    if (playerSpeedX > -10 && playerCondition == onground) {
			playerSpeedX--;
		}
		stillTimer = 0;
	}
	if (playerStatus == right) {
		if (playerSpeedX < 10 && playerCondition == onground) {
			playerSpeedX++;
		}
		stillTimer = 0;
	}
	if (playerStatus == still) {
		playerSpeedX = 0;
		standingStillForLong();
	}
	if (playerCondition == onground) {
		playerSpeedY = 0;
		
	}
	if (playerCondition == falling) {
		if (playerSpeedY > -15) {
			playerSpeedY--;			
		}
	}
	if (playerCondition == jumping) {
		if (playerPos.y > maxHeight) {
			if (playerSpeedY > 1) {
				playerSpeedY--;
			}
		} else {
			playerCondition = falling;
			playerSpeedY = -5;
		}
	}
	
	playerPos.y -= playerSpeedY;
	playerPos.x += playerSpeedX;
	
}

void checkIfPlayerMoveCam()
{
	int plx = playerPos.x;
	int ply = playerPos.y;
	int limit = 512;
	
	if (plx <= limit) {
		camx = -playerSpeedX;
		playerPos.x -= playerSpeedX;
	} else if (plx > SCREEN_W - limit) {
		camx = -playerSpeedX;
		playerPos.x -= playerSpeedX;
	} else {
		camx = 0;
	}
	
	/*if (ply <= (limit/4)) {
		camy = playerSpeedY;
		playerPos.y += playerSpeedY;
	} else if (ply > SCREEN_H - (limit/2)) {
		camy = playerSpeedY;
		playerPos.y = -playerSpeedY;
	} else {
		camy = 0;
	}
	*/
	
}

void moveCam()
{
	if (camx != 0 || camy != 0) {
		for (int i = 0; i < sizeSolid; i++) {
			arraySolid[i].pos.x += camx;
		    arraySolid[i].pos.y += camy;
		}
		for (int i = 0; i < sizeNonSolid; i++) {
			arrayNonSolid[i].pos.x += camx;
			arrayNonSolid[i].pos.y += camy;
		}
		for (int i = 0; i < sizeAnimated; i++) {
			arrayAnimated[i].pos.x += camx;
			arrayAnimated[i].pos.y += camy;
		}
	}
    
}

void gameLogic()
{
	checkCollision();
	playerMovement();
	checkIfPlayerMoveCam();
	moveCam();
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

	showMessage = 0;
	
	stillTimer = 0;
	
	playerSpeedX = 5;
	
	playerCrop.x = 0;
	playerCrop.y = 0;
	playerCrop.w = 64;
	playerCrop.h = 64;

	playerPos.x = SCREEN_W / 2;
	playerPos.y = 128;
	playerPos.w = 64;
	playerPos.h = 64;
    
	return 1;
}
