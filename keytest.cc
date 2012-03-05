/*Key Test for GUFG
 *This program is intended to do some of the initial SDL engine testing for
 *Project: Ground Up Fighting Game.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "interface.h"

const int GRAV = 1;

int main(int argc, char* argv[])
{
	/*Set up sprite stuff sprite*/
	SDL_Surface *p1sprite, *p2sprite, *sTemp, *bg;
	SDL_Rect s1Rect, s2Rect;
	int deltaX = 0, deltaY = 0;
	int aerial = 0;
	int sUp = 0, sDown = 0, sLeft = 0, sRight = 0;

	/*Ghetto alpha-value. Not sure why we can't alpha value. This might change*/
	int colorKey;

	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);

	/*WM stuff blah blah*/
	SDL_WM_SetCaption("GUFG", "GUFG");

	/*Make a window*/
	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, 0);

	/*Set up the p1sprite*/
	sTemp = SDL_LoadBMP("SP.bmp");
	p1sprite = SDL_DisplayFormat(sTemp);
	SDL_FreeSurface(sTemp);

	/*Background color, temporary until we have backgrounds*/
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));

	/*Set the color key*/
	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);
	SDL_SetColorKey(p1sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);

	/*More sprite setup*/
	s1Rect.x = 200;
	s1Rect.y = 330;

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
				if(event.key.keysym.sym == game.input[0].key.keysym.sym) {
					sUp = 1;
				}
				if(event.key.keysym.sym == game.input[1].key.keysym.sym) {
					sDown = 1;
				}
				if(event.key.keysym.sym == game.input[2].key.keysym.sym) {
					sLeft = 1;
				}
				if(event.key.keysym.sym == game.input[3].key.keysym.sym) {
					sRight = 1;
				}
				
				for(int i = 4; i < 10; i++){
					int a = 0;
					if(event.key.keysym.sym == game.input[i].key.keysym.sym){
						if(a != 0) printf("+"); 
						a++;
						printf("%s", game.inputName[i]); 
					}
					if(a > 0) printf(" pressed (%i)\n", a);
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
				if(event.key.keysym.sym == game.input[0].key.keysym.sym) {
					sUp = 0;
				}
				if(event.key.keysym.sym == game.input[1].key.keysym.sym) {
					sDown = 0;
				}
				if(event.key.keysym.sym == game.input[2].key.keysym.sym) {
					sLeft = 0;
				}
				if(event.key.keysym.sym == game.input[3].key.keysym.sym) {
					sRight = 0;
				}
				for(int i = 4; i < 10; i++){
					if(event.key.keysym.sym == game.input[i].key.keysym.sym) 
						printf("%s released\n", game.inputName[i]);
				}

				break;
			}

		}
		/* Movement currently determined by static deltas */
		if(s1Rect.y < 330) aerial = 1;
		s1Rect.x += deltaX;
		s1Rect.y += deltaY;

		/* No escaping the screen */
		if (s1Rect.x < -10)
			s1Rect.x = -10;
		else if (s1Rect.x > 560)
			s1Rect.x = 560;
		if (s1Rect.y < 0)
			s1Rect.y = 0;
		else if (s1Rect.y > 330)
			s1Rect.y = 330;

		/*Enforcing gravity*/
		if(s1Rect.y == 330 && aerial == 1) { 
			aerial = 0; 
		}
		if(!aerial){
			if(sUp) deltaY = -23;
			else deltaY = 0;
			if(sRight) deltaX = 3;
			if(sLeft) deltaX = -3;
			if((!sLeft && !sRight) || sDown == 1) deltaX = 0;
		}
		if(aerial) deltaY += GRAV;

		/*Refresh, not important just yet*/
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
		SDL_BlitSurface(p1sprite, NULL, screen, &s1Rect);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		game.pushInput(event, sUp, sDown, sLeft, sRight);
		while(SDL_GetTicks() % 17 != 0);

	}


	/*Quit SDL*/
	SDL_Quit();

	return 0;
}

