#include "move.h"
move::move()
{
	name = NULL;
	cost = 0;
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

int move::check(/*int meter, int state*/)
{
	//if(meter < cost) return 0;
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
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = a;
}

moveTrie * moveTrie::insert(int a, move * b)
{
	if(a < 10 && a > 0){
		child[a] = new moveTrie(b);
		return child[a];
	}
	else return NULL;
}

moveTrie * moveTrie::insert(int a)
{
	if(a < 10 && a > 0) {
		child[a] = new moveTrie();
		return child[a];
	}
	else return NULL;
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
