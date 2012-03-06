#include "move.h"
move::move()
{
	name = NULL;
}

move::move(char * n, int i)
{
	name = n;
	input = i;
}

move::~move()
{
	delete [] name;
}

int move::check(int meter, int state)
{
	if(meter < cost) return 0;
//	if(state != allowedState) return 0;
	return 1;
}

void move::execute()
{
	printf("Hook for %s detected\n", name);
}

moveTrie::moveTrie()
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = NULL;
}

moveTrie::moveTrie(move * a)
{
	fish = a;
	for(int i = 0; i < 10; i++)
	child[i] = NULL;
}

moveTrie * moveTrie::insert(int a, move * b)
{
	if(a > 9 || a < 1);
	else child[a] = new moveTrie(b);
	return child[a];
}

moveTrie * moveTrie::insert(int a)
{
	if(a > 9 || a < 1);
	else child[a] = new moveTrie();
	return child[a];
}

moveTrie::~moveTrie()
{
	for(int i = 0; i < 9; i++){
		if(child[i] != NULL){
			delete child[i];
			child[i] = NULL;
		}
	}
	delete fish;
	fish = NULL;
}
