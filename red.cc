#include "red.h"
red::red()
{
	name = "Red";
	head = new moveTrie;
	head->insert(new move("Red/A", "A", 0, 10));
	head->fish[0].debugStateInit(2, 16, 30);
	head->fish[0].debugRectsInit();
	head->fish[0].debugHitboxInit(45, 35, 30, 30);
	head->fish[0].debugHittableInit(0, 0, 65, 150);
	head->fish[0].debugDamageInit(9);
	head->fish[0].debugCollisionInit(5, 0, 55, 150);
	head->fish[0].xLock = 1;
	head->fish[0].yLock = 1;

	neutral = new move("Red/N", 1);
	neutral->debugRectsInit();
	neutral->debugStateInit(1, 31, 31);
	neutral->debugHittableInit(0, 0, 65, 150);
	neutral->debugCollisionInit(5, 0, 55, 150);
	
	cMove = neutral;
}
