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
	if(pos.y + pos.h >= floor && pick->aerial == 1){
		if(pick->cMove == pick->airBlock){
			pick->standBlock->init(pick->airBlock->counter);
			pick->cMove == pick->standBlock;
		} else { 
			pick->cMove->init();
			pick->cMove = pick->neutral;
		}
		pick->aerial = 0;
	}

	if(pick->aerial) deltaY += grav;

	if (pos.y + deltaY + pos.h >= floor){
		pos.y = floor - pos.h;
		pick->aerial = 0; deltaY = 0;
	}
	else pos.y += deltaY;
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
