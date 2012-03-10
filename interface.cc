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
	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_JOYSTICK);
	/*WM stuff blah blah*/
	SDL_WM_SetCaption("GUFG", "GUFG");
	screen = SDL_SetVideoMode(640, 480, 0, 0);

	current = NULL;
	/*Set up the p1sprite*/
	spriteInit();

	/*Background color, temporary until we have backgrounds*/
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));

	/*Flag to kill the game*/
	gameover = 0;

	/*Build the character. Eventually this will probably be a function.*/
	pick = new character;

	/*Set up input buffers and joysticks*/
	for(int i = 0; i < SDL_NumJoysticks(); i++)
		SDL_JoystickOpen(i);
	for(int i = 0; i < 30; i++)
		inputBuffer[i] = 5;

	/*Initialize input containers*/
	for(int i = 0; i < 4; i++) 
		sAxis[i] = 0;
	for(int i = 0; i < 5; i++){
		posEdge[i] = 0;
		negEdge[i] = 0;
	}

	deltaX = 0;
	s1Rect.x = 200;
	s1Rect.y = 330;
	deltaY = 0;
	aerial = 0;
	grav = 3;
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

void interface::keyConfig(int curr)
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

void interface::runTimer()
{
	if(timer > 0) timer--;
}

void interface::resolve()
{
	/* Movement currently determined by static deltas */
	if(s1Rect.y < 480 - s1Rect.h) aerial = 1;
	s1Rect.x += deltaX;
	s1Rect.y += deltaY;

	/* No escaping the screen */
	if (s1Rect.x < 0)
		s1Rect.x = 0;
	else if (s1Rect.x > 640 - s1Rect.w)
		s1Rect.x = 640 - s1Rect.w;
	if (s1Rect.y < 0)
		s1Rect.y = 0;
	else if (s1Rect.y > 330)
		s1Rect.y = 330;

	/*Enforcing gravity*/
	if(s1Rect.y == 330 && aerial == 1)
		aerial = 0;
	if(!aerial){
		if(sAxis[0]) deltaY = -35;
		else deltaY = 0;
		if(sAxis[3]) deltaX = 5;
		if(sAxis[2]) deltaX = -5;
		if((!sAxis[2] && !sAxis[3]) || sAxis[1] == 1) deltaX = 0;
	}
	if(aerial) deltaY += grav;
	for(int i = 0; i < 5; i++){
		posEdge[i] = 0;
		negEdge[i] = 0;
	}
	if(current != NULL){
		p1sprite = SDL_DisplayFormat(current->sprite);
		SDL_SetColorKey(p1sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
		current = current->next;
		sFlag = 0;
/*		Testing stuff, to be deleted later.
		deltaX = 0;
//*/
	}
	else if(!sFlag) spriteInit();
	runTimer();
}

void interface::readInput()

{
	/*Make our dummy event for polls*/
	SDL_Event event;
	for(int i = 0; i < 14; i++){
		if (SDL_PollEvent(&event)){
			/*Do stuff with event*/
			switch (event.type){
			/*Kill handler*/
			case SDL_QUIT:
				gameover = 1;
				break;
				/*Keyboard handler. Maybe I'll optimize such that the knows if it even needs to check this (EG if sticks are used)*/
			case SDL_JOYAXISMOTION:
				for(int i = 0; i < 4; i++)
					if(event.jaxis.which == input[i].jaxis.which && event.jaxis.axis == input[i].jaxis.axis && event.jaxis.value == input[i].jaxis.value)
						sAxis[i] = 1;
					for(int i = 0; i < 4; i++)
					if(event.jaxis.which == input[i].jaxis.which && event.jaxis.axis == input[i].jaxis.axis && event.jaxis.value == 0)
						sAxis[i] = 0;
				break;
			case SDL_JOYBUTTONDOWN:
				for(int i = 4; i < 9; i++)
					if(event.jbutton.which == input[i].jbutton.which && event.jbutton.button == input[i].jbutton.button)
						posEdge[i-4] = 1;
				break;
			case SDL_JOYBUTTONUP:
				for(int i = 4; i < 9; i++)
					if(event.jbutton.which == input[i].jbutton.which && event.jbutton.button == input[i].jbutton.button)
						negEdge[i-4] = 1;
				break;
			case SDL_KEYDOWN:
				for(int i = 0; i < 4; i++)
					if(event.key.keysym.sym == input[i].key.keysym.sym) 
						sAxis[i] = 1;
					for(int i = 4; i < 9; i++)
					if(event.key.keysym.sym == input[i].key.keysym.sym)
						posEdge[i-4] = 1;
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				for(int i = 0; i < 4; i++)
					if(event.key.keysym.sym == input[i].key.keysym.sym)
						sAxis[i] = 0;
					for(int i = 4; i < 9; i++)
					if(event.key.keysym.sym == input[i].key.keysym.sym)
						negEdge[i-4] = 1;
				break;
			}
		}
	}
}

void interface::draw()
{
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_BlitSurface(p1sprite, NULL, screen, &s1Rect);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void interface::spriteInit()
{
	SDL_Surface *sTemp = SDL_LoadBMP("SP.bmp");
	p1sprite = SDL_DisplayFormat(sTemp);
	SDL_FreeSurface(sTemp);
	/*Ghetto alpha-value. Not sure why we can't alpha value. This might change*/

	/*Set the color key*/
	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);
	SDL_SetColorKey(p1sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	sFlag = 1;
}
