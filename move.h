/*Header file for "move" class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012. 
 *Licensed under MIT OSI, see "COPYING" for details
 */

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
	bool step(SDL_Rect&, SDL_Rect&, SDL_Rect&, SDL_Rect&); //Return the relevant information needed for interface::resolve(), then step to the next frame.
	bool operator==(move*); //Cancel allowed check. Essentially: is move Lvalue allowed given the current state of move Rvalue?
	void init();		//Really just sets current frame to 0. I wanted current frame to be private for now, so I don't break anything.
	void connect();

	frame * start;		//The first frame of the move's sprite.
	int * state;		//An array of states. If the states are single integers, the array is the same size as the number of frames.
	int allowed;		//The state in which the move is allowed. Depending on how we handle states, this may need to be an array of states or something.
	bool xLock, yLock;
	int damage;		//How much damage the move does
	int stun;		//How much stun the move does
	int blockMask;		//High, Low, Mid, Unblockable, and Air Block check. Basically a 3-digit binary number expressed as an integer from 0-7
	bool cFlag;
	int cState;

	//Some initialization functions for testing purposes. Once we work out the finalized constructor this will be obviated.
	void debugStateInit(int, int, int); 	
	void debugRectsInit();
	void debugDamageInit(int);
	void debugHittableInit(int, int, int, int);
	void debugHitboxInit(int, int, int, int);
	void debugDeltaInit(int, int, int, int);
	void debugCollisionInit(int, int, int, int);
private:
	int frames;	//Number of frames.
//	SDL_Surface *sprite, *hit, *hitreg, *collision;
	bool special;
	int cost;
	int tolerance;
	int button[5];
	char * name;
	int currentFrame; 	//The frame that is currently running.
	SDL_Rect * collision;	//This will be an array of rects that are the collision boxes for the move per frame
	SDL_Rect * hitbox;	//Same but for hitboxes
	SDL_Rect * hitreg;	//Same but for hitreg boxes
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
