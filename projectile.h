#include "special.h"
class projectile {
public:
	projectile(char*);
	~projectile();
	
	int owner;
	SDL_Rect pos;
	SDL_Surface * sprite;
	move * go;
	move * spawn;
};

class summon : public special {
public:
	projectile * payload;
	projectile * spawnProjectile();
};
