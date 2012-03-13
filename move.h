#include "SDL.h"
#include "frame.h"

class move{
public:
	move();
	move(char*);
	move(char*, int);
	move(char*, char*, bool, int);
	move(char*, char*, bool);
	~move();
	int currentFrame;
	void execute();				//Do the move. 
	bool check(bool[], bool[], int);	//Check to see if the move is possible right now.
	void setTolerance(int);
	void execute(frame *&);
	void step(SDL_Rect&, SDL_Rect&, SDL_Rect&, SDL_Rect&, frame *&);
	int input;
	bool operator==(move*); //An operator to compare allowed start states versus a move's current state; Basically a cancel mechanism.
	frame * start;
	int * state;	//An array of states. If the states are single integers, the array is the same size as the number of frames.
private:
	int frames;	//Number of frames.
//	SDL_Surface *sprite, *hit, *hittable, *collision;
	char * name;
	bool special;
	int cost;
	int tolerance;
	int button[5];
	SDL_Rect * collision;	//This will be an array of rects that are the collision boxes for the move per frame
	SDL_Rect ** hitbox;	//Same but for hitboxes
	SDL_Rect ** hittable;	//Same but for hittable boxes
	SDL_Rect * deltaPos;	//Same but for position on the screen.
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
