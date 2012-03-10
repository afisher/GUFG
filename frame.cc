#include "frame.h"
#include <cstring>
#include <stdio.h>
#include <iostream>
using namespace std;
#include <fstream>
frame::frame()
{
        sprite = NULL;
        next = NULL;
}

void frame::init(char * name)
{
        SDL_Surface * temp = SDL_LoadBMP(name);
        sprite = SDL_DisplayFormat(temp);
        SDL_FreeSurface(temp);
        next = NULL;
}

frame::frame(char * name, int n)
{
	int l = strlen(name);
	char num[n/10+1];	
	char * fName;
	fName = new char[l+7+n/10];
	strcpy(fName, name);
	strcat(fName, "#0");
	init(fName);
	delete [] fName;
//	printf("%s: %i\n", fName, l);
	frame * x = this;
	for(int i = 1 ; i < n; i++){
		fName = new char[l+6+n];
		strcpy(fName, name);
		strcat(fName, "#");
		sprintf(num, "%i", i);
		strcat(fName, num);
		ifstream z(fName);
		if(z != NULL){
			x->next = new frame();
			x = x->next;
			x->init(fName);
		} else {
			x->next = NULL;
			i = n;
		}
		delete [] fName;
	}
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
