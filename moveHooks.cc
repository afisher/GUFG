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
	
//	moveHook();

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

void interface::moveHook(int a, int b, int c, int d, int e)
{
	
	return;
}
