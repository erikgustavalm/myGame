#include "editor.h"

void initEditor(char* level)
{
	char filepath[30];
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
	
	while (!feof(file)) {
		int posx = 0;
		int posy = 0;
		int cropx = 0;
		int cropy = 0;
		int type = 0;
		printf("reading line\n");
		if (fscanf(file, "%d %d %d %d %d\n", &type, &posx, &posy, &cropx, &cropy)) {
			switch(type){
			case 0:
				createNonSolid(posx, posy, cropx, cropy);
				break;
			case 1:
				createSolid(posx, posy, cropx, cropy);
				break;
			default:
				break;
			}
		} else {
			printf("reading from file not working\n");
		}
	}
	
	fclose(file);

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

void createNonSolid(int x, int y, int cx, int cy)
{
	if (nonSolidSize > nonSolidMaxSize) {
		nonSolidMaxSize *= 2;
		arrayNonSolid = realloc(arrayNonSolid, sizeof(struct Tile)*nonSolidMaxSize);
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

	arrayNonSolid[nonSolidSize] = new;
	printf("created a nonSolid at: &d - &d\n", x, y);
}

void createSolid(int x, int y, int cx, int cy)
{
	if (solidSize > solidMaxSize) {
		solidMaxSize *= 2;
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

	arraySolid[solidSize] = new;
	printf("created a solid at: &d - &d\n", x, y);
}

void gameloop()
{
	selected = nonsolid;

	loadTextures();
	
	activeTile.x = 0;
	activeTile.y = 0;
	activeTile.w = TILE_CROP_SIZE;
	activeTile.h = TILE_CROP_SIZE;
	
	running = 1;
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
			
			      default:
					  break;
			   }
		    case SDL_MOUSEBUTTONDOWN:
				if (selected == solid ) {
					
				}
				break;
		    default:
				break;
		}
	}
}

void update()
{
	
}

void render()
{
	SDL_RenderClear(gRender);

	for (int i = 0; i < nonSolidSize; i++) {
		SDL_RenderCopy(gRender, nonSolidSprite, &arrayNonSolid[i].crop.x, &arrayNonSolid[i].crop.y);
	}
	
	SDL_RenderPresent(gRender);
}

void loadTextures()
{
	SDL_Surface* loadSurf = IMG_Load("nonsolid.png");
	nonSolidSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);
	SDL_FreeSurface(loadSurf);
}

void quit()
{
	free(arrayNonSolid);
	arrayNonSolid = NULL;
	free(arraySolid);
	arraySolid = NULL;

	SDL_DestroyTexture(nonSolidSprite);
	SDL_DestroyRenderer(gRender);
	gRender = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();

	printf("editor exited\n");
}
