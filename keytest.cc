/*Key Test for GUFG
 *This tiny program is intended to do some of the initial SDL engine testing for
 *Project: Ground Up Fighting Game.
 *
 *Written by Alex Kelly in 2012
 *Under the MIT OSI license
 *For detailed licensing information, see the COPYING file in this directory.
 */

#include "interface.h"
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	/*Spawn the interface*/
	interface game;
	int rounds = 2;
	if(argc > 1) rounds = atoi(argv[1]);
	if(rounds > 0 || rounds < 10) game.numRounds = rounds;

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
