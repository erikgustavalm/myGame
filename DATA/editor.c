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

	groundSize = 0;
	groundMaxSize = 20;
	arrayGround = malloc(sizeof(struct Tile) * groundMaxSize);

	objectSize = 0;
	objectMaxSize = 20;
	arrayObject = malloc(sizeof(struct Tile) * objectMaxSize);
	
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
					createGround(posx, posy, cropx, cropy);
					break;
				case 1:
					createObject(posx, posy, cropx, cropy);
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

void createObject(int x, int y, int cx, int cy)
{
	if (objectSize >= objectMaxSize) {
		objectMaxSize += 10;
		arrayObject = realloc(arrayObject, sizeof(struct Tile) * objectMaxSize);
	}

	struct Tile new;
	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_W;
	new.crop.h = TILE_CROP_H * 2;

	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_CROP_W;
	new.pos.h = TILE_CROP_H * 2;

	arrayObject[objectSize] = new;
	objectSize++;
}
void createGround(int x,int y,int cx,int cy)
{
	if (groundSize >= groundMaxSize) {
		groundMaxSize += 10;
		arrayGround = realloc(arrayGround, sizeof(struct Tile) * groundMaxSize);
	}

	struct Tile new;
	new.crop.x = cx;
	new.crop.y = cy;
	new.crop.w = TILE_CROP_W;
	new.crop.h = TILE_CROP_H;

	new.pos.x = x;
	new.pos.y = y;
	new.pos.w = TILE_CROP_W;
	new.pos.h = TILE_CROP_H;

	arrayGround[groundSize] = new;
	groundSize++;
}

int deleteTiles(int x, int y)
{
    
	int indexOfTaken = 0;
	int isTaken = 0;
	
// First checking if the position contains a solid then an animated and last nonsolid
	
	for (int i = 0; i < groundSize; i++) {
		if (arrayGround[i].pos.x == x && arrayGround[i].pos.y == y) {
			indexOfTaken = i;
			isTaken = 1;
			printf("deleting tile at x:%d - y:%d\n", arrayGround[i].pos.x, arrayGround[i].pos.y);
			break;
		}
	}
	if (isTaken) {
		for (int i = indexOfTaken; i <= groundSize; i++) {
			arrayGround[i] = arrayGround[i+1];
		}
		groundSize--;
		return 1;
	}
	for (int i = 0; i < objectSize; i++) {
		if (arrayObject[i].pos.x == x && arrayObject[i].pos.y == y) {
			indexOfTaken = i;
			isTaken = 1;
			printf("deleting tile at x:%d - y:%d\n", arrayObject[i].pos.x, arrayObject[i].pos.y);
			break;
		}
	}
	if (isTaken) {
		for (int i = indexOfTaken; i <= objectSize; i++) {
			arrayObject[i] = arrayObject[i+1];
		}
		objectSize--;
		return 1;
	}

	return 0;
}

void gameloop()
{
	selected = ground;

	loadTextures();

	activeTileDest.w = TILE_CROP_W;
	activeTileDest.h = TILE_CROP_H;
	
	activeTileCrop.x = 0;
	activeTileCrop.y = 0;
	activeTileCrop.w = TILE_CROP_W;
	activeTileCrop.h = TILE_CROP_H;
	
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
							   selected = ground;
						   }
					   }
					   break;
					   
				   case SDLK_RETURN:
					   saveToFile();
					   break;
					   
				   case SDLK_i:
					   selected = erase;
					   printf("selected: erase\n");
					   break;

				   case SDLK_g:
					   selected = ground;
					   printf("selected: ground\n");
					   break;

				   case SDLK_o:
					   selected = obj;
					   printf("selected: object\n");
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
			    if (selected == erase) {				 
					if (!showCollection) {
						deleteTiles(activeTileDest.x, activeTileDest.y);
					}
				} else if (selected == ground) {
					if (showCollection) {
						setActiveCrop(activeTileDest.x, activeTileDest.y);
						showCollection = 0;
					} else {
						if (deleteTiles(activeTileDest.x, activeTileDest.y) == 1) {
							
						} else {
							createGround(activeTileDest.x, activeTileDest.y, activeTileCrop.x, activeTileCrop.y);	
						}						
					}
				} else if (selected == obj) {
					if (showCollection) {
						setActiveCrop(activeTileDest.x, activeTileDest.y);
						showCollection = 0;					
					} else {
						if (deleteTiles(activeTileDest.x, activeTileDest.y) == 1) {
							
						} else {
							createObject(activeTileDest.x, activeTileDest.y, activeTileCrop.x, activeTileCrop.y);
						}
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

	int diffx = 0;
	int diffy = 0;
	
	if (showCollection) {
		diffx = mouse_x % TILE_CROP_W;
		diffy = mouse_y % TILE_CROP_H;		
		SDL_ShowCursor(SDL_ENABLE);
	} else {
		SDL_ShowCursor(SDL_DISABLE);
		diffx = mouse_x % (TILE_CROP_W / 4);
		diffy = mouse_y % (TILE_CROP_H / 4);
	}

   	int newx = mouse_x - diffx;
   	int newy = mouse_y - diffy;
	
	activeTileDest.x = newx;
	activeTileDest.y = newy;

	
	
	if (camLeft){ 
		movement(TILE_CROP_W,0);
		corrx += TILE_CROP_W;
		
	}else if (camRight) {

		movement(-TILE_CROP_W,0);
		corrx -= TILE_CROP_W;
		
	}
	
	if (camUp) {
		
		movement(0,TILE_CROP_H);
		corry += TILE_CROP_H;
		
	}else if (camDown) {
		
		movement(0,-TILE_CROP_H);
		corry -= TILE_CROP_H;
		
	}
}

void movement(int x, int y)
{
	for (int i = 0; i < groundSize; i++) {
		arrayGround[i].pos.x += x;
		arrayGround[i].pos.y += y;
	}
	for (int i = 0; i < objectSize; i++) {
		arrayObject[i].pos.x += x;
		arrayObject[i].pos.y += y;
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

	for (int i = 0; i < groundSize; i++) {
		SDL_RenderCopy(gRender, groundSprite, &arrayGround[i].crop, &arrayGround[i].pos);
	}
	for (int i = 0; i < objectSize; i++) {
		SDL_RenderCopy(gRender, houseSprite, &arrayObject[i].crop, &arrayObject[i].pos);
	}
	
	if (showCollection) {
	  
	    if (selected == ground){
			SDL_RenderCopy(gRender, groundSprite, NULL, &groundWH);
		} else if (selected == obj) {
			SDL_RenderCopy(gRender, houseSprite, NULL, &houseWH);
		}
		
	} else {
		
	    if (selected == erase) {
			SDL_RenderCopy(gRender, editorSprite, &eraserSymbol, &activeTileDest);
		} else if (selected == ground) {
			SDL_RenderCopy(gRender, groundSprite, &activeTileCrop, &activeTileDest);
		} else if (selected == obj) {
			SDL_RenderCopy(gRender, houseSprite, &activeTileCrop, &activeTileDest);
		}
	}
	
	SDL_RenderPresent(gRender);
}

void setActiveCrop(int x, int y)
{
	activeTileCrop.x = x;
	activeTileCrop.y = y;
}

void setActiveEditorCrop(int x, int y)
{
	activeEditorDest.x = x;
	activeEditorDest.y = y;
}

void loadTextures()
{
	SDL_Surface* loadSurf = IMG_Load("iso.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError());
	}
	
	groundSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (groundSprite == NULL) {
		printf("%s\n", SDL_GetError());
	}

	SDL_FreeSurface(loadSurf);

//-------------------------------------------	

	loadSurf = IMG_Load("tree.png");

	if (loadSurf == NULL) {
		printf("%s\n", SDL_GetError());
	}

	houseSprite = SDL_CreateTextureFromSurface(gRender, loadSurf);

	if (houseSprite == NULL) {
		printf("%s\n", SDL_GetError());
	}
	
	SDL_FreeSurface(loadSurf);

	houseWH.x = 0;
	houseWH.y = 0;
	SDL_QueryTexture(houseSprite, NULL, NULL, &houseWH.w, &houseWH.h);
	
	groundWH.x = 0;
	groundWH.y = 0;
	SDL_QueryTexture(groundSprite, NULL, NULL, &groundWH.w, &groundWH.h);
	
	
	eraserSymbol.x = 0;
	eraserSymbol.y = 0;
	eraserSymbol.w = 32;
	eraserSymbol.h = 32;
}

void saveToFile()
{

	sortObjects();
	FILE* file = fopen(filepath, "w");
	if (file == NULL) {
		printf("%s:could not be open for saving\n", filepath );
	}else {
		for (int i = 0; i < groundSize; i++) {
			fprintf(file, "0 %d %d 0 %d %d\n", arrayGround[i].pos.x - corrx, arrayGround[i].pos.y - corry,
					arrayGround[i].crop.x, arrayGround[i].crop.y);
		}
		for (int i = 0; i < objectSize; i++) {
			fprintf(file, "1 %d %d 0 %d %d\n", arrayObject[i].pos.x - corrx, arrayObject[i].pos.y - corry,
					arrayObject[i].crop.x, arrayObject[i].crop.y);
		}
		fclose(file);
		printf("saved to file: %s\n", filepath);
	}
}

void sortObjects()
{
    
	for (int i = 0; i < objectSize; i++) {
		for (int j = 0; j < objectSize; j++) {
			if (arrayObject[i].pos.y < arrayObject[j].pos.y) {

				int tmp = arrayObject[i].pos.y;
				arrayObject[i].pos.y = arrayObject[j].pos.y;
				arrayObject[j].pos.y = tmp;
			}
		}
	}
}

void quit()
{

	free(arrayGround);
	arrayGround = NULL;
	
	free(arrayObject);
	arrayObject = NULL;

	SDL_DestroyTexture(houseSprite);
	houseSprite = NULL;
	
	SDL_DestroyTexture(groundSprite);
	groundSprite = NULL;

	SDL_DestroyTexture(editorSprite);
	editorSprite = NULL;

	SDL_DestroyRenderer(gRender);
	gRender = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();

	printf("editor exited\n");
}
