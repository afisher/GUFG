#include "hitstun.h"

void hitstun::init(int n)
{
	counter = n;
}

void hitstun::step()
{
	if(counter <= 0) currentFrame++;
	else counter--;
}
	
void hitstun::blockSuccess(int st){
	init(st);
}

hitstun::hitstun(char * n)
{
	ifstream read;
	SDL_Surface * temp;
	int startup, active, recovery;
	char fname[40];
	char buffer[100];
	streampos backup;
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

	while(read.get() != ':'); read.ignore();
	//Properties will be a bit more complicated, I'll add this later.
	collision = new SDL_Rect[frames];
	hitbox = new SDL_Rect[frames];
	hitreg = new SDL_Rect*[frames];
	delta = new SDL_Rect[frames];

	for(int i = 0; i < frames; i++)
	{
		while(read.get() != '$'); read.ignore(2);
		read >> collision[i].x >> collision[i].y >> collision[i].w >> collision[i].h;
		while(read.get() != '$'); read.ignore(2);
		read.tellg();
		read.getline(buffer, 100);
		regComplexity[i] = 1;
		for(int j = 0; j < strlen(buffer); j++)
			if(buffer[i] == ',') regComplexity[i]++;
		read.seekg(backup);
		hitreg[i] = new SDL_Rect[regComplexity[i]];
		for(int j = 0; j < regComplexity[i]; j++){
			read >> hitreg[i][j].x >> hitreg[i][j].y >> hitreg[i][j].w >> hitreg[i][j].h;
			read.ignore();
		}
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
	tolerance = 0;
	activation = 0;
	currentFrame = 0;
}
