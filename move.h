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
	void execute();				 
	bool check(bool[], bool[], int);	//Check to see if the move is possible right now.
	void setTolerance(int);
	void execute(frame *&);			//Currently what we're using to send the first frame to the animation routines. This will likely change.
	bool step(SDL_Rect&, SDL_Rect&, SDL_Rect&, SDL_Rect&, frame *&); //Return the relevant information needed for interface::resolve(), then step to the next frame.
	bool operator==(move*); //An operator to compare allowed start states versus a move's current state; Basically a cancel mechanism.
	void init();		//Really just sets current frame to 0. I wanted current frame to be private for now, so I don't break anything.
		
	frame * start;		//The first frame of the move's sprite.
	int * state;		//An array of states. If the states are single integers, the array is the same size as the number of frames.
	int allowed;		//The state in which the move is allowed. Depending on how we handle states, this may need to be an array of states or something.

	void debugStateInit(int, int); //State initialization function for testing purposes. Once we work out the finalized constructor this will be obviated.
	void debugRectsInit();
private:
	int frames;	//Number of frames.
//	SDL_Surface *sprite, *hit, *hittable, *collision;
	char * name;
	bool special;
	int cost;
	int tolerance;
	int button[5];
	int currentFrame; 	//The frame that is currently running.
	SDL_Rect * collision;	//This will be an array of rects that are the collision boxes for the move per frame
	SDL_Rect * hitbox;	//Same but for hitboxes
	SDL_Rect * hittable;	//Same but for hittable boxes
	SDL_Rect * delta;	//Same but for position on the screen.
};

class moveTrie{
public:
	moveTrie();
	moveTrie(move*);
	moveTrie(move*, char*);
	~moveTrie();
	move * moveHook(int[40], int, int, bool[5], bool[5], move *);
	moveTrie * child[10];
	move * fish; 		//Because it's what a hook catches! Okay, this name probably needs to change.
	moveTrie * insert(int);
	moveTrie * insert(int, move*);
	void insert(move *);
private:
	int occupants;
	int * button;
};
