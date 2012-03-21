#include "character.h"
#include <iostream>
using namespace std;
#include <fstream>
#include <cstring>

character::character() 
//Character constructor. This loads the whole character into memory so that that we don't have disk reads during gameplay
{
	name = "White";
	moveTrie * curr;
	move * temp;

	/*Currently I'm using this as a test case for my move hooks*/

	head = new moveTrie(new move("White/A", "A", 0, 11));
	head->fish[0].debugStateInit(3, 16, 30);
	head->fish[0].debugRectsInit();
	head->fish[0].debugDamageInit(10);
	head->fish[0].debugHitboxInit(45, 45, 30, 30);
	head->fish[0].debugHittableInit(0, 0, 80, 150);
	head->fish[0].debugCollisionInit(5, 0, 55, 150);
	head->fish[0].xLock = 1;

	temp = new move("White/D", "D", 0, 1);
	head->insert(temp);
	head->fish[1].debugStateInit(3, 16, 30);
	head->fish[1].debugRectsInit();
	head->fish[1].debugCollisionInit(5, 0, 55, 150);
	airHead = new moveTrie(temp);

	neutral = new move("White/N", 1);
	neutral->debugRectsInit();
	neutral->debugStateInit(1, 31, 31);
	neutral->debugHittableInit(0, 0, 65, 150);
	neutral->debugCollisionInit(5, 0, 55, 150);

	walkBack = new move("White/WB", 1);
	walkBack->debugRectsInit();
	walkBack->debugStateInit(1, 31, 31);
	walkBack->debugHittableInit(0, 0, 65, 150);
	walkBack->debugDeltaInit(-5, 0, 0, 0);
	walkBack->debugCollisionInit(5, 0, 55, 150);
	walkBack->xLock = 1;

	walk = new move("White/W", 1);
	walk->debugRectsInit();
	walk->debugStateInit(1, 31, 31);
	walk->debugHittableInit(0, 0, 65, 150);
	walk->debugDeltaInit(5, 0, 0, 0);
	walk->debugCollisionInit(5, 0, 55, 150);
	walk->setTolerance(1);
	walk->xLock = 1;

	jump = new move("White/J", 1);
	jump->debugRectsInit();
	jump->debugStateInit(5, 31, 31);
	jump->debugHittableInit(0, 0, 65, 150);
	jump->debugDeltaInit(0, -35, 0, 0);
	jump->debugCollisionInit(5, 0, 55, 150);
	jump->setTolerance(1);
	
	head->insert(9, jump);
	head->insert(8, jump);
	head->insert(7, jump);

	reel = new hitstun("White/H", 1);
	reel->debugRectsInit();
	reel->debugHittableInit(0, 0, 65, 150);
	reel->debugCollisionInit(5, 0, 55, 150);

	fall = new hitstun("White/UT", 1);
	fall->debugRectsInit();
	fall->debugHittableInit(0, 0, 150, 65);
	fall->debugCollisionInit(5, 0, 55, 55);

	airBlock = new hitstun("White/BL", 1);
	airBlock->blockState = 4;
	airBlock->debugRectsInit();
	airBlock->debugHittableInit(0, 0, 65, 150);
	airBlock->debugCollisionInit(5, 0, 55, 150);
	airBlock->debugStateInit(33, 32, 32);

	standBlock = new hitstun("White/BL", 1);
	standBlock->blockState = 2;
	standBlock->debugRectsInit();
	standBlock->debugHittableInit(0, 0, 65, 150);
	standBlock->debugCollisionInit(0, 5, 55, 150);
	standBlock->debugStateInit(33, 32, 32);
	standBlock->xLock = 1;
	
	crouchBlock = new hitstun("White/BL", 1);
	crouchBlock->blockState = 1;
	crouchBlock->debugRectsInit();
	crouchBlock->debugHittableInit(0, 0, 65, 150);
	crouchBlock->debugCollisionInit(5, 0, 55, 150);
	crouchBlock->debugStateInit(33, 32, 32);
	crouchBlock->xLock = 1;

	cMove = neutral;

	health = 300;
	meter = 0;
	rounds = 0;
	volitionX = 0;
	volitionY = 0;
	aerial = 0;
}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	delete head;
	//Empty for now
}

/*Here begin move functions. Actually contemplating making this a class instead, but this might be simpler for now*/

int character::takeHit(move * attack)
{
	/*All the important logic like blocking and stuff will go here later.*/
	cMove->init();
	int ct = 0;
	/*Damage scaling logic will factor into this later*/
	if(!attack->cFlag){
		if(cMove->blockState & attack->blockMask){
		/*Do blocking stuff. Specifically, we need to put the player in
		block stun, a state in which they're frozen in the last block animation until blockstun ends.
		During blockstun, generally the option available to everyone is to switch blocks, so as not
		to allow mixup to be guaranteed. Some games have options like Alpha Counters (Any attack out of blockstun) 
		or Rolls (Invulnerable movement option) that cost meter but can break out of blockstun state. These can
		be universal or character-specific. Notably, characters are throw-invulnerable during blockstun,
		for what should be obvious reasons. In MOST games, characters remain throw-invuln for a few frames after
		coming out of blockstun.
		*/
			cMove->blockSuccess(attack->stun);
			printf("Block!\n");
			attack->connect();
		}
		else{
		/*Do hitstun stuff. Specifically, the player needs to be put in a "hitstun" state for a number
		of frames defined by the stun of the attacking move. Blockstun may be separate, or a function of the same
		number. The hitstun state while standing generally has a "reeling" animation that can affect what hits them,
		but unless we have a "burst-like" option, no actions can be taken by a player during hitstun. There will probably
		also be a "launch" property, which knocks a grounded player into the air if hit by certain moves.
		In the air, hitstun is slightly different. There is a "falling" animation the character is in, and they are launched
		a little bit by any further move that hits them. Generally, there's some option to get out of aerial hitstun, most
		easily referred to as a "Tech." Therefore, while falling state persists until they hit the ground, there's an amount of
		"untechable" time associated with any move that hits them. This is not functionally different from hitstun in any other way.
		*/
			if(cMove == reel || cMove == fall) ct++;
			if(!aerial && attack->launch) aerial = 1;
			if(aerial){
				volitionY -= attack->lift;
				fall->init(attack->stun);
				cMove = fall;
			} else {
				reel->init(attack->stun);
				cMove = reel;
			}
			health -= attack->damage;
			if(health < 0){
				health = 0; 	//Healthbar can't go below 0;
				//Reckon other KO stuff;
			}
		}
		volitionX -= attack->push;
		attack->connect(); 	//Tell the attack it's connected.
	}
 	return ct;
	/*Eventually the plan is to have this return a combo count. This not only allows us to display a counter and do whatever scaling/combo 
	mitigation we want to, but also allows us to do things like pushback ramping during blockstrings*/
}

SDL_Surface * character::draw(int facing){
	SDL_Surface * temp = cMove->draw(facing);
	if(cMove->currentFrame == cMove->frames){
		cMove->init();
		cMove = NULL;
	}
	return temp;
}
