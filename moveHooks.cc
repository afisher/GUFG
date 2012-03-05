#include "interface.h"

void interface::pushInput(SDL_Event & tick, int up, int down, int left, int right)
{
	int temp = 5;
	if(up == 1) temp += 3;
	if(down == 1) temp -= 3;
	if(left == 1) temp -= 1;
	if(right == 1) temp += 1;

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

move * moveTrie::moveHook(int i)
{
	move * test;
	for(i; i < 16; i++){
		for(int j = 0; j < 13; j++)
			test = child[j]->moveHook(i);
		if(test != NULL) return test;
	}
	if(fish != NULL) return fish;
	else return NULL;
}
