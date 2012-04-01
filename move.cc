#include "move.h"

move::move()
{
	name = NULL;
	state = 0;
}

move::move(char * n)
{
	ifstream read;
	SDL_Surface * temp;
	int startup, active, recovery;
	char fname[40];
	sprintf(fname, "%s.mv", n);
	name = n; 
	read.open(fname);
	while(read.get() != ':'); read.ignore();
	while(read.get() != ':'); read.ignore();
	read >> frames;
	while(read.get() != ':'); read.ignore();
	read >> startup;
	while(read.get() != ':'); read.ignore();
	read >> active;
	while(read.get() != ':'); read.ignore();
	read >> recovery;
	while(read.get() != ':'); read.ignore();
	read >> allowed;
	while(read.get() != ':'); read.ignore();
	read >> state;
	while(read.get() != ':'); read.ignore();
	read >> cState;
	while(read.get() != ':'); read.ignore();
	read >> damage;
	while(read.get() != ':'); read.ignore();
	read >> stun;
	while(read.get() != ':'); read.ignore();
	read >> push;
	while(read.get() != ':'); read.ignore();
	read >> lift;
	while(read.get() != ':'); read.ignore();
	read >> blockMask;
	while(read.get() != ':'); read.ignore(); 
	read >> blockState;

	//Properties will be a bit more complicated, I'll add this later.
	collision = new SDL_Rect[frames];
	hitbox = new SDL_Rect[frames];
	hitreg = new SDL_Rect[frames];
	delta = new SDL_Rect[frames];

	for(int i = 0; i < frames; i++)
	{
		while(read.get() != '$'); read.ignore(2);
		read >> collision[i].x >> collision[i].y >> collision[i].w >> collision[i].h;
		while(read.get() != '$'); read.ignore(2);
		read >> hitreg[i].x >> hitreg[i].y >> hitreg[i].w >> hitreg[i].h;
		while(read.get() != '$'); read.ignore(2);
		read >> delta[i].x >> delta[i].y >> delta[i].w >> delta[i].h;
		if(i >= startup && i < startup+active){
			while(read.get() != '$'); read.ignore(2);
			read >> hitbox[i].x >> hitbox[i].y >> hitbox[i].w >> hitbox[i].h;
		} else {
			hitbox[i].x = 0; hitbox[i].y = 0; hitbox[i].w = 0; hitbox[i].h = 0;
		}	
	}

	launch = 0;
	for(int i = 0; i < 5; i++)
		button[i] = 0;
	int r = strlen(name);
	for(int i = 0; i < r; i++){
		switch(name[i]){
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
		default:
			break;
		}

	}
	read.close();
	sprite = new SDL_Surface*[frames];
	fSprite = new SDL_Surface*[frames];
	for(int i = 0; i < frames; i++){
		sprintf(fname, "%s#%i", n, i);
		temp = SDL_LoadBMP(fname);
		sprite[i] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		sprintf(fname, "%s#%iF", n, i);
		temp = SDL_LoadBMP(fname);
		fSprite[i] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
	}
	tolerance = 30;
	activation = 30;
	init();
}

void move::setTolerance(int t)
{
	tolerance = t;
}


move::~move()
{
	if(!this) return;
	for(int i = 0; i < frames; i++){
		if(sprite[i]) SDL_FreeSurface(sprite[i]);
		if(fSprite[i]) SDL_FreeSurface(fSprite[i]);
	}
	if(sprite) delete [] sprite;
	if(fSprite) delete [] fSprite;
	if(collision) delete [] collision;
	if(hitbox) delete [] hitbox;
	if(hitreg) delete [] hitreg;
	if(delta) delete [] delta;
}

bool move::check(bool pos[5], bool neg[5], int t, int f)
{
	//if(meter < cost) return 0;
//	if(state != allowedState) return 0;
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(!pos[i]) return 0;
		}
				
	}
	if(t > tolerance) return 0;
	if(f > activation) return 0;
	return 1;
}

void move::pollRects(SDL_Rect &d, SDL_Rect &c, SDL_Rect &r, SDL_Rect &b)
{

	d.x = delta[currentFrame].x; d.y = delta[currentFrame].y;
	c.x = collision[currentFrame].x; c.w = collision[currentFrame].w;
	c.y = collision[currentFrame].y; c.h = collision[currentFrame].h;
	r.x = hitreg[currentFrame].x; r.w = hitreg[currentFrame].w;
	r.y = hitreg[currentFrame].y; r.h = hitreg[currentFrame].h;
	if(cFlag) {
		b.x = 0; b.w = 0;
		b.y = 0; b.h = 0;
	} else {
		b.x = hitbox[currentFrame].x; b.w = hitbox[currentFrame].w; 
		b.y = hitbox[currentFrame].y; b.h = hitbox[currentFrame].h;
	}
}

SDL_Surface * move::draw(int facing)
{
	SDL_Surface * temp;
	if(facing == -1) temp = fSprite[currentFrame];
	else temp = sprite[currentFrame];
	step();
	return temp;
}

bool move::operator>(move * x)
{
	if(x == NULL) return 1;
	if(frames == 0 || x->frames == 0) return 0;
	else{
		if(x->cFlag == 1){
			if(allowed & cState) return 1;
		} else if(this != x) { /*Moves can't cancel into themselves without connecting. 
					I put this in place mainly to allow neutral states to
					loop without re-triggering. Perhaps there's a better way.
					If this causes problems, we'll revisit it*/
			if(allowed & x->state) return 1;
		}
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

void move::blockSuccess(int st)
{
	return;
}

void execute(move * last)
{
	last->init();
}



/*Testing stuff for now, thus it becomes necessary to set all states to stuff*/
void move::debugStateInit(int q, int r, int s)
{
	allowed = q;
	state = r;
	cState = s;
}

void move::debugRectsInit()
{
	for(int i = 0; i < frames; i++){
		hitbox[i].y = 0;
		hitbox[i].x = 0;
		hitbox[i].w = 0;
		hitbox[i].h = 0;
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
		hitreg[i].w = 0;
		hitreg[i].h = 0;
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
	if(fish != NULL) delete [] fish;
	fish = NULL;
}

