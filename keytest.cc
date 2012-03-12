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
		game.p1->pushInput(game.sAxis1, game.posEdge1, game.negEdge1);
		game.p2->pushInput(game.sAxis2, game.posEdge2, game.negEdge2);
		game.resolve();
		game.draw();
		while(SDL_GetTicks() % 17 != 0);
	}
}
