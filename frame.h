#include "SDL.h"

class frame{
public:
	frame();
	void init(char *);
	frame(char *, int);
	~frame();
	frame * play(SDL_Surface *&);
	SDL_Surface *sprite, *fSprite;
//	SDL_Rect *hitbox, *collision, *hittable;
	frame * next;
};
