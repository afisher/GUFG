#include "trie.h"
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

move * moveTrie::moveHook(int inputBuffer[30], int i, int f, bool pos[5], bool neg[5], move * c)
{
	moveTrie * test = NULL;
	move * result = NULL;
	int j;
	for(j = i; j < 30; j++){
		test = child[inputBuffer[j]];
		if(test != NULL){
			if (f < 0) result = test->moveHook(inputBuffer, j, j, pos, neg, c);
			else result = test->moveHook(inputBuffer, j, f, pos, neg, c);
			if(result != NULL) return result;
		}
	}
	if(occupants != 0){ 
		for(int i = 0; i < occupants; i++){
			if(fish[i]->check(pos, neg, i, f) == 1){
				if((*fish[i]) > c){
					c->init();
					return fish[i];
				}
			}
		}
	}
	return NULL;
}
