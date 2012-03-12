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
#include "player.h"
class interface
{
public:
	interface(); 		//Constructor!
	
	/*Important interface functions that will remain interface functions*/
	
	void readInput();	//Reads input from players. 
	void resolve();		//This will eventually resolve actions (Such as a move hitting a player.) With only one player, this is not currently possible.
	void draw();		//This will eventually draw everything. Right now the drawing is done in the main loop.
	void runTimer();
	void spriteInit();
	void flip();

	player *p1, *p2;	
	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	SDL_Surface *p1sprite, *p2sprite, *bg, *screen;
	character * pick;
	int grav;
	char * inputName[10];	//Input names. This is really just for housekeeping.
	int colorKey;
	int deltaX1, deltaX2, deltaY1, deltaY2;
	SDL_Rect pos1, pos2;
	int sAxis1[4], sAxis2[4]; 
	int posEdge1[5], posEdge2[5];
	int negEdge1[5], negEdge2[5];
	int sFlag1, sFlag2;
	int gameover;
private:
	int inputBuffer[30];
	int state;
	int timer;
/*
	player p1;		
	player p2;		
*/
};
