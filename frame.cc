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
	char num[n];
	char * bfName = new char[l+6+n];
	char * fName;
	fName = new char[l+6+n/10];
	strcpy(bfName, name);
	strcat(bfName, "#");
	strcpy(fName, bfName);
	strcat(fName, "0.bmp");
	init(fName);
	delete [] fName;
//	printf("%s: %i\n", fName, l);
	frame * x = this;
	for(int i = 1 ; i < n; i++){
		fName = new char[l+6+n];
		strcpy(fName, bfName);
		sprintf(num, "%i", i);
		strcat(fName, num);
		strcat(fName, ".bmp");
		printf("%s\n", fName);
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
	delete [] bfName;
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
