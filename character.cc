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
	head->fish[0].debugStateInit(1, 0);
	head->fish[0].debugRectsInit();
	
	head->insert(new move("B", "B", 0));
	head->fish[1].debugStateInit(1, 0);
	head->fish[1].debugRectsInit();
	
	head->insert(new move("C", "C", 0));
	head->fish[2].debugStateInit(1, 0);
	head->fish[2].debugRectsInit();
	
	head->insert(new move("White/D", "D", 0, 0));
	head->fish[3].debugStateInit(1, 1);
	head->fish[3].debugRectsInit();
	
	head->insert(new move("E", "E", 0));
	head->fish[4].debugStateInit(1, 0);
	head->fish[4].debugRectsInit();
	
	curr = head->insert(6);
	curr = curr->insert(3);
	curr = curr->insert(2, new move("236B", "B", 1));
	curr->fish[0].debugStateInit(1, 1);
	curr->fish[0].debugRectsInit();

}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	delete head;
	//Empty for now
}

/*Here begin move functions. Actually contemplating making this a class instead, but this might be simpler for now*/

