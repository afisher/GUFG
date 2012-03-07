/*Interface header for GUFG
 *The interface class/struct is intended as an abstraction between the player and the engine
 *This is where things pertaining to inputs should go, as well as possibly information like the
 *round timer.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "SDL.h"
#include "character.h"
#include <stdio.h>
struct interface
{
public:
	interface(); 		//Constructor!
	void keyConfig(int);	//Keyconfig. Sets up buttons so they're all dynamic-y and such
	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	char * inputName[10];	//Input names. This is really just for housekeeping.
	void pushInput(int[], int[], int[]); //I know I know this is pretty ugly. Currently just exists to test the input buffer. Eventually will not be shit.
	void resolve();		//This will eventually resolve actions (Such as a move hitting a player.) With only one player, this is not currently possible.
	void draw();		//This will eventually draw everything. Right now the drawing is done in the main loop.
	character * pick;
	void runTimer();
		
private:
	int inputBuffer[30];
	int state;
	int timer;
/*
	player p1;		
	player p2;		
*/
};
