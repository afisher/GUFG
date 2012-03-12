#include "charlist.h"
class player{
public:
	player();

	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	frame * current;
	character * pick;
	int deltaX, deltaY;
	char * inputName[10];	//Input names. This is really just for housekeeping.
	int aerial;
	int sFlag;
	int gameover;
	int facing;
	void init();
	void keyConfig(int);
	void characterSelect(int);
	void pushInput(int*, int*, int*);
	int colorkey;
private:
	int inputBuffer[29];
};
