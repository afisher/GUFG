#include "SDL.h"

struct activeWindow{
	int start;
	int end;
	int damage;
	int stun;
};

class move{
public:
	move();
	~move();
	void execute();	//Do the move. The char arg is just for testing.
	int check(int, int);	//Check to see if the move is possible right now.
private:
	int frames;	//Number of frames.
//	SDL_Surface *sprite, *hit, *hittable, *collision;
	char * name;
	int cost;
};

class moveTrie{
public:
	move * moveHook(int, int, int);
	moveTrie ** child;
	move * fish; 		//Because it's what a hook catches! Okay, this name probably needs to change.
private:
	int children;
};
