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

	/*Currently I'm using this as a test case for my move hooks*/
	
	head = new moveTrie(new move("White/A", "A", 0, 11));
	head->fish[0].debugStateInit(3, 16, 30);
	head->fish[0].debugRectsInit();
	head->fish[0].debugDamageInit(10);
	head->fish[0].debugHitboxInit(55, 45, 30, 30);
	head->fish[0].debugHittableInit(0, 0, 100, 150);
	head->fish[0].xLock = 1;
	head->fish[0].yLock = 1;


	head->insert(new move("B", "B", 0));
	head->fish[1].debugStateInit(1, 16, 30);
	head->fish[1].debugRectsInit();
	
	head->insert(new move("C", "C", 0));
	head->fish[2].debugStateInit(1, 16, 30);
	head->fish[2].debugRectsInit();
	
	head->insert(new move("White/D", "D", 0, 1));
	head->fish[3].debugStateInit(3, 16, 30);
	head->fish[3].debugRectsInit();
	
	head->insert(new move("E", "E", 0));
	head->fish[4].debugStateInit(1, 16, 30);
	head->fish[4].debugRectsInit();
	
	curr = head->insert(6);
	curr = curr->insert(3);
	curr = curr->insert(2, new move("236B", "B", 1));
	curr->fish[0].debugStateInit(8, 16, 16);
	curr->fish[0].debugRectsInit();

	neutral = new move("White/N", 1);
	neutral->debugRectsInit();
	neutral->debugStateInit(1, 31, 31);
	neutral->debugHittableInit(0, 0, 100, 150);

	cMove = neutral;
	health = 300;
	meter = 0;
}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	delete head;
	//Empty for now
}

/*Here begin move functions. Actually contemplating making this a class instead, but this might be simpler for now*/

int * character::takeHit(move * attack)
{
	/*All the important logic like blocking and stuff will go here later.*/

	/*Damage scaling logic will factor into this later*/
	health -= attack->damage;
	if(health < 0) health = 0;
	attack->connect();
	/*It will eventually return the "move" that the hit causes, usually a hit- or blockstun animation*/
	return NULL;
}
