#include "interface.h"

typedef void (*mv)(void);
void interface::pushInput(int axis[4], int down[5], int up[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2] + axis[3];
	move * t = NULL;
//	if(temp != 5){	
	mv l = NULL;
	inputBuffer[0] = temp;
	if(down[0] == 1 || up[0] == 1) {
		t = pick->head->moveHook(inputBuffer, 0, (void*)l, up, down);
	}
	if(t != NULL) t->execute();
	
	for(int i = 30; i > 0; i--){
		inputBuffer[i] = inputBuffer[i-1];
	}

/*	printf("Current input buffer: ");
	for(int i = 16; i > 0; i--)
		printf("%i", inputBuffer[i]);
	printf("\n");
*/
}

move * moveTrie::moveHook(int inputBuffer[30], int i, void * l, int pos[5], int neg[5])
{
	moveTrie * test = NULL;
	move * result = NULL;
	for(int j = i; j < 30; j++){
		test = child[inputBuffer[j]];
		if(test != NULL){
			result = test->moveHook(inputBuffer, j, l, pos, neg);
			if(result != NULL) {
				return result;
			}
		}
	}
	if(fish != NULL) 
		if(fish->check() == 1) return fish;
	return NULL;
}
