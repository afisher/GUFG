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
	
hitstun::hitstun(char * n, int l)
{
	name = n;
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	special = 0;
	start = new frame(n, l);
	tolerance = 30;
	frames = l;
	state = 0;
	collision = new SDL_Rect[l];
	hitbox = new SDL_Rect[l];
	hitreg = new SDL_Rect[l];
	delta = new SDL_Rect[l];
	xLock = 0;
	stun = 10;
	yLock = 0;
	currentFrame = 0;
	damage = 0;
	blockMask = 7;
	blockState = 0;
	debugStateInit(0, 0, 0);
}

void hitstun::blockSuccess(int st){
	init(st);
}
