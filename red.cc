#include "red.h"
red::red()
{
	name = "Red";
	head = new moveTrie;
	head->insert(new move("Red/A"));
	head->fish[0].xLock = 1; 
	
	initMoves();
	

	cMove = neutral;
	
}
