#include "interface.h"

void interface::pushInput(int axis[4], int button[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2] + axis[3];
	move * t = NULL;
//	if(temp != 5){	

	inputBuffer[0] = temp;

	if(button[0] == 1) t = pick->head->moveHook(inputBuffer, 0);
	if(t != NULL) t->execute();
	
	for(int i = 15; i >= 0; i--){
		if(inputBuffer[i] != 0) inputBuffer[i+1] = inputBuffer[i];
	}
	inputBuffer[0] = 0;

/*	printf("Current input buffer: ");
	for(int i = 16; i > 0; i--)
		printf("%i", inputBuffer[i]);
	printf("\n");
*/
}

move * moveTrie::moveHook(int inputBuffer[16], int i)
{
	moveTrie * test;
	move * result;
	for(i; i < 16; i++){
		test = child[inputBuffer[i]];
		if(test != NULL){
			result = test->moveHook(inputBuffer, i);
			if(result != NULL) return result;
		}
	}
	if(fish != NULL) 
		if(fish->check(0, 0) == 1) return fish;
	return NULL;
}
