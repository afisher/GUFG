/*Header file for character class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012
 *Under MIT OSI license, see "COPYING" for details
 */

#include "SDL.h"
#include "hitstun.h"
#include <stdio.h>

class character{
public:
	character(); 	//Load the entire character into memory
	~character();	//Free stuff

	moveTrie * head;
	moveTrie * airHead;

	char * name; //The name of the directory from which the character spawns. This is important for loading into memory
	SDL_Surface * draw(int);
	int takeHit(move *);

	move * neutral;
	move * walk;
	move * jump;
	move * dash;
	move * brake;
	move * walkBack;
	hitstun * reel;	
	
	move * cMove;

	int health;
	int meter;
	int volitionX;
	int volitionY;
	
private:

	int state; 	//The current state of the character. This might need to be more variables. We'll see.


};	
