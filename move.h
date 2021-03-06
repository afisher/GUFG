/*Header file for "move" class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012. 
 *Licensed under MIT OSI, see "COPYING" for details
 */

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "aux.h"

using namespace std;

class move{
public:
	move();
	move(char*);
	~move();
	void build(char *);
	void execute();
	void execute(move *);		/*Okay so, hopefully the idea here is that we can init() 
						the move we're cancelling out of in the usual case, and, well
						Do other stuff sometimes.*/
	virtual bool check(bool[], bool[], int, int);	//Check to see if the move is possible right now.
	void setTolerance(int);
	virtual void blockSuccess(int);
	void pollRects(SDL_Rect&, SDL_Rect&, SDL_Rect*&, int&, SDL_Rect*&, int&); //Return the relevant information needed for interface::resolve(), then step to the next frame.
	bool operator>(move*); //Cancel allowed check. Essentially: is move Lvalue allowed given the current state of move Rvalue?
	void init();		//Really just sets current frame to 0. I wanted current frame to be private for now, so I don't break anything.
	virtual void step();
	void connect();
	SDL_Surface * draw(int, bool);

	unsigned int allowed;		//The state in which the move is allowed. Depending on how we handle states, this may need to be an array of states or something.
	bool xLock, yLock;
	int push;		//How much pushback the move does
	int lift;		//How much the move lifts an aerial opponent.
	bool launch;		//Whether or not the move launches on hit

	int damage;		//How much damage the move does
	int stun;		//How much stun the move does
	unsigned int blockMask;	//Low, High, Air Block. Basically a 3-digit binary number expressed as an integer from 0-7.
				//Digits from right to left: Blockable low, Blockable high, Blockable in the air, 
	bool cFlag;
	unsigned int state;	
	unsigned int cState;
	int currentFrame; 	//The frame that is currently running.
	int frames;	//Number of frames.
	
	unsigned int blockState;//Partner to the blockmask. This variable is the move's actual "guard state." Usually it will only be one of the three.
	
//	SDL_Surface *sprite, *hit, *hitreg, *collision;
	int button[5];
	char * name;
	int cost;		
	int tolerance;		/*Tolerance refers to the individual size of the input buffer allowed for this move.
				Default is 30 (The entire input buffer)*/
	int activation;		/*Activation refers to the most recent accepted last command. So an activation of 0
				Implies that the most recent input has to have been *this frame* for the move to be
				Accepted. Default is 30 (the entire input buffer)*/
	SDL_Rect * collision;	//This will be an array of rects that are the collision boxes for the move per frame
	SDL_Rect ** hitbox;	//Same but for hitboxes
	SDL_Rect ** hitreg;	//Same but for hitreg boxes
	SDL_Rect * delta;	//Same but for position on the screen.
	int * hitComplexity;
	int * regComplexity;
	SDL_Surface **sprite, **fSprite;
};

