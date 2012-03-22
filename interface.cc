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

	/*Flag to kill the game*/
	gameover = 0;
	numRounds = 2;

	/*Temporarily, this is where we do character select. Obviously this will become a menu later*/
	p1->characterSelect(1);
	p2->characterSelect(0);

	/*Build the character. Eventually this will probably be a function.*/
	colorKey = SDL_MapRGB(screen->format, 0, 255, 0);
	roundInit();
}

void interface::roundInit()
{
	p1->pick->health = 300;
	p2->pick->health = 300;
	p1->sprite = NULL;
	p2->sprite = NULL;
	
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
	p1->facing = 1;
	p2->facing = -1;
	p1->spriteInit();
	p2->spriteInit();
	p1->pos.x = wall*4;
	p2->pos.x = screenWidth - wall*4 - p2->sprite->w;
	p1->pos.y = floor - p1->sprite->h;
	p2->pos.y = floor - p2->sprite->h;
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

	p1->updateRects();
	p2->updateRects();
	
	bool collision = 0;
	
	p1->deltaX += p1->pick->volitionX*p1->facing;
	p1->deltaY += p1->pick->volitionY;
	if(p1->lCorner || p1->rCorner) p2->deltaX += p1->pick->volitionX*p2->facing;
	p2->deltaX += p2->pick->volitionX*p2->facing;
	p2->deltaY += p2->pick->volitionY;
	if(p2->lCorner || p2->rCorner) p1->deltaX += p2->pick->volitionX*p1->facing;

	p1->pos.y += p1->deltaY;
	p2->pos.y += p2->deltaY;

	p1->pick->volitionX = 0;
	p2->pick->volitionX = 0;
	p1->pick->volitionY = 0;
	p2->pick->volitionY = 0;

	p1->pos.x += p1->deltaX;
	p2->pos.x += p2->deltaX;
	
	p1->enforceGravity(grav, floor);
	p2->enforceGravity(grav, floor);	
	
	p1->checkFacing(p2->pos.x);
	p2->checkFacing(p1->pos.x);

	p1->checkCorners(floor, wall, screenWidth - wall);
	p2->checkCorners(floor, wall, screenWidth - wall);
	
	SDL_Rect a = p1->collision, b = p2->collision;
	collision = checkCollision(a, b);
	
	if(p1->pick->cMove != p1->pick->reel && p1->pick->cMove != p1->pick->fall) combo2 = 0;
	if(p2->pick->cMove != p2->pick->reel && p2->pick->cMove != p2->pick->fall) combo1 = 0;

	if(p2->hitbox.w > 0) p1->checkBlocking();
	if(p1->hitbox.w > 0) p2->checkBlocking();

	if(p1->hitbox.w > 0 && p2->hitreg.w > 0){
		if(checkCollision(p1->hitbox, p2->hitreg)) {
			combo1 += p2->pick->takeHit(p1->pick->cMove);
			if(combo1 > 0) printf("p1: %i-hit combo\n", combo1+1);
		}
	}
	if(p2->hitbox.w > 0 && p1->hitreg.w > 0){
		if(checkCollision(p2->hitbox, p1->hitreg)) {
			combo2 += p1->pick->takeHit(p2->pick->cMove);
			if(combo2 > 0) printf("p2: %i-hit combo\n", combo2+1);
		}
	}

	/*One more collision case: Resolving jumping on people*/

	if(checkCollision(p1->pos, p2->pos)){
		if(p1->pick->aerial && !(p2->pick->aerial)){
			if(p2->facing == 1) p1->pos.x = p2->pos.x + p2->pos.w;
			else p1->pos.x = p2->pos.x - p1->pos.w;
		}
		if(p2->pick->aerial && !(p1->pick->aerial)){
			if(p1->facing == 1) p2->pos.x = p1->pos.x + p1->pos.w;
			else p2->pos.x = p1->pos.x - p2->pos.w;
		}
	}


	if(!p1->pick->aerial)
		if(p1->pick->cMove == p1->pick->neutral)
			p1->deltaX = 0;

	if(!p2->pick->aerial)
		if(p2->pick->cMove == p2->pick->neutral)
			p2->deltaX = 0;

	/*Reinitialize inputs*/
	for(int i = 0; i < 5; i++){
		posEdge1[i] = 0;
		posEdge2[i] = 0;
		negEdge1[i] = 0;
		negEdge2[i] = 0;
	}

	/*Draw the sprites*/
	p1->spriteInit();
	p2->spriteInit();
	checkWin();
	runTimer();
}

void interface::checkWin()
{
	if(p1->pick->health == 0 || p2->pick->health == 0 || timer == 0){
		if(p1->pick->health > p2->pick->health) {
			printf("Player 1 wins!\n");
			p1->rounds++;
		}
		else if(p2->pick->health > p1->pick->health) {
			printf("Player 2 wins!\n");
			p2->rounds++;
		}
		else {
			printf("Draw!\n");
			if(p1->rounds < numRounds - 1) p1->rounds++;
			if(p2->rounds < numRounds - 1) p2->rounds++;
		}
		if(p1->rounds == numRounds || p2->rounds == numRounds) gameover = 1;
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


bool interface::checkCollision(SDL_Rect a, SDL_Rect b)
{
	if(a.y + a.h - b.y <= 0 || b.y + b.h - a.y <= 0) return 0;
	if(a.x + a.w - b.x < 0 || b.x + b.w - a.x < 0) return 0;
	return 1;
}
