#include "player.h"
player::player()
{

	/*Initialize input containers*/
	for(int i = 0; i < 30; i++)
		inputBuffer[i] = 5;

	/*Yeah yeah, I know, char* to literal conversion. I'm lazy right now. Will fix later. Maybe with cstring*/
	inputName[0] = "Up";
	inputName[1] = "Down";
	inputName[2] = "Left";
	inputName[3] = "Right";
	inputName[4] = "A";
	inputName[5] = "B";
	inputName[6] = "C";
	inputName[7] = "D";
	inputName[8] = "E";
	inputName[9] = "Start";

	/*Set up ALL the inputs*/
	for(int i = 0; i < 9; i++){
		printf("Please enter a command for %s\n", inputName[i]);
		keyConfig(i);
	}

	/*Set up the sprite*/

	rounds = 0;
	deltaX = 0;
	deltaY = 0;
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

void player::updateRects()
{
	if(pick->cMove != NULL) {
		pick->cMove->pollRects(delta, collision, hitreg, hitbox);
		if(pick->cMove->xLock) deltaX = delta.x*facing; else deltaX += delta.x*facing;
		if(!pick->aerial){
			if(pick->cMove->yLock) deltaY = delta.y; else deltaY += delta.y;
		}
		if(facing == -1) hitbox.x = pos.x + pos.w - hitbox.x - hitbox.w;
		else hitbox.x += pos.x;
		hitbox.y += pos.y;
		if(facing == -1) hitreg.x = pos.x + pos.w - hitreg.x - hitreg.w;
		else hitreg.x += pos.x;
		hitreg.y += pos.y;
		if(facing == -1) collision.x = pos.x + pos.w - collision.x - collision.w;
		else collision.x += pos.x;
		collision.y += pos.y;
	}
}

void player::enforceGravity(int grav, int floor)
{	
	if(pos.y + pos.h < floor && pick->aerial == 0) pick->aerial = 1; 
	
	else if(pick->aerial) deltaY += grav;
}

void player::checkBlocking()
{
	int st;
	if(pick->cMove == pick->airBlock || pick->cMove == pick->standBlock || pick->cMove == pick->crouchBlock)
		st = ((hitstun*)pick->cMove)->counter;
	else st = 0;

	switch(inputBuffer[0]){
	case 7:
	case 4:
		if(pick->aerial && (*pick->airBlock) > pick->cMove) {
			pick->airBlock->init(st);
			pick->cMove = pick->airBlock;
		}
		else if((*pick->standBlock) > pick->cMove) {
			pick->standBlock->init(st);
			pick->cMove = pick->standBlock;
		}
		updateRects();
		break;
	case 1:
		if(pick->aerial && (*pick->airBlock) > pick->cMove) {
			pick->airBlock->init(st);
			pick->cMove = pick->airBlock;
		}
		else if((*pick->crouchBlock) > pick->cMove) {
			pick->crouchBlock->init(st);
			pick->cMove = pick->crouchBlock;
		}
		updateRects();
		break;
	default:
		break;
	}
}

void player::checkCorners(int floor, int left, int right)
{
	/*Offset variables. I could do these calculations on the fly, but it's easier this way.
	Essentially, this represents the offset between the sprite and the collision box, since
	even though we're *checking* collision, we're still *moving* pos*/
	int lOffset = pos.x - collision.x;
	int rOffset = pos.x + pos.w - (collision.x + collision.w);
	int bOffset = pos.y + pos.h - (collision.y + collision.h);	

	/*Floor, or "Bottom corner"*/

	/*Currently this is done just with pos, but it needs to use collision, since
	pos is just the sprite, and our collision boxes won't always be a rectangle circumscribing
	the sprite. But something's broken about collision rects at the moment that makes that not work.
	Maybe check what's happening in updateRects() for that, or move::debugCollisionInit() is broken,
	which is also a possibility (and one that may be better remedied by having a real move constructor).*/

	if (pos.y + pos.h > floor){
		if(pick->aerial == 1){
			if(pick->cMove == pick->airBlock){
				pick->standBlock->init(pick->airBlock->counter);
				pick->cMove == pick->standBlock;
			} else { 
				if(pick->cMove) pick->cMove->init();
				pick->cMove = pick->neutral;
			}
			pick->aerial = 0;
			deltaY = 0;
		}
		pos.y = floor - pos.h;
	}

	/*Walls, or "Left and Right" corners

	This not only keeps the characters within the stage boundaries, but flags them as "in the corner"
	so we can specialcase collision checks for when one player is in the corner.*/

	if(collision.x <= left){
		if(facing == 1) lCorner = 1;
		if(collision.x < left) 
			pos.x = left + lOffset;
	} else lCorner = 0;
	if(collision.x + collision.w >= right){
		if(facing == -1) rCorner = 1;
		if(collision.x + collision.w > right)
			pos.x = right - collision.w - rOffset;
	} else rCorner = 0;
	updateRects(); //Update rectangles or the next collision check will be wrong.
}

void player::resolveCollision(player * other)
{
	/*Collision between players. Unfortunately a lot of specialcasing necessary here.*/
	bool displace = 0;
	bool oDisplace = 0;
	int lOffset = pos.x - collision.x;
	int rOffset = pos.x + pos.w - (collision.x + collision.w);
	int oLOffset = other->pos.x - other->collision.x;
	int oROffset = other->pos.x + other->pos.w - (other->collision.x + other->collision.w);


	if(other->lCorner) pos.x = other->collision.x + other->collision.w + lOffset;
	else if(other->rCorner) pos.x = other->collision.x - collision.w - rOffset;
	else if(lCorner) other->pos.x = collision.x + collision.w + oLOffset;
	else if(rCorner) other->pos.x = collision.x - other->collision.w - oROffset;
	else {
		if((!pick->aerial && !other->pick->aerial) || (pick->aerial && other->pick->aerial)){
			if((facing == 1 && deltaX > 0) || (facing == -1 && deltaX < 0)) oDisplace = 1;
			if((other->facing == 1 && other->deltaX > 0) || (other->facing == -1 && other->deltaX < 0)) displace = 1;
			if(displace && !oDisplace) {
				if(facing == 1) pos.x = other->collision.x - collision.w - rOffset;
				else pos.x = other->collision.x + other->collision.w + lOffset;
			} else if (oDisplace && !displace) {
				if(other->facing == 1) other->pos.x = collision.x - other->collision.w - oROffset;
				else other->pos.x = collision.x + collision.w + oLOffset;
			} else if (oDisplace && displace) { 
				if(abs(deltaX) > abs(other->deltaX)){
					pos.x += other->deltaX; 
					updateRects();
					if(other->facing == 1) other->pos.x = collision.x - other->collision.w - oROffset;
					else other->pos.x = collision.x + collision.w + oLOffset;
				} else {
					other->pos.x += deltaX; 
					other->updateRects();
					if(facing == 1) pos.x = other->collision.x - collision.w - rOffset;
					else pos.x = other->collision.x + other->collision.w + lOffset;
				}
			}
//*
		} else if (pick->aerial && !other->pick->aerial) {
			if(other->facing == 1) pos.x = other->collision.x + other->collision.w + lOffset;
			else pos.x = other->collision.x - collision.w - rOffset;
		} else {
			if(facing == 1) other->pos.x = collision.x + collision.w + oLOffset;
			else other->pos.x = collision.x - other->collision.w - oROffset;
//*/
		}	
	}
	updateRects();
	other->updateRects();
}

void player::checkFacing(int maypole){
	if(!pick->aerial){
		if (lCorner) facing = 1;
		else if (rCorner) facing = -1;
		else if (pos.x < maypole && facing == -1) facing = 1; 
		else if (pos.x > maypole && facing == 1) facing = -1; 
	}
}
