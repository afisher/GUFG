#include "red.h"
red::red()
{
	name = "Red\0";
	a5A = new frame("Red/A\0", 10);
}

void red::m5A(int f, SDL_Rect &collision, SDL_Rect &hitBox, SDL_Rect &hittable, frame *& sprite)
{
	if(f = 0) sprite = a5A;
}
