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
	
	t = pick->head->moveHook(inputBuffer, 0, 0, down, up, cMove);
	
	if(t != NULL && current == NULL){
		t->execute(current);
		cMove = t;
	}
	

/*	printf("Current input buffer: ");
	for(int i = 16; i > 0; i--)
		printf("%i", inputBuffer[i]);
	printf("\n");
*/
}

move * moveTrie::moveHook(int inputBuffer[30], int i, int delta, bool pos[5], bool neg[5], move * c)
{
	moveTrie * test = NULL;
	move * result = NULL;
	int j;
	for(j = i; j < 30; j++){
		test = child[inputBuffer[j]];
		if(test != NULL){
			result = test->moveHook(inputBuffer, j, j-i, pos, neg, c);
			if(result != NULL) {
				return result;
			}
		}
	}
	if(occupants != 0) 
		for(int i = 0; i < occupants; i++){
			if(fish[i].check(pos, neg, delta) == 1){
			//Testing!
				if(c == NULL)
					return &fish[i];
				else if(fish[i] == c)
					return &fish[i];
			}
		}
	return NULL;
}
