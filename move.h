#include "SDL.h"

class move{
public:
	move();
	~move();
	void execute();
	int frames;
	SDL_Surface *sprite, *hit, *hittable, *collision;
};

class moveTrie{
public:
	move * moveHook(int);
	moveTrie * child[13];
	move * fish;
};
