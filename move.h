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
	void execute(move *);		/*Okay so, hopefully the idea here is that we can init() 
						the move we're cancelling out of in the usual case, and, well
						Do other stuff sometimes.*/
	virtual bool check(bool[], bool[], int);	//Check to see if the move is possible right now.
	void setTolerance(int);
	virtual void blockSuccess(int);
	void pollRects(SDL_Rect&, SDL_Rect&, SDL_Rect&, SDL_Rect&); //Return the relevant information needed for interface::resolve(), then step to the next frame.
	bool operator>(move*); //Cancel allowed check. Essentially: is move Lvalue allowed given the current state of move Rvalue?
	void init();		//Really just sets current frame to 0. I wanted current frame to be private for now, so I don't break anything.
	virtual void step();
	void connect();
	SDL_Surface * draw(int);

	frame * start;		//The first frame of the move's sprite.
	unsigned int allowed;		//The state in which the move is allowed. Depending on how we handle states, this may need to be an array of states or something.
	bool xLock, yLock;
	int push;		//How much pushback the move does
	int lift;		//How much the move lifts an aerial opponent.
	bool launch;		//Whether or not the move launches on hit

	int damage;		//How much damage the move does
	int stun;		//How much stun the move does
	unsigned int blockMask;	//Low, High, Air Block. Basically a 3-digit binary number expressed as an integer from 0-7.
				//Digits from right to left: Blockable low, Blockable high, Blockable in the air, 
	unsigned int blockState;//Partner to the blockmask. This variable is the move's actual "guard state." Usually it will only be one of the three.
	bool cFlag;
	unsigned int state;	
	unsigned int cState;
	int currentFrame; 	//The frame that is currently running.
	int frames;	//Number of frames.
	
	//Some initialization functions for testing purposes. Once we work out the finalized constructor this will be obviated.
	void debugStateInit(int, int, int); 	
	void debugRectsInit();
	void debugDamageInit(int);
	void debugHittableInit(int, int, int, int);
	void debugHitboxInit(int, int, int, int);
	void debugDeltaInit(int, int, int, int);
	void debugCollisionInit(int, int, int, int);

//	SDL_Surface *sprite, *hit, *hitreg, *collision;
	int button[5];
	char * name;
	int cost;
	int tolerance;
	SDL_Rect * collision;	//This will be an array of rects that are the collision boxes for the move per frame
	SDL_Rect * hitbox;	//Same but for hitboxes
	SDL_Rect * hitreg;	//Same but for hitreg boxes
	SDL_Rect * delta;	//Same but for position on the screen.
};

/*Move trie class. Essentially a recursive trie-search algorithm that looks for input hooks*/

class moveTrie{
public:
	moveTrie();
	moveTrie(move*);
	moveTrie(move*, char*);
	~moveTrie();
	move * moveHook(int[40], int, int, bool[5], bool[5], move *);
	moveTrie * child[10];
	move * fish; 		//Because it's what a hook catches! Okay, this name probably needs to change.
	int * tolerance;
	moveTrie * insert(int);
	moveTrie * insert(int, move*);
	void insert(move *);
private:
	int occupants;
	int * button;
};
