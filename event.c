#include "event.h"


void gameEvent()
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
						menu = 1;
						break;
					case SDLK_h:
						helpFunction();
						break;
					case SDLK_LEFT:
						if (playerStatus == still) {
							playerStatus = left;
						}
						break;
					case SDLK_RIGHT:
						if (playerStatus == still) {
							playerStatus = right;
						}
						break;
					default:
						break;
				}
				break;

			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
					case SDLK_LEFT:
						if (playerStatus == left) {
							playerStatus = still;							
						}
						break;
					case SDLK_RIGHT:
						if (playerStatus == right) {
							playerStatus = still; 
						}
						break;
					default:
						break;
				}
				
			default:
				break;
		}
	}
}

void menuEvent()
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
					case SDLK_RETURN:
						if (option == start) {
							menu = 0;
						}
						break;
					default:
						break;
				}
				break;
				
			default:
				break;
		}
	}
}

void checkEvent()
{
	if (menu) {
		menuEvent();
	} else {
		gameEvent();
	}
}
