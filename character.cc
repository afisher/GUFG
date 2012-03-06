#include "character.h"

character::character() 
//Character constructor. This loads the whole character into memory so that that we don't have disk reads during gameplay
{
	moveTrie * curr;
	move * temp;
	temp = new move("A\0", 5);
	head = new moveTrie(temp);
	curr = head->insert(6);
	temp = new move("236A\0", 236);
	curr = curr->insert(3);
	curr = curr->insert(2, temp); 
	//Empty for now
}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	delete head;
	//Empty for now
}

/*Here begin move functions. Actually contemplating making this a class instead, but this might be simpler for now*/
