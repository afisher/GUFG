#include "character.h"

character::character() 
//Character constructor. This loads the whole character into memory so that that we don't have disk reads during gameplay
{
	//Empty for now
}

character::~character()
//Character destructor. Might not need this if we aren't working with any dynamic memory, but it might be worthwhile to have.
{
	//Empty for now
}

void character::m5A()
{
	//Print statement for now
	printf("5A hook detected");
}

void character::m236A()
{
	//Print statement for now
	printf("236A hook detected");
}
