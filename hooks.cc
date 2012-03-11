#include "interface.h"

void interface::pushInput(int axis[4], int down[5], int up[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2]*facing + axis[3]*facing;
	move * t = NULL;
//	if(temp != 5){	
	inputBuffer[0] = temp;
	for(int i = 30; i > 0; i--){
		inputBuffer[i] = inputBuffer[i-1];
	}
	t = pick->head->moveHook(inputBuffer, 0, 0, down, up);
	if(t != NULL && current == NULL) t->execute(current);
	

/*	printf("Current input buffer: ");
	for(int i = 16; i > 0; i--)
		printf("%i", inputBuffer[i]);
	printf("\n");
*/
}

void player::pushInput(int axis[4], int down[5], int up[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2]*facing + axis[3]*facing;
	move * t = NULL;
//	if(temp != 5){	
	inputBuffer[0] = temp;
	for(int i = 30; i > 0; i--){
		inputBuffer[i] = inputBuffer[i-1];
	}
	t = pick->head->moveHook(inputBuffer, 0, 0, down, up);
	if(t != NULL && current == NULL) t->execute(current);
	

/*	printf("Current input buffer: ");
	for(int i = 16; i > 0; i--)
		printf("%i", inputBuffer[i]);
	printf("\n");
*/
}

move * moveTrie::moveHook(int inputBuffer[30], int i, int delta, int pos[5], int neg[5])
{
	moveTrie * test = NULL;
	move * result = NULL;
	int j;
	for(j = i; j < 30; j++){
		test = child[inputBuffer[j]];
		if(test != NULL){
			result = test->moveHook(inputBuffer, j, j-i, pos, neg);
			if(result != NULL) {
				return result;
			}
		}
	}
	if(occupants != 0) 
		for(int i = 0; i < occupants; i++)
			if(fish[i].check(pos, neg) == 1 && delta <= tolerance) return &fish[i];
	return NULL;
}
