#include "setup.h"


void helpFunction()
{
	if (playerCondition == onground) {		
		printf("player onground\n");
	}
	playerPos.y = 130;
	playerPos.x = SCREEN_W / 2;
	
}

void createSolid(int x, int y, int end, int cx, int cy)
{
	if (sizeSolid == maxSizeSolid) {
		maxSizeSolid += 10;
		arraySolid = realloc(arraySolid, sizeof(struct Solid) * maxSizeSolid);
		if (arraySolid == NULL) {
			printf("mem realloc for arraySolid failed\n");
		}
	}
	struct Solid new;
	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_W;
	new.pos.h = TILE_H;

	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_W;
	new.crop.h = TILE_CROP_H;

	new.endpos = end;

	arraySolid[sizeSolid] = new;
	sizeSolid++;
}

void createNonSolid(int x, int y, int cx, int cy)
{
	if (sizeNonSolid == maxSizeNonSolid) {
		maxSizeNonSolid += 10;
		arrayNonSolid = realloc(arrayNonSolid, sizeof(struct NonSolid) * maxSizeNonSolid);
		if (arrayNonSolid == NULL) {
			printf("mem realloc for arrayNonSolid failed\n");
		}
	}
	struct NonSolid new;
	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_W;
	new.pos.h = TILE_H;

	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_W;
	new.crop.h = TILE_CROP_H;

	arrayNonSolid[sizeNonSolid] = new;
	sizeNonSolid++;
}

int loadLevel()
{
	char filepathForLevel[20];
	
	switch (levelNumber) {
		case 1:
			strcpy(filepathForLevel, "DATA/levels/level1");
	   		break;
		default:
			break;
	}

	FILE* f = fopen(filepathForLevel, "a+");
	if (f == NULL) {
		printf("levelfile: %s couldn't be loaded\n", filepathForLevel );
		return 0;
	}
	
	
	
	while (!feof(f)) {
		int type = 0;
		int xpos = 0;
		int ypos = 0;
		int endpos = 0;
		int cropx = 0;
		int cropy = 0;
		if (fscanf(f, "%d %d %d %d %d %d\n", &type, &xpos, &ypos, &endpos, &cropx, &cropy)) {
			switch (type) {
				case 0:
					createNonSolid(xpos, ypos, cropx, cropy);
					break;
				case 1:
					createSolid(xpos, ypos, endpos, cropx, cropy);
					break;
				default:
					break;
			}
		}
	}

	fclose(f);

	levelLoaded = 1;
	return 1;
}

int setupSDL()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	levelLoaded = 0;
	
	if (initRender() && initLogic()) {
		sizeNonSolid = 0;
		maxSizeNonSolid = 30;
		arrayNonSolid = malloc(sizeof(struct NonSolid) * maxSizeNonSolid);
		if (arrayNonSolid == NULL) {
			printf("init mem alloc for arrayNonSolid failed\n");
		}

		sizeSolid = 0;
		maxSizeSolid = 30;
		arraySolid = malloc(sizeof(struct Solid) * maxSizeSolid);
		if (arraySolid == NULL) {
			printf("init mem alloc arraySolid failed\n");
		}
		
		return 1;
	}
	
	return 0;
	
}

void setupQuit()
{
	free(arrayNonSolid);
	arrayNonSolid = NULL;

	free(arraySolid);
	arraySolid = NULL;
	
	killRender();

	SDL_Quit();
}
