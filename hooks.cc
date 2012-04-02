/*Move hook infrastructure file for project: GUFG
 *
 *Written by Alex Kelly in 2012 under MIT OSI License
 *See the file COPYING for details
 */
#include "interface.h"

void player::pushInput(bool axis[4], bool down[5], bool up[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2]*facing + axis[3]*facing;
	move * t = NULL;
//	if(temp != 5){	
	inputBuffer[0] = temp;

	for(int i = 30; i > 0; i--){
		inputBuffer[i] = inputBuffer[i-1];
	}
	
	if(pick->aerial) t = pick->airHead->moveHook(inputBuffer, 0, -1, down, up, pick->cMove);
	else t = pick->head->moveHook(inputBuffer, 0, -1, down, up, pick->cMove);

	if(t != NULL){ 
		pick->cMove = t;
	}
//*	
	else if (pick->cMove == NULL) {
		if(!pick->aerial){
			if(inputBuffer[0] == 4) pick->cMove = pick->walkBack;
			else if(inputBuffer[0] == 6) pick->cMove = pick->walk;
			else pick->cMove = pick->neutral;
		}
	}
//*/	
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
			if(fish[i].check(pos, neg, i, f) == 1){
				if(c == NULL)
					return &fish[i];
				else if(fish[i] > c){
					c->init();
					return &fish[i];
				}
			}
		}
	}
	return NULL;
}
