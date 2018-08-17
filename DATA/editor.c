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

	while (!feof(file)) {
		int posx = 0;
		int posy = 0;
		int cropx = 0;
		int cropy = 0;
		int type = 0;
		if (fscanf(file, "%d %d %d %d %d\n", &type, &posx, &posy, &cropx, &cropy)) {
			switch(type){
			case 0:
				createBg(posx, posy, cropx, cropy);
				break;
			case 1:
				createSolid(posx, posy, cropx, cropy);
				break;
			default:
				break;
			}
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
			SDL_SetRenderDrawColor(gRender,255,255,255,255);
			gameloop();
		}
	}
	
}

void createBg(int x, int y, int cx, int cy)
{
	
}

void createSolid(int x, int y, int cx, int cy)
{
	
}

void gameloop()
{
	
	running = 1;
	int FPS = 0;

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

	SDL_RenderPresent(gRender);
}

void quit()
{
	SDL_DestroyRenderer(gRender);
	gRender = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();

	printf("editor exited\n");
}
