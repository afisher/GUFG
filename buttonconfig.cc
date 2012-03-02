#include "interface.h"
interface::interface()
{
	/*Yeah yeah, I know, char* to literal conversion. I'm lazy right now. Will fix later. Maybe with cstring*/
	inputName[0] = "Up\0";
	inputName[1] = "Down\0";
	inputName[2] = "Left\0";
	inputName[3] = "Right\0";
	inputName[4] = "A\0";
	inputName[5] = "B\0";
	inputName[6] = "C\0";
	inputName[7] = "D\0";
	inputName[8] = "E\0";
	inputName[9] = "Start\0";

	/*Set up ALL the inputs*/
	for(int i = 0; i < 10; i++){
		printf("Please enter a command for %s\n", inputName[i]);
		keyConfig(i);
	}
}

void interface::keyConfig(int current)
{
	/*Set up dummy event*/
	SDL_Event temp; 

	/*Ghetto flag for seeing if we have already config'd*/
	bool configFlag = 0;

	/*Run a simple event poll*/
	while (configFlag == 0){
		if (SDL_PollEvent(&temp)) {
			switch (temp.type) {
			case SDL_KEYDOWN:
				input[current] = temp;
				printf("Set to keyboard %s\n", SDL_GetKeyName(input[current].key.keysym.sym));
				configFlag = 1;
				break;
			default: 
				break;

			}
		}
	}
	configFlag == 0;
/*
	printf("Enter a key for Down:\n");
	printf("Enter a key for Left:\n");
	printf("Enter a key for Right:\n");
	printf("Enter a key for A:\n");
	printf("Enter a key for B:\n");
	printf("Enter a key for C:\n");
	printf("Enter a key for D:\n");
	printf("Enter a key for E:\n");
	printf("Enter a key for Start:\n");
//*/
}
