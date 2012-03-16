/*Header file for "Player" class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012
 *Under MIT OSI license, see the file "COPYING" for details
 */

#include "charlist.h"
class player{
public:
	player();

	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	SDL_Surface * sprite;
	character * pick;
	int deltaX, deltaY;
	char * inputName[10];	//Input names. This is really just for housekeeping.
	bool aerial;
	int gameover;
	int facing;
	void init();
	void keyConfig(int);
	void characterSelect(int);
	void pushInput(bool*, bool*, bool*);
	int colorkey;
	void draw();
	void spriteInit();
	SDL_Rect pos;
private:
	int inputBuffer[29];
};
