#include "red.h"
red::red()
{
	name = "Red";
	head = new moveTrie;
	head->insert(new move("Red/A"));
	head->fish[0].xLock = 1; 

	neutral = new move("Red/N", 1);
	neutral->debugRectsInit();
	neutral->debugStateInit(1, 31, 31);
	neutral->debugHittableInit(0, 0, 65, 150);
	neutral->debugCollisionInit(5, 0, 55, 150);
	
	cMove = neutral;
}
