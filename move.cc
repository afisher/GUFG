#include "move.h"
#include <cstring>
move::move()
{
	name = NULL;
	cost = 0;
}

move::move(char * n)
{
	name = n;
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	special = 0;
}

move::move(char* n, char * b, bool s)
{
	int r = strlen(b);
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	name = n;
	for(int i = 0; i < r; i++){
		switch(b[i]){
		case 'A':
			button[0] = 1;
			break;
		case 'B':
			button[1] = 1;
			break;
		case 'C':
			button[2] = 1;
			break;
		case 'D':
			button[3] = 1;
			break;
		case 'E':
			button[4] = 1;
			break;
		}

	}
	special = s;
				
}

move::~move()
{
	delete [] name;
}

int move::check(int pos[5], int neg[5])
{
	//if(meter < cost) return 0;
//	if(state != allowedState) return 0;
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(special){
				if(pos[i] == 0 && neg[i] == 0) return 0;
			}
			else if(pos[i] == 0) return 0;
		}
				
	}
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
	tolerance = 30;
	occupants = 0;
}





moveTrie::moveTrie(move * a)
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = a;
	tolerance = 30;
	occupants = 1;
}

moveTrie::moveTrie(move * a, int t)
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = a;
	tolerance = t;
	occupants = 1;
}

moveTrie * moveTrie::insert(int a, move * b, int t)
{
	move * temp;
	if(a < 10 && a > 0){
		if(!child[a]) child[a] = new moveTrie(b, t);
		else child[a]->insert(b);
		return child[a];
	}
	else return NULL;
}

void moveTrie::insert(move * b)
{
	int i;
	move * temp;
	occupants++;
	temp = new move[occupants];
	for(i = 0; i < occupants-1; i++)
		temp[i] = fish[i];
	temp[i] = *b;
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
	delete fish;
	fish = NULL;
}
