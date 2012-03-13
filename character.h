#include "SDL.h"
#include "move.h"
#include <stdio.h>

class character{
public:
	character(); 	//Load the entire character into memory
	~character();	//Free stuff
	
	moveTrie * head;
	void run(int, SDL_Rect&, SDL_Rect&, SDL_Rect&, frame *&);
		
	move * standingAnim;
	char * name; //The name of the directory from which the character spawns. This is important for loading into memory

private:

	int state; 	//The current state of the character. This might need to be more variables. We'll see.


};	
