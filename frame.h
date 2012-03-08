#include "SDL.h"
class frame{
public:
	SDL_Surface *sprite;
	SDL_Rect *hitbox, *hittable, *collision;
	frame * next;
private:
};
