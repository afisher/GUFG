/*Key Test for GUFG
 *This program is intended to do some of the initial SDL engine testing for
 *Project: Ground Up Fighting Game.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "interface.h"

const int GRAV = 3;

int main(int argc, char* argv[])
{
	/*Set up sprite stuff sprite*/
	SDL_Surface *p1sprite, *p2sprite, *sTemp, *bg;
	SDL_Rect s1Rect, s2Rect;
	int deltaX = 0, deltaY = 0;
	int aerial = 0;
	int sAxis[4]; 
	for(int i = 0; i < 5; i++)
		sAxis[i] = 0;
	int posEdge[5];
	int negEdge[5];
	for(int i = 0; i < 5; i++){
		posEdge[i] = 0;
		negEdge[i] = 0;
	}
	

	/*Ghetto alpha-value. Not sure why we can't alpha value. This might change*/
	int colorKey;

	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_JOYSTICK);
	SDL_JoystickOpen(1);

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
	while (!gameover){
		while(SDL_GetTicks() % 17 != 1);
		/*Seek out event*/
		for(int i = 0; i < 14; i++){
			if (SDL_PollEvent(&event)){
				/*Do stuff with event*/
				switch (event.type){
				/*Kill handler*/
				case SDL_QUIT:
					gameover = 1;
					break;

				/*Keyboard handler. Maybe I'll optimize such that the game knows if it even needs to check this (EG if sticks are used)*/
				case SDL_JOYAXISMOTION:
					for(int i = 0; i < 4; i++)
						if(event.jaxis.which == game.input[i].jaxis.which && event.jaxis.axis == game.input[i].jaxis.axis && event.jaxis.value == game.input[i].jaxis.value)
							sAxis[i] = 1;

					for(int i = 0; i < 4; i++)
						if(event.jaxis.which == game.input[i].jaxis.which && event.jaxis.axis == game.input[i].jaxis.axis && event.jaxis.value == 0)
							sAxis[i] = 0;
					break;
				case SDL_JOYBUTTONDOWN:
					for(int i = 4; i < 9; i++)
						if(event.jbutton.which == game.input[i].jbutton.which && event.jbutton.button == game.input[i].jbutton.button)
							posEdge[i-4] = 1;
					break;
				case SDL_JOYBUTTONUP:
					for(int i = 4; i < 9; i++)
						if(event.jbutton.which == game.input[i].jbutton.which && event.jbutton.button == game.input[i].jbutton.button)
							negEdge[i-4] = 1;
					break;
				case SDL_KEYDOWN:
					for(int i = 0; i < 4; i++)
						if(event.key.keysym.sym == game.input[i].key.keysym.sym) 
							sAxis[i] = 1;

					for(int i = 4; i < 9; i++)
						if(event.key.keysym.sym == game.input[i].key.keysym.sym)
							posEdge[i-4] = 1;
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
					for(int i = 0; i < 4; i++)
						if(event.key.keysym.sym == game.input[i].key.keysym.sym)
							sAxis[i] = 0;

					for(int i = 4; i < 9; i++)
						if(event.key.keysym.sym == game.input[i].key.keysym.sym)
							negEdge[i-4] = 1;
					
					break;
				}
	
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
		else if (s1Rect.x > 560)
			s1Rect.x = 560;
		if (s1Rect.y < 0)
			s1Rect.y = 0;
		else if (s1Rect.x > 560)
			s1Rect.x = 560;
		if (s1Rect.y < 0)
			s1Rect.y = 0;
		else if (s1Rect.y > 330)
			s1Rect.y = 330;

		/*Enforcing gravity*/
		if(s1Rect.y == 330 && aerial == 1)  
			aerial = 0; 
		if(!aerial){
			if(sAxis[0]) deltaY = -30;
			else deltaY = 0;
			if(sAxis[3]) deltaX = 5;
			if(sAxis[2]) deltaX = -5;
			if((!sAxis[2] && !sAxis[3]) || sAxis[1] == 1) deltaX = 0;
		}
		if(aerial) deltaY += GRAV;

		/*Refresh, not important just yet*/
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
		SDL_BlitSurface(p1sprite, NULL, screen, &s1Rect);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		game.pushInput(sAxis, posEdge, negEdge);
		for(int i = 0; i < 5; i++){
			posEdge[i] = 0;
			negEdge[i] = 0;
		}
		game.runTimer();
		while(SDL_GetTicks() % 17 != 0);

	}
}
