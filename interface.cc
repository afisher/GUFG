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

	printf("Player 1:\n");
	p1 = new player;

	printf("Player 2:\n");
	p2 = new player;

	/*Build the character. Eventually this will probably be a function.*/
	p1sprite = NULL;
	p2sprite = NULL;
	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);

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

	deltaX1 = 0;
	deltaY1 = 0;
	deltaX2 = 0;
	deltaY2 = 0;
	grav = 3;
	timer = 5824;
	sFlag1 = 0; 
	sFlag2 = 0;
	spriteInit();
	pos1.x = 100;
	pos2.x = 475;
	pos1.y = 330;
	pos2.y = 330;
	draw();
}


void interface::runTimer()
{
	if(timer > 0) timer--;
}

void interface::resolve()
{
	/* Movement currently determined by static deltas */
	if(pos1.y + pos1.h < 480) p1->aerial = 1;
	if(pos2.y + pos2.h < 480) p2->aerial = 1;
	pos1.x += deltaX1;
	pos1.y += deltaY1;
	pos2.x += deltaX2;
	pos2.y += deltaY2;


	/* No escaping the screen */
	if (pos1.x < 0)
		pos1.x = 0;
	else if (pos1.x + pos1.w > 640)
		pos1.x = 640 - pos1.w;
	if (pos1.y < 0)
		pos1.y = 0;
	else if (pos1.y + pos1.h > 480)
		pos1.y = 480 - pos1.h;

			/*Player 2*/

	if (pos2.x < 0)
		pos2.x = 0;
	else if (pos2.x + pos2.w > 640)
		pos2.x = 640 - pos2.w;
	if (pos1.y < 0)
		pos2.y = 0;
	else if (pos2.y + pos2.h > 480)
		pos2.y = 480 - pos2.h;

	/*Enforcing gravity*/
	if(pos1.y + pos1.h == 480 && p1->aerial == 1)
		p1->aerial = 0;
	if(!p1->aerial){
		if(sAxis1[0]) deltaY1 = -35;
		else deltaY1 = 0;
		if(sAxis1[3]) deltaX1 = 5;
		if(sAxis1[2]) deltaX1 = -5;
		if((!sAxis1[2] && !sAxis1[3]) || sAxis1[1] == 1) deltaX1 = 0;
		if (pos1.x < pos2.x && p1->facing == -1) { p1->facing = 1; sFlag1 = 0;}
		else if (pos1.x > pos2.x && p1->facing == 1) { p1->facing = -1; sFlag1 = 0; }
	}
	if(p1->aerial) deltaY1 += grav;

			/*Player 2*/


	if(pos2.y + pos2.h == 480 && p2->aerial == 1)
		p2->aerial = 0;
	if(!p2->aerial){
		if(sAxis2[0]) deltaY2 = -35;
		else deltaY2 = 0;
		if(sAxis2[3]) deltaX2 = 5;
		if(sAxis2[2]) deltaX2 = -5;
		if((!sAxis2[2] && !sAxis2[3]) || sAxis2[1] == 1) deltaX2 = 0;
		if (pos2.x < pos1.x && p2->facing == -1) { p2->facing = 1; sFlag2 = 0;}
		else if (pos2.x > pos1.x && p2->facing == 1) { p2->facing = -1; sFlag2 = 0; }
	}
	if(p2->aerial) deltaY2 += grav;

	/*Reinitialize inputs*/
	for(int i = 0; i < 5; i++){
		posEdge1[i] = 0;
		posEdge2[i] = 0;
		negEdge1[i] = 0;
		negEdge2[i] = 0;
	}

	spriteInit();
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
				for(int i = 0; i < 4; i++){
					if(event.jaxis.which == p1->input[i].jaxis.which && event.jaxis.axis == p1->input[i].jaxis.axis && event.jaxis.value == p1->input[i].jaxis.value)
						sAxis1[i] = 1;
					if(event.jaxis.which == p2->input[i].jaxis.which && event.jaxis.axis == p2->input[i].jaxis.axis && event.jaxis.value == p2->input[i].jaxis.value)
						sAxis2[i] = 1;
					if(event.jaxis.which == p1->input[i].jaxis.which && event.jaxis.axis == p1->input[i].jaxis.axis && event.jaxis.value == 0)
						sAxis1[i] = 0;
					if(event.jaxis.which == p2->input[i].jaxis.which && event.jaxis.axis == p2->input[i].jaxis.axis && event.jaxis.value == 0)
						sAxis2[i] = 0;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				for(int i = 4; i < 9; i++){
					if(event.jbutton.which == p1->input[i].jbutton.which && event.jbutton.button == p1->input[i].jbutton.button)
						posEdge1[i-4] = 1;
					if(event.jbutton.which == p2->input[i].jbutton.which && event.jbutton.button == p2->input[i].jbutton.button)
						posEdge2[i-4] = 1;
				}
				break;
			case SDL_JOYBUTTONUP:
				for(int i = 4; i < 9; i++){
					if(event.jbutton.which == p1->input[i].jbutton.which && event.jbutton.button == p1->input[i].jbutton.button)
						negEdge1[i-4] = 1;
					if(event.jbutton.which == p2->input[i].jbutton.which && event.jbutton.button == p2->input[i].jbutton.button)
						negEdge2[i-4] = 1;
				}
				break;
			case SDL_KEYDOWN:
				for(int i = 0; i < 4; i++) {
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym) 
						sAxis1[i] = 1;
					if(event.key.keysym.sym == p2->input[i].key.keysym.sym) 
						sAxis2[i] = 1;
				}
				for(int i = 4; i < 9; i++) {
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym)
						posEdge1[i-4] = 1;
					if(event.key.keysym.sym == p2->input[i].key.keysym.sym)
						posEdge2[i-4] = 1;
				}
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
				for(int i = 0; i < 4; i++){
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym)
						sAxis1[i] = 0;
					if(event.key.keysym.sym == p2->input[i].key.keysym.sym)
						sAxis2[i] = 0;
				}
				for(int i = 4; i < 9; i++){
					if(event.key.keysym.sym == p1->input[i].key.keysym.sym)
						negEdge1[i-4] = 1;
					if(event.key.keysym.sym == p2->input[i].key.keysym.sym)
						negEdge2[i-4] = 1;
				}
				break;
			}
		}
	}
}

void interface::draw()
{
	SDL_SetColorKey(p1sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_SetColorKey(p2sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_BlitSurface(p1sprite, NULL, screen, &pos1);
	SDL_BlitSurface(p2sprite, NULL, screen, &pos2);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void interface::spriteInit()
{
	int displacement;
	SDL_Surface *sTemp;
	if(p1sprite) displacement = p1sprite->w;
	/*Doing moves*/
	if(p1->current != NULL){
		if(p1->facing == -1) { 
			p1sprite = SDL_DisplayFormat(p1->current->fSprite);
			pos1.x += (displacement - p1sprite->w);
		}
		else p1sprite = SDL_DisplayFormat(p1->current->sprite);
		p1->current = p1->current->next;
		sFlag1 = 0;
	}
	else if(!sFlag1){
		char nsprt[strlen(p1->pick->name)+4];
		strcpy(nsprt, p1->pick->name);
		strcat(nsprt, "/");
		strcat(nsprt, "N");
		if(p1->facing == -1) strcat(nsprt, "F");
		strcat(nsprt, "\0");
		sTemp = SDL_LoadBMP(nsprt);
		p1sprite = SDL_DisplayFormat(sTemp);
		if(p1->facing == -1)
			if (pos1.x < pos2.x && p1->facing == -1) { p1->facing = 1; sFlag1 = 0;}
		else if (pos1.x > pos2.x && p1->facing == 1) { p1->facing = -1; sFlag1 = 0;}
			pos1.x += (displacement - p1sprite->w);
		SDL_FreeSurface(sTemp);
		sFlag1 = 1;
	}

	if(p2sprite) displacement = p2sprite->w;
	if(p2->current != NULL){
		if(p2->facing == -1) { 
			p2sprite = SDL_DisplayFormat(p2->current->fSprite);
			pos2.x += (displacement - p2sprite->w);
		}
		else p2sprite = SDL_DisplayFormat(p2->current->sprite);
		p2->current = p2->current->next;
		sFlag2 = 0;
	}
	else if (!sFlag2){
		char nsprt[strlen(p2->pick->name)+4];
		strcpy(nsprt, p2->pick->name);
		strcat(nsprt, "/");
		strcat(nsprt, "N");
		if(p2->facing == -1) strcat(nsprt, "F");
		strcat(nsprt, "\0");
		sTemp = SDL_LoadBMP(nsprt);
		p2sprite = SDL_DisplayFormat(sTemp);
		if(p2->facing == -1)
			if (pos2.x < pos1.x && p2->facing == -1) { p2->facing = 1; sFlag2 = 0;}
		else if (pos2.x > pos1.x && p2->facing == 1) { p2->facing = -1; sFlag2 = 0;}
			pos2.x += (displacement - p2sprite->w);
		SDL_FreeSurface(sTemp);
		sFlag2 = 1;
	}
}

