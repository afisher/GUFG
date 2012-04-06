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
	xLock = 0;
	yLock = 0;
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

void move::execute(move * last)
{
	last->init();
}

