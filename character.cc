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

	head = new moveTrie(new move("White/A"));
	head->fish[0]->xLock = 1;

	temp = new move("White/D");
	head->insert(temp);
	airHead = new moveTrie(temp);

	neutral = new utility("White/NS");
	neutral->tolerance = 0; neutral->activation = 0;
	head->insert(neutral);

	walkBack = new utility("White/WB");
	walkBack->tolerance = 0; walkBack->activation = 0; 
	walkBack->xLock = 1;
	head->insert(4, walkBack);

	walk = new utility("White/W");
	walk->tolerance = 0; walk->activation = 0; 
	walk->xLock = 1;
	head->insert(6, walk);

	reel = new hitstun("White/H");
	reel->xLock = 1; reel->yLock = 1;

	fall = new hitstun("White/UT");

	airBlock = new hitstun("White/BA");

	standBlock = new hitstun("White/BH");
	standBlock->xLock = 1;
	
	crouchBlock = new hitstun("White/BL");
	crouchBlock->xLock = 1;

	temp = new utility("White/JQ");
	temp->activation = 0;
	head->insert(7, temp);

	temp = new utility("White/JF");
	temp->activation = 0;
	head->insert(9, temp);

	temp = new utility("White/JN");
	temp->activation = 0;
	head->insert(8, temp);
	
	cMove = neutral;
	bMove = NULL;
	
	health = 300;
	meter = 0;
	rounds = 0;
	volitionX = 0;
	volitionY = 0;
	aerial = 0;
	freeze = 0;
}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	delete head;
	delete neutral;
	delete walk;
	delete walkBack;
	delete reel;
	delete fall;
	delete crouchBlock;
	delete standBlock;
	delete airBlock;
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
	freeze = attack->stun / 2; //For now this is the simple formula for freeze. Eventually it might be changed, or made a separate parameter
 	return ct;
	/*Eventually the plan is to have this return a combo count. This not only allows us to display a counter and do whatever scaling/combo 
	mitigation we want to, but also allows us to do things like pushback ramping during blockstrings*/
}

void character::initMoves()
{
	char fname[30];
	char buffer[30];
	char mvName[30];
	ifstream read;
	
	sprintf(fname, "%s/%s.ch", name, name);
	read.open(fname);
	while(read.get() != ':'); read.ignore();
	read >> buffer;
	sprintf(mvName, "%s/%s", name, buffer);
	neutral = new move(mvName);/*
	while(read.peek() != '\n'){
		switch (read.get()){
		case ':':
			break;
		}
	}*/
}

void character::prepHooks(int inputBuffer[30], bool down[5], bool up[5])
{
	move * t = NULL;
	if (cMove == NULL) {
		if(aerial) cMove = /*air*/neutral;
		else {
			if(inputBuffer[0] == 4) cMove = walkBack;
			else if(inputBuffer[0] == 6) cMove = walk;
			else cMove = neutral;
		}
	}
	
	if(aerial) t = airHead->moveHook(inputBuffer, 0, -1, down, up, cMove);
	else t = head->moveHook(inputBuffer, 0, -1, down, up, cMove);

	if(t != NULL){
		if(freeze > 0){
			if(bMove == NULL || (*t) > bMove) bMove = t;
		} 
		else {
			t->execute(cMove);
			cMove = t;
		}
	} else if (bMove != NULL && freeze <= 0) {
		bMove->execute(cMove);
		cMove = bMove;
		bMove = NULL;
	}
}

