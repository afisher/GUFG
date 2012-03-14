#include "red.h"
red::red()
{
	name = "Red";
	head = new moveTrie;
	head->insert(new move("Red/A", "A", 0, 10));
	head->fish[0].debugStateInit(1, 1);
	head->fish[0].debugRectsInit();
	head->fish[0].debugHittableInit(0, 0, 100, 150);
	head->fish[0].xLock = 1;
	head->fish[0].yLock = 1;
}
