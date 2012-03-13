#include "move.h"
#include <cstring>
move::move()
{
	name = NULL;
	state = NULL;
}

move::move(char * n)
{
	name = n;
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	special = 0;
	start = NULL;
	tolerance = 30;
	state = NULL;
}

move::move(char * n, int l)
{
	name = n;
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	special = 0;
	start = new frame(n, l);
	tolerance = 30;
	frames = l;
	state = new int[l];
	collision = new SDL_Rect[l];
	hitbox = new SDL_Rect[l];
	hittable = new SDL_Rect[l];
	delta = new SDL_Rect[l];
	init();
}

move::move(char* n, char *b, bool s, int l)
{
	int r = strlen(b);
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	for(int i = 0; i < r; i++){
		switch(b[i]){
		case 'A':
			button[0] = 1;
			break;
		case 'B':
			button[1] = 1;
			break;
		case 'C':
			button[2] = 1;
			break;
		case 'D':
			button[3] = 1;
			break;
		case 'E':
			button[4] = 1;
			break;
		}

	}
	special = s;
	start = new frame(n, l);
	tolerance = 30;
	frames = l;
	init();
	state = new int[l];
	collision = new SDL_Rect[l];
	hitbox = new SDL_Rect[l];
	hittable = new SDL_Rect[l];
	delta = new SDL_Rect[l];
}

void move::setTolerance(int t)
{
	tolerance = t;
}

move::move(char* n, char * b, bool s)
{
	int r = strlen(b);
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	name = n;
	for(int i = 0; i < r; i++){
		switch(b[i]){
		case 'A':
			button[0] = 1;
			break;
		case 'B':
			button[1] = 1;
			break;
		case 'C':
			button[2] = 1;
			break;
		case 'D':
			button[3] = 1;
			break;
		case 'E':
			button[4] = 1;
			break;
		}

	}
	special = s;
	start = NULL;
	tolerance = 30;
	frames = 0;

}

move::~move()
{
	if(name) delete [] name;
	if(start) delete start;
	if(state) delete [] state;
	if(collision) delete [] collision;
	if(hitbox) delete [] hitbox;
	if(hittable) delete [] hittable;
	if(delta) delete [] delta;
}

bool move::check(bool pos[5], bool neg[5], int t)
{
	//if(meter < cost) return 0;
//	if(state != allowedState) return 0;
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(special){
				if(pos[i] == 0 && neg[i] == 0) return 0;
			}
			else if(pos[i] == 0) return 0;
		}
				
	}
	if(t > tolerance) return 0;
	return 1;
}

void move::execute()
{
	printf("Hook for %s detected\n", name);
}

void move::execute(frame *& curr)
{
	if(start != NULL) curr = start;
	else printf("Hook for %s detected\n", name);
}

bool move::step(SDL_Rect &d, SDL_Rect &coll, SDL_Rect &hit, SDL_Rect &hbox, frame * &anim)
{

	if(frames > 0){
		d = delta[currentFrame];
		coll = collision[currentFrame];
		hit = hittable[currentFrame];
		hbox = hitbox[currentFrame];
		anim = start;
		if(currentFrame == frames-1) {
			init();
			return 0;
		}
		else {
			currentFrame++;
			return 1;
		}
	}
	else return 0;

}

bool move::operator==(move * x)
{
	if(frames < 1 || x->frames < 1) return 0;
	else if(state[currentFrame] == x->allowed) return 1;
	else return 0;
}

void move::init()
{
	currentFrame = 0;
}

/*Testing stuff for now, thus it becomes necessary to set all states to stuff*/
void move::debugStateInit(int q, int r)
{
	for(int i = 0; i < frames; i++)
		state[i] = q;
	allowed = r;
}

void move::debugRectsInit()
{
	for(int i = 0; i < frames; i++){
		hitbox[i].y = 0;
		hitbox[i].x = 0;
		hitbox[i].w = 1;
		hitbox[i].h = 1;
		delta[i].y = 0;
		delta[i].x = 0;
		delta[i].w = 1;
		delta[i].h = 1;
		collision[i].y = 0;
		collision[i].x = 0;
		collision[i].w = 1;
		collision[i].h = 1;
		hittable[i].y = 0;
		hittable[i].x = 0;
		hittable[i].w = 1;
		hittable[i].h = 1;
	}
}













moveTrie::moveTrie()
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = NULL;
	occupants = 0;
}


moveTrie::moveTrie(move * a)
{
	for(int i = 0; i < 10; i++)
		child[i] = NULL;
	fish = a;
	occupants = 1;
}

void moveTrie::insert(move * b)
{
	int i;
	move * temp;
	occupants++;
	temp = new move[occupants];
	for(i = 0; i < occupants-1; i++)
		temp[i] = fish[i];
	temp[i] = *b;
	fish = temp;

}

moveTrie * moveTrie::insert(int a, move * b)
{
	move * temp;
	if(a < 10 && a > 0){
		if(!child[a]) child[a] = new moveTrie(b);
		else child[a]->insert(b);
		return child[a];
	}
	else return NULL;
}

moveTrie * moveTrie::insert(int a)
{
	if(a < 10 && a > 0) {
		if(!child[a]) child[a] = new moveTrie();
		return child[a];
	}
	else return NULL;
}

moveTrie::~moveTrie()
{
	for(int i = 0; i < 9; i++){
		if(child[i] != NULL){
			delete child[i];
			child[i] = NULL;
		}
	}
	delete fish;
	fish = NULL;
}

