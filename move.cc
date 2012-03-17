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
	init();
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
	state = new unsigned int[l];
	collision = new SDL_Rect[l];
	hitbox = new SDL_Rect[l];
	hitreg = new SDL_Rect[l];
	delta = new SDL_Rect[l];
	xLock = 0;
	stun = 15;
	yLock = 0;
	damage = 0;
	push = 1;
	lift = 1;
	launch = 0;
	init();
	blockMask = 7;
	block = 0;
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
	damage = 0;
	init();
	xLock = 0;
	yLock = 0;
	stun = 15;
	state = new unsigned int[l];
	collision = new SDL_Rect[l];
	hitbox = new SDL_Rect[l];
	hitreg = new SDL_Rect[l];
	delta = new SDL_Rect[l];
	push = 1;
	lift = 1;
	launch = 0;
	blockMask = 7;
	block = 0;
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
	init();
}

move::~move()
{
	if(name) delete [] name;
	if(start) delete start;
	if(state) delete [] state;
	if(collision) delete [] collision;
	if(hitbox) delete [] hitbox;
	if(hitreg) delete [] hitreg;
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

void move::pollRects(SDL_Rect &d, SDL_Rect &c, SDL_Rect &r, SDL_Rect &b)
{

	if(start){
		d.x = delta[currentFrame].x; d.y = delta[currentFrame].y;
		c.x = collision[currentFrame].x; c.w = collision[currentFrame].w;
		c.y = collision[currentFrame].y; c.h = collision[currentFrame].h;
		r.x = hitreg[currentFrame].x; r.w = hitreg[currentFrame].w;
		r.y = hitreg[currentFrame].y; r.h = hitreg[currentFrame].h;
		if(!cFlag) { 
			b.x = hitbox[currentFrame].x; b.w = hitbox[currentFrame].w; 
			b.y = hitbox[currentFrame].y; b.h = hitbox[currentFrame].h;
		}
		else {
			b.x = 0; b.y = 0; b.w = 0; b.h = 0;
		}
	}
}

SDL_Surface * move::draw(int facing)
{
	SDL_Surface * temp;
	if(facing == -1) temp = (*start)[currentFrame]->fSprite;
	else temp = (*start)[currentFrame]->sprite;
	step();
	return temp;
}

bool move::operator==(move * x)
{
	if(frames == 0 || x->frames == 0) return 0;
	else{
		if(x->cFlag == 1){
			if(allowed & cState) return 1;
		}
		else if(allowed & x->state[currentFrame]) return 1;
	}
	return 0;
}

void move::step()
{
	currentFrame++;
}

void move::init()
{
	cFlag = 0;
	currentFrame = 0;
}

void move::connect()
{
	cFlag = 1;
}



/*Testing stuff for now, thus it becomes necessary to set all states to stuff*/
void move::debugStateInit(int q, int r, int s)
{
	allowed = q;
	for(int i = 0; i < frames; i++)
		state[i] = r;
	cState = s;
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
		hitreg[i].y = 0;
		hitreg[i].x = 0;
		hitreg[i].w = 1;
		hitreg[i].h = 1;
	}
}

void move::debugHitboxInit(int x, int y, int w, int h)
{
	for(int i = 0; i < frames; i++){
		hitbox[i].x = x;
		hitbox[i].y = y;
		hitbox[i].w = w;
		hitbox[i].h = h;
	}
}

void move::debugHittableInit(int x, int y, int w, int h)
{
	for(int i = 0; i < frames; i++){
		hitreg[i].y = y;
		hitreg[i].x = x;
		hitreg[i].w = w;
		hitreg[i].h = h;
	}
}
void move::debugCollisionInit(int x, int y, int w, int h)
{	
	for(int i = 0; i < frames; i++){
		collision[i].y = y;
		collision[i].x = x;
		collision[i].w = w;
		collision[i].h = h;
	}
}
void move::debugDeltaInit(int x, int y, int w, int h) 
{
	for(int i = 0; i < frames; i++){
		delta[i].y = y;
		delta[i].x = x;
		delta[i].w = w;
		delta[i].h = h;
	}
}
void move::debugDamageInit(int d) { damage = d; }









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
	tolerance = new int;
	tolerance[0] = 30;
}

void moveTrie::insert(move * b)
{
	int i;
	int * t;
	move * temp;
	occupants++;
	temp = new move[occupants];
	t = new int[occupants];
	for(i = 0; i < occupants-1; i++){
		temp[i] = fish[i];
		t[i] = tolerance[i];
	}
	t[i] = 30;
	temp[i] = *b;
	fish = temp;
	tolerance = t;

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
	delete tolerance;
	delete fish;
	fish = NULL;
}

