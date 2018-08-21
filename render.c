#include "render.h"

void render()
{
	SDL_RenderClear(gRender);
	
	if (menu) {
		drawMenu();
	} else {
		drawGame();
	}
	
	SDL_RenderPresent(gRender);
}

void animatePlayerFalling()
{
	playerCrop.x = PLAYER_TILE * 13;
}

void animatePlayerJumping()
{
    // 11 eller 12

	if (playerPos.y > maxHeight + 64 ) {
		playerCrop.x = PLAYER_TILE * 11;
	} else {
		playerCrop.x = PLAYER_TILE * 12;
	}
}

void animatePlayerLongStill()
{

	if (0 < playerTimer && playerTimer < 10) {
		playerCrop.x = PLAYER_TILE * 9;
	}
	if (10 < playerTimer && playerTimer < 12) {
		playerCrop.x = PLAYER_TILE * 10;
	}
	if (12 < playerTimer && playerTimer < 22) {
		playerCrop.x = PLAYER_TILE * 9;
	}
	if (22 < playerTimer && playerTimer < 24) {
		playerCrop.x = PLAYER_TILE * 10;
	}
	if (24 <  playerTimer && playerTimer < 64) {
		playerCrop.x = PLAYER_TILE * 9;
	}
	if (64 < playerTimer && playerTimer < 100) {
		playerCrop.x = 0;
	}
	if (playerTimer == 100) {
		playerTimer = 0;
	}

	playerTimer++;
}

void animatePlayerRunning()
{
	if (playerTimer < 2) {		
		playerTimer++;
	} else {
		playerTimer = 0;

		playerCrop.x += PLAYER_TILE;
	
		if (playerCrop.x >= PLAYER_TILE * 9) {
			playerCrop.x = PLAYER_TILE;
		}
	}
}

void drawPlayer()
{
	if (playerStatus == left) {
		if (playerCrop.y != 64) {
			playerCrop.y = 64;
		}
		animatePlayerRunning();
		
	} else if (playerStatus == right) {
		if (playerCrop.y != 0) {
			playerCrop.y = 0;
		}
		animatePlayerRunning();
		
	} else if (playerStatus == still) {
		playerCrop.x = 0;
	} else if (playerStatus == longstill) {
		animatePlayerLongStill();
	}
	if (playerCondition == falling) {
		animatePlayerFalling();
	} else if (playerCondition == jumping) {
		animatePlayerJumping();
	}
	
	
	
	SDL_RenderCopy(gRender, playerSprite, &playerCrop, &playerPos);
}

void animateObjects()
{
	for (int i = 0; i < sizeAnimated; i++) {
		if (arrayAnimated[i].timer < arrayAnimated[i].speed) {
			arrayAnimated[i].timer++;
		} else {
			arrayAnimated[i].crop.y += arrayAnimated[i].crop.h;
			if (arrayAnimated[i].crop.y == TILE_CROP_H * 5) {
				arrayAnimated[i].crop.y = 0;
			}
			arrayAnimated[i].timer = 0;
		}
		SDL_RenderCopy(gRender, animationSprite, &arrayAnimated[i].crop, &arrayAnimated[i].pos);
	}
}

void drawGame()
{
	for (int i = 0; i < sizeNonSolid; i++) {
		SDL_RenderCopy(gRender, nonSolidSprite, &arrayNonSolid[i].crop, &arrayNonSolid[i].pos);
	}
	for (int i = 0; i < sizeSolid; i++) {
		SDL_RenderCopy(gRender, solidSprite, &arraySolid[i].crop, &arraySolid[i].pos);
	}
	animateObjects();
	drawPlayer();
}

void drawMenu()
{
	SDL_RenderCopy(gRender, menusprite, NULL, &menuPos);
}

SDL_Texture* loadText(char* filepath)
{
	SDL_Surface* loadsurf = IMG_Load(filepath);
	
	if (loadsurf == NULL) {
		printf("%s: %s\n", filepath, SDL_GetError() );
	} else {
		SDL_Texture* new = SDL_CreateTextureFromSurface(gRender, loadsurf);
		if (new == NULL) {
			printf("texture: %s: %s\n", filepath, SDL_GetError());
		} else {
			return new;
		}
	}
}

int loadTextures()
{
	menusprite = loadText("DATA/menu.png");
	if (menusprite == NULL) {
		return 0;
	}
	menuPos.x = 0;
	menuPos.y = 0;
	SDL_QueryTexture(menusprite, NULL, NULL, &menuPos.w, &menuPos.h);

	nonSolidSprite = loadText("DATA/nonsolid.png");
	if (nonSolidSprite == NULL) {
		return 0;
	}

	solidSprite = loadText("DATA/solid.png");
	if (solidSprite == NULL) {
		return 0;
	}

	playerSprite = loadText("DATA/player.png");
	if (playerSprite == NULL) {
		return 0;
	}
	animationSprite= loadText("DATA/animation.png");
	if (animationSprite == NULL) {
		return 0;
	}
	return 1;
}

int initRender()
{
	gWindow = SDL_CreateWindow("myGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							   SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN);
	
	if (gWindow == NULL) {
		printf("gWindow couldn't be created: %s\n", SDL_GetError() );
	} else {
		gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRender == NULL) {
			printf("gRender, couldn't be created: %s\n", SDL_GetError() );
		} else {
			SDL_SetRenderDrawColor(gRender, 255,255,255,255);
			if (loadTextures()) {
				playerTimer = 0;
				return 1;
			}
		}
	}
	return 0;
}

void killRender()
{
	SDL_DestroyTexture(animationSprite);
	animationSprite = NULL;
	SDL_DestroyTexture(playerSprite);
	playerSprite = NULL;
	SDL_DestroyTexture(nonSolidSprite);
	nonSolidSprite = NULL;
	SDL_DestroyTexture(solidSprite);
	solidSprite = NULL;
	SDL_DestroyTexture(menusprite);
	menusprite = NULL;
	
	SDL_DestroyRenderer(gRender);
	gRender = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
}
