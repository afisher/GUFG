/*Button Config and Interface Constructor for GUFG
 *This is intended as a configuration for inputs within GUFG
 *It currently also includes a constructor for the interface struct/class
 *Which currently just names the inputs and does a call-and-response loop
 *in a terminal that sets up the buttons for one player.
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "interface.h"
interface::interface()
{
	pick = new character;
	for(int i = 0; i < 15; i++)
		inputBuffer[i] = 5;
	timer = 5824;
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

	/*Flag for breaking the loop*/
	bool configFlag = 0;

	/*Run a simple event poll*/
	while (configFlag == 0){
		if (SDL_PollEvent(&temp)) {
			switch (temp.type) {
			case SDL_JOYAXISMOTION:
				if(temp.jaxis.value != 0)
				{
					input[current] = temp;
					printf("Set to Joystick %i axis %i value %i\n", temp.jaxis.which, temp.jaxis.axis, temp.jaxis.value);
					configFlag = 1;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				{
					input[current] = temp;
					printf("Set to Joystick %i button %i\n", temp.jbutton.which, temp.jbutton.button);
					configFlag = 1;
				}
				break;
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
}

void interface::runTimer()
{
	if(timer > 0) timer--;
}

