#include "editor.h"

void initEditor(char* level)
{
	strcpy(filepath, "levels/");
    strcat(filepath, level);
	FILE* file = fopen(filepath, "a+");

	if (file == NULL) {
		printf("file: %s --couldn't be loaded, exiting...\n", filepath);
		exit(1);
	}else {
		printf("file: %s --loaded\n",filepath);
	}

	nonSolidSize = 0;
	nonSolidMaxSize = 20;
	arrayNonSolid = malloc(sizeof(struct Tile) * nonSolidMaxSize);
	
	solidSize = 0;
	solidMaxSize = 20;
	arraySolid = malloc(sizeof(struct Tile) * solidMaxSize);

	animatedSize = 0;
	animatedMaxSize = 20;
	arrayAnimated = malloc(sizeof(struct Tile) * animatedMaxSize);
	
	while (!feof(file)) {

		int posx = 0;
		int posy = 0;
		int endx = 0;
		int cropx = 0;
		int cropy = 0;
		int type = 0;
		
		if (fscanf(file, "%d %d %d %d %d %d\n", &type, &posx, &posy, &endx, &cropx, &cropy)) {
			switch(type){
				case 0:
					createNonSolid(posx, posy, endx, cropx, cropy);
					break;
				case 1:
					createSolid(posx, posy, endx, cropx, cropy);
					break;
				case 2:
					createAnimated(posx, posy, endx, cropx, cropy);
					break;
				default:
					break;
			}
		} else {
			printf("reading from file not working\n");
		}
	}
	
	fclose(file);

	SDL_Init(SDL_INIT_EVERYTHING);
	
	gWindow = SDL_CreateWindow("EDITOR",
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   1280, 640,
							   SDL_WINDOW_SHOWN);
	
	if (gWindow == NULL) {
		printf("gWindow couldn't be created: %s\n", SDL_GetError() );
	} else {
		gRender = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRender == NULL) {
			printf("gRender couldn't be created: %s\n", SDL_GetError() );
		}else {
			printf("gWindow and gRender setup finish\n");
			SDL_SetRenderDrawColor(gRender,255,255,255,255);
			gameloop();
		}
	}
	
}


void createNonSolid(int x, int y, int end, int cx, int cy)
{
	if (nonSolidSize >= nonSolidMaxSize) {
		nonSolidMaxSize += 10;
		arrayNonSolid = realloc(arrayNonSolid, sizeof(struct Tile)*nonSolidMaxSize);

		if (arrayNonSolid == NULL) {
			printf("arrayNonSolid == NULL\n");
		}
	}
	
	struct Tile new;
	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_SIZE;
	new.crop.h = TILE_CROP_SIZE;

	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_SIZE;
	new.pos.h = TILE_SIZE;

	new.endpos = end;
	
	arrayNonSolid[nonSolidSize] = new;
	nonSolidSize++;
}

void createAnimated(int x, int y, int speed, int cx, int cy)
{
	if (animatedSize == animatedMaxSize) {
		animatedMaxSize += 10;
		arrayAnimated = realloc(arrayAnimated, sizeof(struct Tile) * animatedMaxSize);
		if (arrayAnimated == NULL) {
			printf("arrayAnimated == NULL\n");
		}
	}

	struct Tile new;
	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_SIZE;
	new.crop.h = TILE_CROP_SIZE;

	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_SIZE;
	new.pos.h = TILE_SIZE;

	new.endpos = speed;

	arrayAnimated[animatedSize] = new;
	animatedSize++;
}
void createSolid(int x, int y, int end, int cx, int cy)
{
	if (solidSize >= solidMaxSize) {
		solidMaxSize += 10;
		arraySolid = realloc(arraySolid, sizeof(struct Tile)*solidMaxSize);
	}
	
	struct Tile new;
	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_SIZE;
	new.crop.h = TILE_CROP_SIZE;

	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_SIZE;
	new.pos.h = TILE_SIZE;

	new.endpos = end;
	
	arraySolid[solidSize] = new;
	solidSize++;

}

int deleteTiles(int x, int y)
{
    
	int indexOfTaken = 0;
	int isTaken = 0;
	
// First checking if the position contains a solid then an animated and last nonsolid
	
	for (int i = 0; i < solidSize; i++) {
		if (arraySolid[i].pos.x == x && arraySolid[i].pos.y == y) {
			indexOfTaken = i;
			isTaken = 1;
			break;
		}
	}
	if (isTaken) {
		for (int i = indexOfTaken; i <= solidSize; i++) {
			arraySolid[i] = arraySolid[i+1];
		}
		solidSize--;
		return 0;
	}
//--------------------------------
   	for (int i = 0; i < animatedSize; i++) {
		if (arrayAnimated[i].pos.x == x && arrayAnimated[i].pos.y == y) {
			indexOfTaken = i;
			isTaken = 1;
			break;
		}
	}
	if (isTaken) {
		for (int i = indexOfTaken; i < animatedSize; i++) {
			arrayAnimated[i] = arrayAnimated[i+1];
		}
		animatedSize--;
		return 0;
	}
//---------------------	
	for (int i = 0; i < nonSolidSize; i++) {
		if (arrayNonSolid[i].pos.x == x && arrayNonSolid[i].pos.y == y) {
			indexOfTaken = i;
			isTaken = 1;
			break;
		}
	}
	if (isTaken) {
		for (int i = indexOfTaken; i <= nonSolidSize; i++) {
			arrayNonSolid[i] = arrayNonSolid[i+1];
		}
		nonSolidSize--;
		return 0;
	}



	return 0;
}

void gameloop()
{
	selected = solid;

	loadTextures();

	activeTileDest.w = TILE_SIZE;
	activeTileDest.h = TILE_SIZE;
	
	activeTileCrop.x = 0;
	activeTileCrop.y = 0;
	activeTileCrop.w = TILE_CROP_SIZE;
	activeTileCrop.h = TILE_CROP_SIZE;
	
	running = 1;
	showCollection = 0;

	corrx = 0;
	corry = 0;

	camLeft = 0;
	camRight = 0;
	camUp = 0;
	camDown = 0;
	
	int FPS = 0;

	printf("starting up...\n");
	while (running) {	
		if(FPS + 30 < SDL_GetTicks()){
	
			checkevent();
			update();
			render();		
		    
			FPS = SDL_GetTicks();
		}
	}

	quit();
}

void checkevent()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		    case SDL_QUIT: 
			   running = 0;
			   break;
			   
		    case SDL_KEYDOWN:
			   switch (event.key.keysym.sym) {
			       case SDLK_ESCAPE: 
					   running = 0;
					   break;
					   
				   case SDLK_SPACE:
					   if (showCollection) {
						   showCollection = 0;
						   printf("closing collection\n");
					   } else {
						   showCollection = 1;
						   printf("Showing collection\n");
						   if (selected == erase) {
							   selected = nonsolid;
						   }
					   }
					   break;
					   
				   case SDLK_RETURN:
					   saveToFile();
					   break;
					   
				   case SDLK_j:
					   selected = solid;
					   printf("selected: solid\n");
					   break;
					   
				   case SDLK_k:
					   selected = nonsolid;
					   printf("selected: nonsolid\n");
					   break;
					   
				   case SDLK_l:
					   selected = animated;
					   printf("selected: animated\n");
					   break;

				   case SDLK_i:
					   selected = erase;
					   printf("selected : erase\n");
					   break;
					   
				   case SDLK_w:
					   camUp = 1;
					   break;

				   case SDLK_s:
					   camDown = 1;
					   break;

				   case SDLK_a:
					   camLeft = 1;
					   break;

				   case SDLK_d:
					   camRight = 1;
					   break;

				   case SDLK_h:
					   HELP();
					   break;
			
			      default:
					  break;
			   }
			   break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym) {

					case SDLK_w:
						camUp = 0;
						break;

					case SDLK_s:
						camDown = 0;
						break;

					case SDLK_a: 
						camLeft = 0;	
						break;

					case SDLK_d:
						camRight = 0;
						break;
						
					default:
						break;
				}
				break;
				
		    case SDL_MOUSEBUTTONDOWN:
				if (selected == solid ) {

					if (showCollection) {

						setActiveCrop(activeTileDest.x, activeTileDest.y);
						showCollection = 0;
					}else {
						createSolid(activeTileDest.x, activeTileDest.y, 0, activeTileCrop.x, activeTileCrop.y);
					}
				} else if (selected == nonsolid) {
					
					if (showCollection) {			
						setActiveCrop(activeTileDest.x, activeTileDest.y);
						showCollection = 0;
					}else {
						createNonSolid(activeTileDest.x, activeTileDest.y, 0, activeTileCrop.x, activeTileCrop.y); 
					}
				} else if (selected == animated) {
					if (showCollection) {
						setActiveCrop(activeTileDest.x, activeTileDest.y);
						showCollection = 0;
					}else {
						createAnimated(activeTileDest.x, activeTileDest.y, 2, activeTileCrop.x, activeTileCrop.y);
					}
				} else if (selected == erase) {
					
					if (!showCollection) {
						deleteTiles(activeTileDest.x, activeTileDest.y);
					}
				}
				break;
				
		    default:
				break;
		}
	}
}

void update()
{
	SDL_GetMouseState(&mouse_x, &mouse_y);
	
	int diffx = mouse_x % TILE_CROP_SIZE;
	int diffy = mouse_y % TILE_CROP_SIZE;
   	int newx = mouse_x - diffx;
   	int newy = mouse_y - diffy;
	
	activeTileDest.x = newx;
	activeTileDest.y = newy;

/*
	if (!showCollection) {
		SDL_ShowCursor(SDL_DISABLE);
	}else if (showCollection) {
		SDL_ShowCursor(SDL_ENABLE);
	}
*/
	
	if (camLeft){ 
		movement(TILE_SIZE,0);
		corrx += TILE_SIZE;
		
	}else if (camRight) {

		movement(-TILE_SIZE,0);
		corrx -= TILE_SIZE;
		
	}
	
	if (camUp) {
		
		movement(0,TILE_SIZE);
		corry += TILE_SIZE;
		
	}else if (camDown) {
		
		movement(0,-TILE_SIZE);
		corry -= TILE_SIZE;
		
	}
}

void movement(int x, int y)
{
	for (int i = 0; i < nonSolidSize; i++) {
		arrayNonSolid[i].pos.x += x;
		arrayNonSolid[i].pos.y += y;
		arrayNonSolid[i].endpos += x;
	}
	for (int i = 0; i < solidSize; i++) {
		arraySolid[i].pos.x += x;
		arraySolid[i].pos.y += y;
		arrayNonSolid[i].endpos += x;
	}
	for (int i = 0; i < animatedSize; i++) {
		arrayAnimated[i].pos.x += x;
		arrayAnimated[i].pos.y += y;
	}
}

void HELP()
{

	printf("ActiveTileCrop\n  x: %d\n  y: %d\n  w: %d\n  h: %d\n", activeTileCrop.x, activeTileCrop.y,
		   activeTileCrop.w, activeTileCrop.h);
	printf("ActiveTileDest\n  x: %d\n  y: %d\n  w: %d\n  h: %d\n", activeTileDest.x, activeTileDest.y,
		   activeTileDest.w, activeTileDest.h);
}

void render()
{
	SDL_RenderClear(gRender);

	for (int i = 0; i < nonSolidSize; i++) {
		SDL_RenderCopy(gRender, nonSolidSprite, &arrayNonSolid[i].crop, &arrayNonSolid[i].pos);
	}
	for (int i = 0; i < solidSize; i++) {
		SDL_RenderCopy(gRender, solidSprite, &arraySolid[i].crop, &arraySolid[i].pos);
	}
	for (int i = 0; i < animatedSize; i++) {
		SDL_RenderCopy(gRender, animationSprite, &arrayAnimated[i].crop, &arrayAnimated[i].pos);
	}

	if (showCollection) {
	  
		if (selected == nonsolid) {
			SDL_RenderCopy(gRender, nonSolidSprite, NULL, &nonSolidWH);
		}else if (selected == solid) {
			SDL_RenderCopy(gRender, solidSprite, NULL, &solidWH);
		}else if (selected == animated){
			SDL_RenderCopy(gRender, animationSprite, NULL, &animationWH);
		}
		
	} else {
		
		if (selected == nonsolid) {
			SDL_RenderCopy(gRender, nonSolidSprite, &activeTileCrop, &activeTileDest);
		} else if (selected == solid) {
			SDL_RenderCopy(gRender, solidSprite, &activeTileCrop, &activeTileDest);
		} else if (selected == animated) {
			SDL_RenderCopy(gRender, animationSprite, &activeTileCrop, &activeTileDest);
		} else if (selected == erase) {
			SDL_RenderCopy(gRender, editorSprite, &eraserSymbol, &activeTileDest);
		}
	}
	
	SDL_RenderPresent(gRender);
}

void setActiveCrop(int x, int y)
{
	activeTileCrop.x = x;
	activeTileCrop.y = y;
}

void loadTextures()
{
	SDL_Surface* loadSurf = IMG_Load("nonsolid.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError() );
	}

	nonSolidSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (nonSolidSprite == NULL) {
		printf("%s\n", SDL_GetError() );
	}

	SDL_FreeSurface(loadSurf);
	
//-----------------------------------------

	loadSurf = IMG_Load("solid.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError());
	}

	solidSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (solidSprite == NULL) {
		printf("%s\n", SDL_GetError() );
	}

	SDL_FreeSurface(loadSurf);
	
//------------------------------------------

	loadSurf = IMG_Load("enemy.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError());
	}

	enemySprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (enemySprite == NULL) {
		printf("%s\n", SDL_GetError() );
	}

	SDL_FreeSurface(loadSurf);
	
//------------------------------------------

	loadSurf = IMG_Load("editor.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError());
	}

	editorSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (editorSprite == NULL) {
		printf("%s\n", SDL_GetError() );
	}

	SDL_FreeSurface(loadSurf);
	
//------------------------------------------

	loadSurf = IMG_Load("animation.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError());
	}

	animationSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (animationSprite == NULL) {
		printf("%s\n", SDL_GetError());
	}

	SDL_FreeSurface(loadSurf);
	
//-------------------------------------------
	
	nonSolidWH.x = 0;
	nonSolidWH.y = 0;
	SDL_QueryTexture(nonSolidSprite, NULL, NULL, &nonSolidWH.w, &nonSolidWH.h);

	solidWH.x = 0;
	solidWH.y = 0;
	SDL_QueryTexture(solidSprite, NULL, NULL, &solidWH.w, &solidWH.h);

	enemyWH.x = 0;
	enemyWH.y = 0;
	SDL_QueryTexture(enemySprite, NULL, NULL, &enemyWH.w, &enemyWH.h);

	animationWH.x = 0;
	animationWH.y = 0;
	SDL_QueryTexture(animationSprite, NULL, NULL, &animationWH.w, &animationWH.h);
	
	eraserSymbol.x = 0;
	eraserSymbol.y = 0;
	eraserSymbol.w = TILE_CROP_SIZE;
	eraserSymbol.h = TILE_CROP_SIZE;
}

void saveToFile()
{
	FILE* file = fopen(filepath, "w");
	if (file == NULL) {
		printf("%s:could not be open for saving\n", filepath );
	}else {
	    for (int i = 0; i < nonSolidSize; i++) {
			fprintf(file, "%d %d %d %d %d %d\n", 0, arrayNonSolid[i].pos.x - corrx, arrayNonSolid[i].pos.y - corry,
					arrayNonSolid[i].endpos - corrx, arrayNonSolid[i].crop.x, arrayNonSolid[i].crop.y);
		}
		for (int i = 0; i < solidSize; i++) {
			fprintf(file, "%d %d %d %d %d %d\n", 1, arraySolid[i].pos.x - corrx, arraySolid[i].pos.y - corry,
					arraySolid[i].endpos - corrx, arraySolid[i].crop.x, arraySolid[i].crop.y);
		}
		for (int i = 0; i < animatedSize; i++) {
			fprintf(file, "%d %d %d %d %d %d\n", 2, arrayAnimated[i].pos.x - corrx, arrayAnimated[i].pos.y - corry,
					arrayAnimated[i].endpos, arrayAnimated[i].crop.x, arrayAnimated[i].crop.y);
		}
		fclose(file);
		printf("saved to file: %s\n", filepath);
	}
}

void quit()
{
	free(arrayAnimated);
	arrayAnimated = NULL;
	free(arrayNonSolid);
	arrayNonSolid = NULL;
	free(arraySolid);
	arraySolid = NULL;

	SDL_DestroyTexture(animationSprite);
	animationSprite = NULL;
	SDL_DestroyTexture(editorSprite);
	editorSprite = NULL;
	SDL_DestroyTexture(solidSprite);
	solidSprite = NULL;
	SDL_DestroyTexture(enemySprite);
	enemySprite = NULL;
	SDL_DestroyTexture(nonSolidSprite);
	nonSolidSprite = NULL;
	SDL_DestroyRenderer(gRender);
	gRender = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();

	printf("editor exited\n");
}
