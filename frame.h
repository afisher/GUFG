#include "SDL.h"

class frame{
public:
	frame();
	frame(char *);
	~frame();
	frame * play(SDL_Surface *&);
	SDL_Surface *sprite;
//	SDL_Rect *hitbox, *collision, *hittable;
	frame * next;
};
