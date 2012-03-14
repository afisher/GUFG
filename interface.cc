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
#include <math.h>
interface::interface()
{
	/*Start SDL*/
	screenWidth = 800;
	screenHeight = 600;
	floor = screenHeight - 50;
	wall = 50;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_JOYSTICK);
	/*WM stuff blah blah*/
	SDL_WM_SetCaption("GUFG", "GUFG");
	screen = SDL_SetVideoMode(screenWidth, screenHeight, 0, 0);

	/*Set up input buffers and joysticks*/
	for(int i = 0; i < SDL_NumJoysticks(); i++)
		SDL_JoystickOpen(i);

	printf("Player 1:\n");
	p1 = new player;

	printf("Player 2:\n");
	p2 = new player;

	/*Temporarily, this is where we do character select. Obviously this will become a menu later*/
	p1->characterSelect(0);
	p2->characterSelect(1);

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
	p1->sFlag = 0; 
	p2->sFlag = 0;
	p2->facing = -1;
	p1->facing = 1;
	spriteInit();
	p1->pos.x = wall*4;
	p2->pos.x = screenWidth - wall*4 - p2sprite->w;
	p1->pos.y = floor - p1sprite->h;
	p2->pos.y = floor - p2sprite->h;
	draw();
}


void interface::runTimer()
{
	if(timer > 0) timer--;
}

void interface::resolve()
{
	
	SDL_Rect hitbox1, hittable1, delta1, collision1;
	SDL_Rect hitbox2, hittable2, delta2, collision2;
	frame * dummy;
	if(p1->cMove != NULL) {
		if(!p1->cMove->step(delta1, hitbox1, hittable1, collision1, dummy)) p1->cMove = NULL;
		else {
			if(p1->cMove->xLock) deltaX1 = delta1.x;
			if(p1->cMove->yLock) deltaY1 = delta1.y;
		}
	}
	if(p2->cMove != NULL) { 
		if(!p2->cMove->step(delta2, hitbox2, hittable2, collision1, dummy)) p2->cMove = NULL;
		else {
			if(p2->cMove->xLock) deltaX2 = delta2.x;
			if(p2->cMove->yLock) deltaY2 = delta2.y;
		}
	}
			

	/* Movement currently determined by static deltas */
	if(p1->pos.y + p1->pos.h < floor) p1->aerial = 1; 
	if(p2->pos.y + p2->pos.h < floor) p2->aerial = 1;
	bool collision = 0;

	SDL_Rect a = p1->pos, b = p2->pos;
	a.x += deltaX1;
	b.x += deltaX2;
	a.y += deltaY1;
	b.y += deltaY2;
	bool lock1, lock2 = 0;
	collision = hit(a, b);
	/* Some collision */

	/* Floor and Cieling */
	if (p2->pos.y + deltaY2 <= 0)
		p2->pos.y = 0;
	else if (p2->pos.y + deltaY2 + p2->pos.h >= floor)
		p2->pos.y = floor - p2->pos.h;
	else p2->pos.y += deltaY2;
	
	
	
	if (p1->pos.y + deltaY1 <= 0)
		p1->pos.y = 0;
	else if (p1->pos.y + p1->pos.h + deltaY1 >= floor)
		p1->pos.y = floor - p1->pos.h;
	else p1->pos.y += deltaY1;

	/* Walls */

	if (p1->pos.x + deltaX1 <= wall){
		p1->pos.x = wall;
		if(collision && p1->facing == 1) { p2->pos.x = p1->pos.x + p1->pos.w; lock2 = 1;}
	} else if (p1->pos.x + deltaX1 + p1->pos.w >= screenWidth - wall){
		p1->pos.x = screenWidth - wall - p1->pos.w;
		if(collision && p1->facing == -1) { p2->pos.x = p1->pos.x - p2->pos.w; lock2 = 1;}
	} else {
		if(collision) p1->pos.x += deltaX2;
		p1->pos.x += deltaX1;
	}

	if (p2->pos.x + deltaX2 <= wall){
		p2->pos.x = wall;
		if(collision && p2->facing == 1) p1->pos.x = p2->pos.x + p2->pos.w;
	} else if (p2->pos.x + deltaX2 + p2->pos.w >= screenWidth - wall){
		p2->pos.x = screenWidth - wall - p2->pos.w;
		if(collision && p2->facing == -1) p1->pos.x = p2->pos.x - p1->pos.w;
	} else { 
		if(!lock2){
			p2->pos.x += deltaX2;
			if(collision) p2->pos.x += deltaX1;
		}
	}

	if(hit(p1->pos, p2->pos)){
		if(p1->aerial && !(p2->aerial)){
			if(p2->facing == 1) p1->pos.x = p2->pos.x + p2->pos.w;
			else p1->pos.x = p2->pos.x - p1->pos.w;
		}
		if(p2->aerial && !(p1->aerial)){
			if(p1->facing == 1) p2->pos.x = p1->pos.x + p1->pos.w;
			else p2->pos.x = p1->pos.x - p2->pos.w;
		}
	}


	/*Enforcing gravity*/
	if(p1->pos.y + p1->pos.h >= floor && p1->aerial == 1){
		p1->aerial = 0;
	}
	if(!p1->aerial){
		if(p1->cMove == NULL){
			if(sAxis1[0]) deltaY1 = -35;
			else deltaY1 = 0;
			if(sAxis1[3]) deltaX1 = 5;
			if(sAxis1[2]) deltaX1 = -5;
			if((!sAxis1[2] && !sAxis1[3]) || sAxis1[1] == 1) deltaX1 = 0;
		}
		if (p1->pos.x < p2->pos.x && p1->facing == -1) { p1->facing = 1; p1->sFlag = 0;}
		else if (p1->pos.x > p2->pos.x && p1->facing == 1) { p1->facing = -1; p1->sFlag = 0;}
	}
	if(p1->aerial) deltaY1 += grav;

			/*Player 2*/


	if(p2->pos.y + p2->pos.h >= floor && p2->aerial == 1)
		p2->aerial = 0;
	if(!p2->aerial){
		if(p2->cMove == NULL){
			if(sAxis2[0]) deltaY2 = -35;
			else deltaY2 = 0;
			if(sAxis2[3]) deltaX2 = 5;
			if(sAxis2[2]) deltaX2 = -5;
			if((!sAxis2[2] && !sAxis2[3]) || sAxis2[1] == 1) deltaX2 = 0;
		}
		if (p2->pos.x < p1->pos.x && p2->facing == -1) { p2->facing = 1; p2->sFlag = 0;}
		else if (p2->pos.x > p1->pos.x && p2->facing == 1) { p2->facing = -1; p2->sFlag = 0; }
	}
	if(p2->aerial) deltaY2 += grav;

	/*Reinitialize inputs*/
	for(int i = 0; i < 5; i++){
		posEdge1[i] = 0;
		posEdge2[i] = 0;
		negEdge1[i] = 0;
		negEdge2[i] = 0;
	}
	if(!p1->aerial && !p1->cMove){
		if(p1->facing == -1 && p1->pos.x < p2->pos.x) p1->facing = 1;
		else if(p1->facing == 1 && p1->pos.x > p2->pos.x) p1->facing = -1;
	}
	if(!p2->aerial && !p2->cMove){
		if(p2->facing == -1 && p2->pos.x < p1->pos.x) p2->facing = 1;
		else if(p2->facing == 1 && p2->pos.x > p1->pos.x) p2->facing = -1;
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


bool interface::hit(SDL_Rect a, SDL_Rect b)
{
	if(a.y + a.h - b.y <= 0 || b.y + b.h - a.y <= 0) return 0;
	if(a.x + a.w - b.x < 0 || b.x + b.w - a.x < 0) return 0;
	return 1;

}
