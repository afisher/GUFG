#include "hitstun.h"

void hitstun::init(int n)
{
	counter = n;
}

void hitstun::step()
{
	if(counter <= 0) currentFrame++;
	else counter--;
}
	
void hitstun::blockSuccess(int st){
	init(st);
}

hitstun::hitstun(char * n)
{
	build(n);
	tolerance = 0;
	activation = 0;
	xLock = 0;
	yLock = 0;
	currentFrame = 0;
}
