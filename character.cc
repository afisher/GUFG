#include "character.h"

character::character() 
//Character constructor. This loads the whole character into memory so that that we don't have disk reads during gameplay
{
	name = "White";
	/*Currently I'm using this as a test case for my move hooks*/
	moveTrie * curr;

	head = new moveTrie(new move("White/A\0", "A\0", 0, 11));
	head->insert(new move("B\0", "B\0", 0));
	head->insert(new move("C\0", "C\0", 0));
	head->insert(new move("White/D\0", "D\0", 0, 0));
	head->insert(new move("E\0", "E\0", 0));

	curr = head->insert(6, new move("6B\0", "B\0", 0), 1);	
	curr = head->insert(6);
	curr = curr->insert(3);
	curr = curr->insert(2, new move("236A\0", "A\0", 1));

	curr = head->insert(4);
	curr = curr->insert(1);
	curr = curr->insert(2, new move("214B\0", "B\0", 1));

}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	delete head;
	//Empty for now
}

/*Here begin move functions. Actually contemplating making this a class instead, but this might be simpler for now*/
