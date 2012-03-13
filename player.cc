#include "player.h"
player::player()
{

	/*Initialize input containers*/
	for(int i = 0; i < 30; i++)
		inputBuffer[i] = 5;

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

	facing = 1;
	current = NULL;
	cMove = NULL;
	/*Set up the sprite*/

	deltaX = 0;
	deltaY = 0;
	aerial = 0;
}

void player::keyConfig(int curr)
{
	/*Set up dummy event*/
	SDL_Event temp; 

	/*Flag for breaking the loop*/
	bool configFlag = 0;

	/*Run a simple event poll*/
	while (configFlag == 0){
		if (SDL_PollEvent(&temp)) {
			switch (temp.type) {
			case SDL_JOYAXISMOTION:
				if(temp.jaxis.value != 0)
				{
					input[curr] = temp;
					printf("Set to Joystick %i axis %i value %i\n", temp.jaxis.which, temp.jaxis.axis, temp.jaxis.value);
					configFlag = 1;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				input[curr] = temp;
				printf("Set to Joystick %i button %i\n", temp.jbutton.which, temp.jbutton.button);
				configFlag = 1;
				break;
			case SDL_KEYDOWN:
				input[curr] = temp;
				printf("Set to keyboard %s\n", SDL_GetKeyName(input[curr].key.keysym.sym));
				configFlag = 1;
				break;
			default: 
				break;

			}
		}
	}
}

void player::characterSelect(int i)
{
	pick = NULL;
	switch(i){
	case 1:
		pick = new red;
		break;
	default:
		pick = new character;
		break;
	}
}

