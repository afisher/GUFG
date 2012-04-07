/*Header file for "Player" class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012
 *Under MIT OSI license, see the file "COPYING" for details
 */

#include "charlist.h"
class player{
public:
	player();
	~player();

	char * inputName[10];	//Input names. This is really just for housekeeping.
	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	SDL_Surface * sprite;
	character * pick;
	int deltaX, deltaY;
	int rounds;
	int facing;
	int padding[490];	//More magic. Do not touch
	void init();
	void keyConfig(int);
	void characterSelect(int);
	void pushInput(bool*, bool*, bool*);
	int colorkey;
	void draw();
	void spriteInit();

	SDL_Rect pos, hitbox, *hitreg, collision, delta, lock;
	int hitComplexity, regComplexity;
	bool rCorner, lCorner;
	
	/*Helper functions for "resolve" tick*/
	void updateRects();
	int checkHit(player*);
	void resolveCollision(player*);
	void conserveMomentum();
	void enforceGravity(int, int);
	void checkBlocking();
	void checkFacing(int);
	int dragBG(int, int);
	void checkCorners(int, int, int);
private:
	int inputBuffer[29];
};
