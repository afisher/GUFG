#include "SDL.h"
#include "frame.h"

struct activeWindow{
	int start;
	int end;
	int damage;
	int stun;
};

class move{
public:
	move();
	move(char*);
	move(char*, int);
	move(char*, char*, bool, int);
	move(char*, char*, bool);
	~move();
	void execute();			//Do the move. 
	bool check(bool[], bool[], int);	//Check to see if the move is possible right now.
	void setTolerance(int);
	void execute(frame *&);
	void execute(int, SDL_Surface *&, SDL_Rect&, SDL_Rect&);
	int input;
	frame * start;
private:
	int frames;	//Number of frames.
//	SDL_Surface *sprite, *hit, *hittable, *collision;
	char * name;
	bool special;
	int cost;
	int tolerance;
	int button[5];
};

class moveTrie{
public:
	moveTrie();
	moveTrie(move*);
	moveTrie(move*, char*);
	~moveTrie();
	move * moveHook(int[40], int, int, bool[5], bool[5]);
	moveTrie * child[10];
	move * fish; 		//Because it's what a hook catches! Okay, this name probably needs to change.
	moveTrie * insert(int);
	moveTrie * insert(int, move*);
	void insert(move *);
private:
	int occupants;
	int * button;
};
