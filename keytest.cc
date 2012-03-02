#include "interface.h"
int main( int argc, char* argv[] )
{
	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);

	/*WM stuff blah blah*/
	SDL_WM_SetCaption("GUFG", "GUFG");

	/*Make a window*/
	SDL_Surface* screen = SDL_SetVideoMode(800, 600, 0, 0);

	/*Spawn the interface*/
	interface game;

	/*Make our dummy event for polls*/
	SDL_Event event;

	/*Flag to kill the game*/
	bool gameover = 0;

	/*Loop of everything*/
	while (!gameover)
	{
		/*Seek out event*/
		if (SDL_PollEvent(&event)) {
			/*Do stuff with event*/
			switch (event.type) {
			/*Kill handler*/
			case SDL_QUIT:
				gameover = 1;
				break;

			/*Keyboard handler. Maybe I'll optimize such that the game knows if it even needs to check this (EG if sticks are used)*/
			case SDL_JOYBUTTONUP:
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBALLMOTION:
			case SDL_JOYHATMOTION:
			case SDL_JOYAXISMOTION:
				printf("JOYSTICKS EXIST!\n");
				break;
			case SDL_KEYDOWN:
				for(int i = 0; i < 10; i++){
					if(event.key.keysym.sym == game.input[i].key.keysym.sym) 
						printf("%s pressed\n", game.inputName[i]);
				}
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				for(int i = 0; i < 10; i++){
					if(event.key.keysym.sym == game.input[i].key.keysym.sym) 
						printf("%s released\n", game.inputName[i]);
				}
				break;
			}
		}

		/*Refresh, not important just yet*/
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}


	/*Quit SDL*/
	SDL_Quit();

	return 0;
}

