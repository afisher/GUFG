#include "interface.h"

void interface::pushInput(int axis[4], int button[5])
{
	int temp = 5 + axis[0]*3 - axis[1]*3 - axis[2] + axis[3];

//	if(temp != 5){	

	inputBuffer[0] = temp;
	
//	pick.head.moveHook(0);

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

move * moveTrie::moveHook(int i, int meter, int state)
{
	move * test;
	if(child != NULL){
		for(i; i < 16; i++){
			for(int j = 0; j < children; j++)
				test = child[j]->moveHook(i, meter, state);
			if(test != NULL) return test;
		}
	}
	if(fish != NULL) 
		if(fish->check(meter, state) == 1) return fish;
	return NULL;
}
