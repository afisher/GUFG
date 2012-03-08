/*Key Test for GUFG
 *This program is intended to do some of the initial SDL engine testing for
 *Project: Ground Up Fighting Game.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "interface.h"


int main(int argc, char* argv[])
{
	/*Set up sprite stuff sprite*/
	SDL_Surface *p1sprite, *p2sprite, *sTemp, *bg;
	
	/*Ghetto alpha-value. Not sure why we can't alpha value. This might change*/
	int colorKey;

	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_JOYSTICK);

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

	/*Spawn the interface*/
	interface game;


	/*Flag to kill the game*/
	game.gameover = 0;

	/*Loop of everything*/
	while (!game.gameover){
		while(SDL_GetTicks() % 17 != 1);
		/*Seek out event*/
		game.readInput();
		game.pushInput(game.sAxis, game.posEdge, game.negEdge);
		game.resolve();
		/*Refresh, not important just yet*/
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
		SDL_BlitSurface(p1sprite, NULL, screen, &game.s1Rect);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		for(int i = 0; i < 5; i++){
			game.posEdge[i] = 0;
			game.negEdge[i] = 0;
		}
		game.runTimer();
		while(SDL_GetTicks() % 17 != 0);

	}
}
