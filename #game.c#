#include "game.h"

void initGame()
{
	running = setupSDL();
	menu = 1;
	levelNumber = 1;
	
	if ( running) {
		int fps = 0;
		
		while (running) {
			if (!menu && !levelLoaded) {
			    if (!loadLevel()) {
					break;
				}
			}
			if (fps + 30 < SDL_GetTicks()) {		   	
				mainloop();
				fps = SDL_GetTicks();
			}
		}
		setupQuit();
	}    	
}

void mainloop()
{
   	updateLogic();
   	checkEvent();
   	render();

}
