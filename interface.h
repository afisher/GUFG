/*Interface header for GUFG
 *The interface class/struct is intended as an abstraction between the player and the engine
 *This is where things pertaining to inputs should go, as well as possibly information like the
 *round timer.
 *
 *Written by Alex Kelly in 2012
 *Licensed under MIT OSI: Additional information in the file "COPYING"
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include "player.h"
class interface
{
public:
	interface(); 		//Constructor!
	~interface();
	/*Important interface functions that will remain interface functions*/
	
	void readInput();	//Reads input from players. 
	void resolve();		//This will eventually resolve actions (Such as a move hitting a player.) With only one player, this is not currently possible.
	void draw();		//This will eventually draw everything. Right now the drawing is done in the main loop.
	void runTimer();
	void spriteInit();
	void flip();
	void roundInit();
	void matchInit();
	void cSelectMenu();
	void checkWin();
	bool checkCollision(SDL_Rect, SDL_Rect);
	int dragBG(int);

	player * p[2];	
	SDL_Surface *background, *screen;
	SDL_Rect bg;
	int grav;
	int colorKey;
	bool sAxis1[4], sAxis2[4]; 
	bool fullscreen;
	bool posEdge1[5], posEdge2[5];
	bool negEdge1[5], negEdge2[5];
	void readStick(SDL_Event&);
	void readKey(SDL_Event&);
	int combo1;
	int combo2;
	int numRounds;
	bool gameover;

//testing variables;
	bool q;
private:
	int timer;
	int screenHeight, screenWidth, floor, wall;
};
