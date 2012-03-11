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
#include <cstring>
interface::interface()
{
	/*Start SDL*/
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_JOYSTICK);
	/*WM stuff blah blah*/
	SDL_WM_SetCaption("GUFG", "GUFG");
	screen = SDL_SetVideoMode(640, 480, 0, 0);

	/*Set up input buffers and joysticks*/
	for(int i = 0; i < SDL_NumJoysticks(); i++)
		SDL_JoystickOpen(i);

	for(int i = 0; i < 30; i++)
		inputBuffer[i] = 5;

	facing = 1;
	printf("Player 1:\n");
	p1 = new player;

/*	printf("Player 2:\n");
	p2 = new player;

	/*Build the character. Eventually this will probably be a function.*/
	pick = new character;
	p1sprite = NULL;
	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);
	current = NULL;

	/*Background color, temporary until we have backgrounds*/
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));

	/*Flag to kill the game*/
	gameover = 0;



	/*Initialize input containers*/
	for(int i = 0; i < 4; i++) 
	{
		sAxis1[i] = 0;
		sAxis2[i] = 0;
	}
	for(int i = 0; i < 5; i++){
		posEdge1[i] = 0;
		negEdge1[i] = 0;
		posEdge2[i] = 0;
		negEdge2[i] = 0;
	}

	deltaX = 0;
	s1Rect.x = 100;
	s1Rect.y = 330;
	deltaY = 0;
	aerial = 0;
	grav = 3;
	timer = 5824;
	spriteInit();
	draw();
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
	if(s1Rect.y + s1Rect.h < 480) aerial = 1;
	s1Rect.x += deltaX;
	s1Rect.y += deltaY;

	/* No escaping the screen */
	if (s1Rect.x < 0)
		s1Rect.x = 0;
	else if (s1Rect.x + s1Rect.w > 640)
		s1Rect.x = 640 - s1Rect.w;
	if (s1Rect.y < 0)
		s1Rect.y = 0;
	else if (s1Rect.y + s1Rect.h > 480)
		s1Rect.y = 480 - s1Rect.h;
	if (s1Rect.x < 250 && p1->facing == -1) { p1->facing = 1; sFlag = 0;}
	else if (s1Rect.x > 250 && p1->facing == 1) { p1->facing = -1; sFlag = 0;}
	if (s1Rect.x < 250 && facing == -1) { facing = 1; sFlag = 0;}
	else if (s1Rect.x > 250 && facing == 1) { facing = -1; sFlag = 0;}

	/*Enforcing gravity*/
	if(s1Rect.y + s1Rect.h == 480 && aerial == 1)
		aerial = 0;
	if(!aerial){
		if(sAxis1[0]) deltaY = -35;
		else deltaY = 0;
		if(sAxis1[3]) deltaX = 5;
		if(sAxis1[2]) deltaX = -5;
		if((!sAxis1[2] && !sAxis1[3]) || sAxis1[1] == 1) deltaX = 0;
	}
	if(aerial) deltaY += grav;
	
	/*Reinitialize inputs*/
	for(int i = 0; i < 5; i++){
		posEdge1[i] = 0;
		negEdge1[i] = 0;
	}

	/*Doing moves*/
	if(p1->current != NULL){
		int displacement = p1sprite->w;
		if(facing == -1) { 
			p1sprite = SDL_DisplayFormat(p1->current->fSprite);
			s1Rect.x += (displacement - p1sprite->w);
		}
		else p1sprite = SDL_DisplayFormat(p1->current->sprite);
		p1->current = p1->current->next;
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
					if(event.jaxis.which == p1->input[i].jaxis.which && event.jaxis.axis == input[i].jaxis.axis && event.jaxis.value == input[i].jaxis.value)
						sAxis1[i] = 1;
					for(int i = 0; i < 4; i++)
					if(event.jaxis.which == p1->input[i].jaxis.which && event.jaxis.axis == input[i].jaxis.axis && event.jaxis.value == 0)
						sAxis1[i] = 0;
				break;
			case SDL_JOYBUTTONDOWN:
				for(int i = 4; i < 9; i++)
					if(event.jbutton.which == p1->input[i].jbutton.which && event.jbutton.button == input[i].jbutton.button)
						posEdge1[i-4] = 1;
				break;
			case SDL_JOYBUTTONUP:
				for(int i = 4; i < 9; i++)
					if(event.jbutton.which == p1->input[i].jbutton.which && event.jbutton.button == input[i].jbutton.button)
						negEdge1[i-4] = 1;
				break;
			case SDL_KEYDOWN:
				for(int i = 0; i < 4; i++)
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym) 
						sAxis1[i] = 1;
					for(int i = 4; i < 9; i++)
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym)
						posEdge1[i-4] = 1;
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
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym)
						sAxis1[i] = 0;
					for(int i = 4; i < 9; i++)
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym)
						negEdge1[i-4] = 1;
				break;
			}
		}
	}
}

void interface::draw()
{
	SDL_SetColorKey(p1sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_BlitSurface(p1sprite, NULL, screen, &s1Rect);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void interface::spriteInit()
{
	char nsprt[strlen(pick->name)+4];
	strcpy(nsprt, pick->name);
	strcat(nsprt, "/");
	strcat(nsprt, "N");
	if(facing == -1) strcat(nsprt, "F");
	int displacement;
	if(p1sprite) displacement = p1sprite->w;
	SDL_Surface *sTemp = SDL_LoadBMP(nsprt);
	p1sprite = SDL_DisplayFormat(sTemp);
	if(facing == -1)
		s1Rect.x += (displacement - p1sprite->w);
	SDL_FreeSurface(sTemp);
	/*Ghetto alpha-value. Not sure why we can't alpha value. This might change*/

	/*Set the color key*/
	sFlag = 1;
}

