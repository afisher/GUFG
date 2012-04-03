#include "moveTrie.h"
moveTrie::moveTrie()
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = NULL;
	occupants = 0;
}


moveTrie::moveTrie(move * a)
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = new move*[1];
	fish[0] = a;
	occupants = 1;
}

void moveTrie::insert(move * b)
{
	int i;
	move ** temp;
	occupants++;
	temp = new move*[occupants];
	for(i = 0; i < occupants-1; i++){
		temp[i] = fish[i];
	}
	temp[i] = b;
	fish = temp;
}

moveTrie * moveTrie::insert(int a, move * b)
{
	move * temp;
	if(a < 10 && a > 0){
		if(!child[a]) child[a] = new moveTrie(b);
		else child[a]->insert(b);
		return child[a];
	}
	else return NULL;
}

moveTrie * moveTrie::insert(int a)
{
	if(a < 10 && a > 0) {
		if(!child[a]) child[a] = new moveTrie();
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
	if(fish != NULL) delete [] fish;
	fish = NULL;
}

