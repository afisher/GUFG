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
	/*Spawn the interface*/
	interface game;

	/*Loop of everything*/
	while (!game.gameover){
		while(SDL_GetTicks() % 17 != 1);
		game.readInput();
		game.pushInput(game.sAxis, game.posEdge, game.negEdge);
		game.resolve();
		game.draw();
		while(SDL_GetTicks() % 17 != 0);
	}
}
