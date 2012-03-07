#include "character.h"

character::character() 
//Character constructor. This loads the whole character into memory so that that we don't have disk reads during gameplay
{
	moveTrie * curr;
	move * temp;
	
	temp = new move("A\0", "A\0", 0);
	head = new moveTrie(temp);

	temp = new move("6E\0", "E\0", 0);
	curr = head->insert(6, temp);

	temp = new move("236A\0", "A\0", 1);
	curr = curr->insert(3);
	curr->insert(2, temp); 
	temp = new move("236B\0", "B\0", 1);
	curr = curr->insert(2, temp);
	
	temp = new move("623B\0", "B\0", 1);
	curr = head->insert(3);
	curr = curr->insert(2);
	curr = curr->insert(6, temp);

	temp = new move("236236A\0", "A\0", 1);
	curr = curr->insert(6);
	curr = curr->insert(3);
	curr = curr->insert(2, temp);

	curr = head->insert(2);
	temp = new move("22C\0", "C\0", 1);
	curr = curr->insert(5);
	curr = curr->insert(2, temp);

	curr = head->insert(4);
	temp = new move("214D\0", "D\0", 1);
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
