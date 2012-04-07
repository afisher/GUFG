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
	p[0] = new player;

	printf("Player 2:\n");
	p[1] = new player;

	/*Flag to kill the game*/
	gameover = 0;
	numRounds = 2;

	/*Select characters. Currently done automatically, to change later*/

	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);
	cSelectMenu();

	/*Build the character. Eventually this will probably be a function.*/
	matchInit();
}

void interface::matchInit()
{
	SDL_Surface * temp;
	p[0]->rounds = 0;
	p[1]->rounds = 0;
	temp = SDL_LoadBMP("Misc/BG1.bmp");
	background = SDL_DisplayFormat(temp);
	bg.w = 1600;
	bg.h = 900;
	roundInit();
}

void interface::roundInit()
{
	bg.x = 400;
	bg.y = 300;
	p[0]->pick->health = 300;
	p[1]->pick->health = 300;
	p[0]->pick->meter = 0;
	p[1]->pick->meter = 0;
	p[0]->deltaX = 0;
	p[1]->deltaX = 0;
	p[0]->deltaY = 0;
	p[1]->deltaY = 0;
	p[0]->pick->volitionX = 0;
	p[1]->pick->volitionX = 0;
	p[0]->pick->volitionY = 0;
	p[1]->pick->volitionY = 0;
	if(p[0]->pick->cMove != p[0]->pick->neutral && p[0]->pick->cMove){
		p[0]->pick->cMove->init();
		p[0]->pick->cMove = p[0]->pick->neutral;
	}
	if(p[1]->pick->cMove != p[1]->pick->neutral && p[1]->pick->cMove){
		p[1]->pick->cMove->init();
		p[1]->pick->cMove = p[1]->pick->neutral;
	}

	p[0]->sprite = NULL;
	p[1]->sprite = NULL;
	
	/*Background color, temporary until we have backgrounds*/
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));

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

	combo1 = 0;
	combo2 = 0;
	grav = 3;
	timer = 5824;
	p[0]->facing = 1;
	p[1]->facing = -1;
	p[0]->spriteInit();
	p[1]->spriteInit();
	p[0]->pos.x = wall*4;
	p[1]->pos.x = screenWidth - wall*4 - p[1]->sprite->w;
	p[0]->pos.y = floor - p[0]->sprite->h;
	p[1]->pos.y = floor - p[1]->sprite->h;
	draw();
}

void interface::runTimer()
{
	if(timer > 0) timer--;
}

void interface::resolve()
{

	/*Current plan for this function: Once I've got everything reasonably functionally abstracted into player members,
	the idea is to do the procedure as follows: 
		1. Update to current rectangles. Since the actual step is part of the draw routine, this should happen first.
		2. Figure out all deltas that should currently apply to sprite positions. Basically move the sprites to where 
		   they'd be if there were no collision rules.
		3. Check for things like hits and blocks. Enact all the effects of these, including stun, damage, etc.
		4. Check for collision against the opponent or against boundaries such as the floor or the corners. 
		   Haven't decided if there should be a cieling yet.
		5. Apply any changes to deltas that need to be made before the next frame
		6. Initialize sprites.
	*/

	p[0]->updateRects();
	p[1]->updateRects();

	p[0]->deltaX += p[0]->pick->volitionX*p[0]->facing;
	p[0]->deltaY += p[0]->pick->volitionY;
	p[1]->deltaX += p[1]->pick->volitionX*p[1]->facing;
	p[1]->deltaY += p[1]->pick->volitionY;
	
	if(p[0]->lCorner || p[0]->rCorner) p[1]->deltaX += p[0]->pick->volitionX*p[1]->facing;
	if(p[1]->lCorner || p[1]->rCorner) p[0]->deltaX += p[1]->pick->volitionX*p[0]->facing;
	
	p[1]->pick->volitionX = 0;
	p[1]->pick->volitionY = 0;
	p[0]->pick->volitionX = 0;
	p[0]->pick->volitionY = 0;
	
	if(p[0]->pick->freeze < 1){
		p[0]->pos.x += p[0]->deltaX;
		p[0]->pos.y += p[0]->deltaY;
	}
	if(p[1]->pick->freeze < 1){
		p[1]->pos.y += p[1]->deltaY;
		p[1]->pos.x += p[1]->deltaX;
	}

	p[0]->updateRects();
	p[1]->updateRects();
	
	p[0]->enforceGravity(grav, floor);
	p[1]->enforceGravity(grav, floor);	
	
	p[0]->checkFacing(p[1]->pos.x);
	p[1]->checkFacing(p[0]->pos.x);

	p[0]->checkCorners(floor, wall, screenWidth - wall);
	p[1]->checkCorners(floor, wall, screenWidth - wall);
	
	if (checkCollision(p[0]->collision, p[1]->collision)){
		p[0]->resolveCollision(p[1]);
	}
	if(p[0]->pick->cMove != p[0]->pick->reel && p[0]->pick->cMove != p[0]->pick->fall) combo2 = 0;
	if(p[1]->pick->cMove != p[1]->pick->reel && p[1]->pick->cMove != p[1]->pick->fall) combo1 = 0;

	if(p[1]->hitbox.w > 0) p[0]->checkBlocking();
	if(p[0]->hitbox.w > 0) p[1]->checkBlocking();

	move * temp1 = p[0]->pick->cMove;
	move * temp2 = p[1]->pick->cMove;
	bool hit1 = 0;
	bool hit2 = 0;

	if(p[0]->hitbox.w > 0 && p[1]->hitreg.w > 0){
		if(checkCollision(p[0]->hitbox, p[1]->hitreg)) {
			combo1 += p[1]->pick->takeHit(temp1);
			if(combo1 > 0) printf("p1: %i-hit combo\n", combo1+1);
			p[0]->pick->freeze = p[0]->pick->cMove->stun / 2;
			hit2 = 1;
		}
	}
	if(p[1]->hitbox.w > 0 && p[0]->hitreg.w > 0){
		if(checkCollision(p[1]->hitbox, p[0]->hitreg)) {
			combo2 += p[0]->pick->takeHit(temp2);
			if(combo2 > 0) printf("p2: %i-hit combo\n", combo2+1);
			p[1]->pick->freeze = p[1]->pick->cMove->stun / 2;
			hit1 = 1;
		}
	}
	if(hit2) temp2->init();
	if(hit1) temp1->init();

	if(!p[0]->pick->aerial){
		if(p[0]->pick->cMove == p[0]->pick->neutral)
			p[0]->deltaX = 0;
	}
	if(!p[1]->pick->aerial){
		if(p[1]->pick->cMove == p[1]->pick->neutral)
			p[1]->deltaX = 0;
	}
	/*Reinitialize inputs*/
	for(int i = 0; i < 5; i++){
		posEdge1[i] = 0;
		posEdge2[i] = 0;
		negEdge1[i] = 0;
		negEdge2[i] = 0;
	}

	/*Draw the sprites*/
	p[0]->spriteInit();
	p[1]->spriteInit();
	checkWin();
	runTimer();
}

void interface::checkWin()
{
	if(p[0]->pick->health == 0 || p[1]->pick->health == 0 || timer == 0){
		if(p[0]->pick->health > p[1]->pick->health) {
			printf("Player 1 wins!\n");
			p[0]->rounds++;
		}
		else if(p[1]->pick->health > p[0]->pick->health) {
			printf("Player 2 wins!\n");
			p[1]->rounds++;
		}
		else {
			printf("Draw!\n");
			if(p[0]->rounds < numRounds - 1) p[0]->rounds++;
			if(p[1]->rounds < numRounds - 1) p[1]->rounds++;
		}
		if(p[0]->rounds == numRounds || p[1]->rounds == numRounds){
			delete p[0]->pick;
			delete p[1]->pick;
			cSelectMenu();
			matchInit();
		}
		else roundInit();
	}
}

void interface::readInput()
{
	/*Make our dummy event for polls*/
	SDL_Event event;
	for(int i = 0; i < 20; i++){
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
					if(event.jaxis.which == p[0]->input[i].jaxis.which && event.jaxis.axis == p[0]->input[i].jaxis.axis && event.jaxis.value == p[0]->input[i].jaxis.value)
						sAxis1[i] = 1;
					if(event.jaxis.which == p[1]->input[i].jaxis.which && event.jaxis.axis == p[1]->input[i].jaxis.axis && event.jaxis.value == p[1]->input[i].jaxis.value)
						sAxis2[i] = 1;
					if(event.jaxis.which == p[0]->input[i].jaxis.which && event.jaxis.axis == p[0]->input[i].jaxis.axis && event.jaxis.value == 0)
						sAxis1[i] = 0;
					if(event.jaxis.which == p[1]->input[i].jaxis.which && event.jaxis.axis == p[1]->input[i].jaxis.axis && event.jaxis.value == 0)
						sAxis2[i] = 0;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				for(int i = 4; i < 9; i++){
					if(event.jbutton.which == p[0]->input[i].jbutton.which && event.jbutton.button == p[0]->input[i].jbutton.button)
						posEdge1[i-4] = 1;
					if(event.jbutton.which == p[1]->input[i].jbutton.which && event.jbutton.button == p[1]->input[i].jbutton.button)
						posEdge2[i-4] = 1;
				}
				break;
			case SDL_JOYBUTTONUP:
				for(int i = 4; i < 9; i++){
					if(event.jbutton.which == p[0]->input[i].jbutton.which && event.jbutton.button == p[0]->input[i].jbutton.button)
						negEdge1[i-4] = 1;
					if(event.jbutton.which == p[1]->input[i].jbutton.which && event.jbutton.button == p[1]->input[i].jbutton.button)
						negEdge2[i-4] = 1;
				}
				break;
			case SDL_KEYDOWN:
				for(int i = 0; i < 4; i++) {
					if(event.key.keysym.sym == p[0]->input[i].key.keysym.sym) 
						sAxis1[i] = 1;
					if(event.key.keysym.sym == p[1]->input[i].key.keysym.sym) 
						sAxis2[i] = 1;
				}
				for(int i = 4; i < 9; i++) {
					if(event.key.keysym.sym == p[0]->input[i].key.keysym.sym)
						posEdge1[i-4] = 1;
					if(event.key.keysym.sym == p[1]->input[i].key.keysym.sym)
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
					if(event.key.keysym.sym == p[0]->input[i].key.keysym.sym)
						sAxis1[i] = 0;
					if(event.key.keysym.sym == p[1]->input[i].key.keysym.sym)
						sAxis2[i] = 0;
				}
				for(int i = 4; i < 9; i++){
					if(event.key.keysym.sym == p[0]->input[i].key.keysym.sym)
						negEdge1[i-4] = 1;
					if(event.key.keysym.sym == p[1]->input[i].key.keysym.sym)
						negEdge2[i-4] = 1;
				}
				break;
			}
		}
	}
}


bool interface::checkCollision(SDL_Rect a, SDL_Rect b)
{
	if(a.y + a.h - b.y <= 0 || b.y + b.h - a.y <= 0) return 0;
	if(a.x + a.w - b.x < 0 || b.x + b.w - a.x < 0) return 0;
	return 1;
}

void interface::cSelectMenu()
{
	/*The plan is that this is eventually a menu, preferably pretty visual, in which players can select characters.*/
	printf("Please select a character:\n");
	int numChars = 2;
	SDL_Event event;
	SDL_Surface *temp = SDL_LoadBMP("Misc/Select.bmp");
	SDL_Surface *cursor1, *cursor2, *ct1, *ct2, *selectScreen;
	int select1, select2;
	select1 = 1;
	select2 = 2;
	SDL_Rect c1, c2, wheel;
	c1.x = 100; c1.y = 0;
	c2.x = 100; c2.y = 0;
	wheel.x = 100; wheel.y = 0;
	char base1[40];
	char base2[40];
	bool selectFlag1 = 0;
	bool selectFlag2 = 0;

	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 100, 100, 100));
	selectScreen = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	SDL_SetColorKey(selectScreen, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_BlitSurface(selectScreen, NULL, screen, &wheel);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	
	while(!selectFlag1 || !selectFlag2){
		if (SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == p[0]->input[2].key.keysym.sym && !selectFlag1) select1--;
				if(event.key.keysym.sym == p[0]->input[3].key.keysym.sym && !selectFlag1)	select1++;
				if(event.key.keysym.sym == p[1]->input[2].key.keysym.sym && !selectFlag2) select2--;
				if(event.key.keysym.sym == p[1]->input[3].key.keysym.sym && !selectFlag2)	select2++;
						
				for(int i = 4; i < 9; i++){
					if(event.key.keysym.sym == p[0]->input[i].key.keysym.sym) selectFlag1 = 1;
					if(event.key.keysym.sym == p[1]->input[i].key.keysym.sym) selectFlag2 = 1;
				}
				if(event.key.keysym.sym == SDLK_q){
					gameover = 1;
					selectFlag1 = 1;
					selectFlag2 = 1;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				for(int i = 4; i < 9; i++){
					if(event.jbutton.which == p[0]->input[i].jbutton.which && event.jbutton.button == p[0]->input[i].jbutton.button) selectFlag1 = 1;
					if(event.jbutton.which == p[1]->input[i].jbutton.which && event.jbutton.button == p[1]->input[i].jbutton.button) selectFlag2 = 1;
				}
				break;
			case SDL_JOYAXISMOTION:
				if(event.jaxis.which == p[0]->input[2].jaxis.which && event.jaxis.axis == p[0]->input[2].jaxis.axis && event.jaxis.value == p[0]->input[2].jaxis.value && !selectFlag1) select1--;
				if(event.jaxis.which == p[0]->input[3].jaxis.which && event.jaxis.axis == p[0]->input[3].jaxis.axis && event.jaxis.value == p[0]->input[3].jaxis.value && !selectFlag1) select1++;
				if(event.jaxis.which == p[1]->input[2].jaxis.which && event.jaxis.axis == p[1]->input[2].jaxis.axis && event.jaxis.value == p[1]->input[2].jaxis.value && !selectFlag2) select2--;
				if(event.jaxis.which == p[1]->input[3].jaxis.which && event.jaxis.axis == p[1]->input[3].jaxis.axis && event.jaxis.value == p[1]->input[3].jaxis.value && !selectFlag2) select2++;
				break;
			}
			if(select2 > numChars) select2 = 1;
			if(select1 > numChars) select1 = 1;
			if(select1 < 1) select1 = numChars;
			if(select2 < 1) select2 = numChars;
			sprintf(base1, "Misc/P1Select%i.bmp", select1);
			sprintf(base2, "Misc/P2Select%i.bmp", select2);
			ct1 = SDL_LoadBMP(base1);
			ct2 = SDL_LoadBMP(base2);
			cursor1 = SDL_DisplayFormat(ct1);
			cursor2 = SDL_DisplayFormat(ct2);
			SDL_FreeSurface(ct1);
			SDL_FreeSurface(ct2);
			SDL_SetColorKey(cursor1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
			SDL_SetColorKey(cursor2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
			SDL_BlitSurface(selectScreen, NULL, screen, &wheel);
			SDL_BlitSurface(cursor1, NULL, screen, &c1);
			SDL_BlitSurface(cursor2, NULL, screen, &c2);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
		}
	}
	SDL_FreeSurface(cursor1);
	SDL_FreeSurface(cursor2);
	SDL_FreeSurface(selectScreen);

	p[0]->characterSelect(select1);
	p[1]->characterSelect(select2);
}

interface::~interface()
{
	SDL_FreeSurface(screen);
	delete p[0];
	delete p[1];
}
