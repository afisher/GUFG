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
	
	curr = head->insert(2);
	temp = new move("252A\0", 252);
	curr = curr->insert(5);
	curr = curr->insert(2, temp);

	curr = head->insert(4);
	temp = new move("214A\0", 214);
	curr = curr->insert(1);
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
