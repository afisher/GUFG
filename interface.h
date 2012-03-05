/*Interface header for GUFG
 *The interface class/struct is intended as an abstraction between the player and the engine
 *This is where things pertaining to inputs should go, as well as possibly information like the
 *round timer.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "SDL.h"
#include <stdio.h>
struct interface
{
public:
	interface(); 		//Constructor! Currently does almost nothing.
	void keyConfig(int);	//Keyconfig. Sets up buttons so they're all dynamic-y and such
	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	char * inputName[10];	//Input names. This is really just for housekeeping.
	void pushInput(SDL_Event &, int, int, int, int); //I know I know this is pretty ugly. Currently just exists to test the input buffer. Eventually will not be shit.
	void moveHook();	//This will search a trie of move inputs with the input buffer. Thanks for the suggestion, Psy!
	void resolve();		//This will eventually resolve actions (Such as a move hitting a player.) With only one player, this is not currently possible.
	void draw();		//This will eventually draw everything. Right now the drawing is done in the main loop.

private:
	int inputBuffer[16];
	int state;
	/*
	player p1;		//Obviously the idea here is that we'll reckon players separately within the interface eventually.
	player p2;		//But as it stands, there is only one player, and player is not a class. Etc. 
	*/
};
