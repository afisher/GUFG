/*Header file for character class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012
 *Under MIT OSI license, see "COPYING" for details
 */

#include "SDL.h"
#include "projectile.h"
#include <stdio.h>

class character{
public:
	character(); 	//Load the entire character into memory
	~character();	//Free stuff

	void initMoves();
	moveTrie * head;
	moveTrie * airHead;

	char * name; //The name of the directory from which the character spawns. This is important for loading into memory
	SDL_Surface * draw(int);
	int takeHit(move *);
	void prepHooks(int[], bool[], bool[]);

	move * neutral;
	move * walk;
	move * jump;
	move * fJump;
	move * bJump;
//	move * dash;
//	move * brake;
	move * walkBack;
	hitstun * reel;	
	hitstun * fall;
	hitstun * crouchBlock;
	hitstun * standBlock;
	hitstun * airBlock;

	move * cMove;

	int freeze;
	int health;
	int meter;
	int rounds;
	int volitionX;
	int volitionY;
	bool aerial;	
private:

	int state; 	//The current state of the character. This might need to be more variables. We'll see.


};	
