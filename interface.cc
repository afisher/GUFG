/*Interface class for GUFG
 *This will run all the main game functions within GUFG
 *
 *Written by Alex Kelly in 2012
 *License to come
 */

#include "interface.h"
#include <cstring>
#include <math.h>
interface::interface()
{
	/*Initialize some pseudo-constants*/
	screenWidth = 800; //By screen, I mean the window the game occurs in.
	screenHeight = 600;
	bg.w = 1600;	//By background, I mean the thing the characters actually move on. Bigger than the screen.
	bg.h = 900;
	floor = bg.h - 25; //Value of the floor. This is the maximum distance downward that characters can travel.
	wall = 25;	//The size of the offset at which characters start to scroll the background, and get stuck.
	/*Initialize SDL*/
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_JOYSTICK);
	/*WM stuff*/
	SDL_WM_SetCaption("GUFG", "GUFG");
	screen = SDL_SetVideoMode(screenWidth, screenHeight, 0, 0);
	SDL_ShowCursor(SDL_DISABLE);

	/*Set up input buffers and joysticks*/
	for(int i = 0; i < SDL_NumJoysticks(); i++)
		SDL_JoystickOpen(i);
	/*Initialize players.*/
	printf("Player 1:\n"); //Print statement, a temporary measure.
	p[0] = new player;

	printf("Player 2:\n");
	p[1] = new player;

	/*Game and round end conditions*/
	gameover = 0;
	numRounds = 2;

	/*Select characters.*/

	colorKey = SDL_MapRGB(screen->format, 0, 255, 0); //Soon to be deprecated.
	cSelectMenu();

	/*Start a match*/
	matchInit();
}

/*This functions sets things up for a new match. Initializes some things and draws the background*/
void interface::matchInit() 
{
	SDL_Surface * temp;
	p[0]->rounds = 0;
	p[1]->rounds = 0;
	background = IMG_Load("Misc/BG1.png");
	q = 0;
	roundInit();
}

/*Sets stuff up for a new round. This initializes the characters, the timer, and the background.*/
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
	p[0]->pos.x = 700;
	p[1]->pos.x = 900 - p[1]->sprite->w;
	p[0]->pos.y = floor - p[0]->sprite->h;
	p[1]->pos.y = floor - p[1]->sprite->h;
	draw();
}

/*Pretty simple timer modifier*/
void interface::runTimer()
{
	if(timer > 0) timer--;
}

/*Main function for a frame. This resolves character positions, background scrolling, and hitboxes*/
void interface::resolve()
{
	p[0]->pushInput(sAxis1, posEdge1, negEdge1);
	p[1]->pushInput(sAxis2, posEdge2, negEdge2);

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

//*	
	dragBG(	p[1]->dragBG(bg.x + wall, bg.x + screenWidth - wall) + 
	p[0]->dragBG(bg.x + wall, bg.x + screenWidth - wall) );
//*/
	p[0]->checkCorners(floor, bg.x + wall, bg.x + screenWidth - wall);
	p[1]->checkCorners(floor, bg.x + wall, bg.x + screenWidth - wall);
	
	if (aux::checkCollision(p[0]->collision, p[1]->collision)){
		p[0]->resolveCollision(p[1]);
	}
	if(p[0]->pick->cMove != p[0]->pick->reel && p[0]->pick->cMove != p[0]->pick->fall) combo2 = 0;
	if(p[1]->pick->cMove != p[1]->pick->reel && p[1]->pick->cMove != p[1]->pick->fall) combo1 = 0;

	if(p[1]->hitbox[0].w > 0) p[0]->checkBlocking();
	if(p[0]->hitbox[0].w > 0) p[1]->checkBlocking();

	//Check if moves hit. This will probably be a function at some point

	move * temp1 = p[0]->pick->cMove;
	move * temp2 = p[1]->pick->cMove;
	bool hit1 = 0;
	bool hit2 = 0;

	/*This loop checks for hits. Eventually this might be a function*/
	for(int i = 0; i < p[1]->regComplexity; i++){
		for(int j = 0; j < p[0]->hitComplexity; j++){
			if(p[0]->hitbox[j].w > 0 && p[1]->hitreg[i].w > 0){
				if(aux::checkCollision(p[0]->hitbox[j], p[1]->hitreg[i])) {
					combo1 += p[1]->pick->takeHit(temp1);
					if(combo1 > 0) printf("p1: %i-hit combo\n", combo1+1);
					p[0]->pick->freeze = temp1->stun / 2;
					hit2 = 1;
					i = p[1]->regComplexity;
					j = p[0]->hitComplexity;
				}
			}

		}
	}
	for(int i = 0; i < p[0]->regComplexity; i++){
		for(int j = 0; j < p[1]->hitComplexity; j++){
			if(p[1]->hitbox[j].w > 0 && p[0]->hitreg[i].w > 0){
				if(aux::checkCollision(p[1]->hitbox[j], p[0]->hitreg[i])) {
					combo2 += p[0]->pick->takeHit(temp2);
					if(combo2 > 0) printf("p2: %i-hit combo\n", combo2+1);
					p[1]->pick->freeze = temp2->stun / 2;
					hit1 = 1;
					i = p[0]->regComplexity;
					j = p[1]->hitComplexity;
				}
			}
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
	/*if(q) { bg.y += 3; bg.x += 8; }
	else { bg.y -= 3; bg.x -= 8; }
	if(bg.y >= 300) q = 0;
	if(bg.y <= 0) q = 1;*/
}

/*Check if someone won*/
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

/*Read the input that's happened this frame*/
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



void interface::cSelectMenu()
{
	/*The plan is that this is eventually a menu, preferably pretty visual, in which players can select characters.*/
	printf("Please select a character:\n");
	int numChars = 2;
	SDL_Event event;
	SDL_Surface *selectScreen = aux::load_image("Misc/Select.png");
	SDL_Surface *cursor1, *cursor2;
	int select1, select2;
	select1 = 1;
	select2 = 2;
	SDL_Rect wheel;
	wheel.x = 100; wheel.y = 0;
	char base1[40];
	char base2[40];
	bool selectFlag1 = 0;
	bool selectFlag2 = 0;

	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 100, 100, 100));
	SDL_BlitSurface(selectScreen, NULL, screen, &wheel);
	SDL_UpdateRect(screen, 0, 0, 0, 0);


	while(!selectFlag1 || !selectFlag2){
		if (SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_KEYDOWN:
				if(event.key.keysym.sym == p[0]->input[2].key.keysym.sym && !selectFlag1) select1--;
				if(event.key.keysym.sym == p[0]->input[3].key.keysym.sym && !selectFlag1) select1++;
				if(event.key.keysym.sym == p[1]->input[2].key.keysym.sym && !selectFlag2) select2--;
				if(event.key.keysym.sym == p[1]->input[3].key.keysym.sym && !selectFlag2) select2++;
						
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
			sprintf(base1, "Misc/P1Select%i.png", select1);
			sprintf(base2, "Misc/P2Select%i.png", select2);
			cursor1 = aux::load_image(base1);
			cursor2 = aux::load_image(base2);
			SDL_BlitSurface(selectScreen, NULL, screen, &wheel);
			SDL_BlitSurface(cursor1, NULL, screen, &wheel);
			SDL_BlitSurface(cursor2, NULL, screen, &wheel);
			SDL_UpdateRect(screen, 0, 0, 0, 0);
			SDL_FreeSurface(cursor1);
			SDL_FreeSurface(cursor2);
		}
	}
	SDL_FreeSurface(selectScreen);

	p[0]->characterSelect(select1);
	p[1]->characterSelect(select2);
}

int interface::dragBG(int deltaX)
{
	bg.x += deltaX;
	if(bg.x < 0) bg.x = 0;
	else if(bg.x > 800) bg.x = 800;
}

interface::~interface()
{
	SDL_FreeSurface(screen);
	SDL_FreeSurface(background);
	delete p[0];
	delete p[1];
	SDL_Quit();
}

