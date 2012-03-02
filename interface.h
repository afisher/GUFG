#include "SDL.h"
#include <stdio.h>
struct interface
{
public:
	interface(); 		//Constructor! Currently does almost nothing.
	void keyConfig(int);	//Keyconfig. Sets up buttons so they're all dynamic-y and such
	SDL_Event input[10];	//Inputs. These are the SDL_Events tied to the 10 buttons in the actual game
	char * inputName[10];	//Input names. This is really just for housekeeping.
};
