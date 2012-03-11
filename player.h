#include "character.h"
class player{
public:
	SDL_Rect pos;
	SDL_Surface *sprite;
	frame * current;

	void init();
};
