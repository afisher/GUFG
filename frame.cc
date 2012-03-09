#include "frame.h"
#include <cstring>
#include <stdio.h>
frame::frame()
{
        sprite = NULL;
        next = NULL;
}

frame::frame(char * name)
{
	int l = strlen(name);
	char * fName = new char[l+5];
	strcpy(fName, name);
	fName[l] = '.';
	fName[l+1] = 'b';
	fName[l+2] = 'm';
	fName[l+3] = 'p';
	fName[l+4] = '\0';
        SDL_Surface * temp = SDL_LoadBMP(fName);
        sprite = SDL_DisplayFormat(temp);
        SDL_FreeSurface(temp);
        next = NULL;
	delete [] fName;
}

frame::frame(char * name, int n)
{
	int l = strlen(name);
	char * fName = new char[l+5];
	strcpy(fName, name);
	fName[l] = '.';
	fName[l+1] = 'b';
	fName[l+2] = 'm';
	fName[l+3] = 'p';
	fName[l+4] = '\0';
//	printf("%s: %i\n", fName, l);
	frame * x = this;
        SDL_Surface * temp = SDL_LoadBMP(fName);
        sprite = SDL_DisplayFormat(temp);
        SDL_FreeSurface(temp);
	for(int i = 0; i < n; i++){
		x->next = new frame(name);
		x = x->next;
	}
	delete [] fName;
}

frame::~frame()
{
        if(next) {
                delete next;
                next = NULL;
	}
        if(sprite)
		SDL_FreeSurface(sprite);
}

frame * frame::play(SDL_Surface *& gameSprite)
{
	gameSprite = sprite;
	return next;
}
