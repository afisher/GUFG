#include "red.h"
red::red()
{
	name = "Red";
	delete head;
	move * temp;
	head = new moveTrie;
	head->insert(new move("Red/A"));
	head->fish[0]->xLock = 1; 
	
	initMoves();
	
	temp = new move("Red/JQ");
	temp->activation = 0;
	head->insert(7, temp);

	temp = new move("Red/JF");
	temp->activation = 0;
	head->insert(9, temp);

	temp = new move("Red/JN");
	temp->activation = 0;
	head->insert(8, temp);

	cMove = neutral;	
}
