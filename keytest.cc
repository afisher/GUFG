/*Key Test for GUFG
 *This program is intended to do some of the initial SDL engine testing for
 *Project: Ground Up Fighting Game.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "interface.h"

const int GRAV = 5;

int main( int argc, char* argv[] )
{
	/*Set up sprite stuff sprite*/
	SDL_Surface *sprite, *sTemp, *bg;
	SDL_Rect sRect;
	int deltaX = 0;
	int deltaY = 0;
	bool aerial = 0;

	/*Ghetto alpha-value. Not sure why we can't alpha value. This might change*/
	int colorKey;

	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);

	/*WM stuff blah blah*/
	SDL_WM_SetCaption("GUFG", "GUFG");

	/*Make a window*/
	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, 0);

	/*Set up the sprite*/
	sTemp = SDL_LoadBMP("SP.bmp");
	sprite = SDL_DisplayFormat(sTemp);
	SDL_FreeSurface(sTemp);

	/*Background color, temporary until we have backgrounds*/
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));

	/*Set the color key*/
	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);

	/*More sprite setup*/
	sRect.x = 200;
	sRect.y = 330;

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
				if(!aerial){
					if(event.key.keysym.sym == game.input[0].key.keysym.sym) {
						deltaY = -25;
						aerial = 1;
					if(event.key.keysym.sym == game.input[2].key.keysym.sym) {
						if(aerial) deltaX = -5;
						else deltaX = -1;
					}
					if(event.key.keysym.sym == game.input[3].key.keysym.sym) {
						if(aerial) deltaX = 5;
						else deltaX = 1;
					}
				}
				for(int i = 0; i < 10; i++){
					if(event.key.keysym.sym == game.input[i].key.keysym.sym) 
						printf("%s pressed\n", game.inputName[i]);
					if(i == 2) i += 2;
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
				if(!aerial){
					for(int i = 2; i < 4; i++)
						if(event.key.keysym.sym == game.input[i].key.keysym.sym)
							deltaX = 0;
				}
				for(int i = 0; i < 10; i++){
					if(event.key.keysym.sym == game.input[i].key.keysym.sym) 
						printf("%s released\n", game.inputName[i]);
					if(i == 2) i += 2;
				}

				break;
			}

		}
		/* Movement currently determined by static deltas */
		sRect.x += deltaX;
		sRect.y += deltaY;

		/* No escaping the screen */
		if (sRect.x < -10)
			sRect.x = -10;
		else if (sRect.x > 560)
			sRect.x = 560;
		if (sRect.y < 0)
			sRect.y = 0;
		else if (sRect.y > 330)
			sRect.y = 330;

		/*Enforcing gravity*/
		if(aerial && sRect.y == 0) { aerial = 0; deltaX = 0; }
		if(aerial) deltaY += GRAV;

		/*Refresh, not important just yet*/
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
		SDL_BlitSurface(sprite, NULL, screen, &sRect);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}


	/*Quit SDL*/
	SDL_Quit();

	return 0;
}

