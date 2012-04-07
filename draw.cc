/*Drawing routines for project: Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012, under MIT OSI
 *For detailed license information, see the file COPYING in this directory
 */

#include "interface.h"
void interface::draw()
{
	SDL_Rect bar1, bar2, meter1, meter2, rounds1[numRounds], rounds2[numRounds];
	
	if(p[0]->pick->health >= 0) bar1.w = p[0]->pick->health; else bar1.w = 1; 
	if(p[0]->pick->health >= 0) bar2.w = p[1]->pick->health; else bar1.w = 1;
	
	bar1.x = 50 + (300 - bar1.w); bar2.x = 450;
	bar1.h = 5; bar2.h = 5;
	bar1.y = 5; bar2.y = 5;

	for(int i = 0; i < numRounds; i++){
		rounds1[i].y = 12; rounds1[i].w = 10; rounds1[i].h = 5;
		rounds2[i].y = 12; rounds2[i].w = 10; rounds2[i].h = 5;
		rounds1[i].x = 340 - 12 * i; rounds2[i].x = 450 + 12 * i;
	}

	SDL_SetColorKey(p[0]->sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_SetColorKey(p[1]->sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_BlitSurface(background, NULL, screen, &bg);
	
	for(int i = 0; i < numRounds; i++){
		if(p[0]->rounds > i) SDL_FillRect(screen, &rounds1[i], SDL_MapRGB(screen->format, 0, 255, 255));
		else SDL_FillRect(screen, &rounds1[i], SDL_MapRGB(screen->format, 0, 0, 0));
		if(p[1]->rounds > i) SDL_FillRect(screen, &rounds2[i], SDL_MapRGB(screen->format, 0, 255, 255));
		else SDL_FillRect(screen, &rounds2[i], SDL_MapRGB(screen->format, 0, 0, 0));
	}

	SDL_FillRect(screen, &bar1, SDL_MapRGB(screen->format, 255, 0, 0));
	SDL_FillRect(screen, &bar2, SDL_MapRGB(screen->format, 255, 0, 0));
	
	SDL_BlitSurface(p[0]->sprite, NULL, screen, &p[0]->pos);
	SDL_BlitSurface(p[1]->sprite, NULL, screen, &p[1]->pos);
	
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void player::spriteInit()
{
	int displacement;
	SDL_Surface *sTemp;
	if(sprite) displacement = sprite->w;

	/*Doing moves*/
	if(pick->cMove == NULL) pick->cMove = pick->neutral;
	sTemp = pick->draw(facing);
	if(sTemp != NULL) sprite = SDL_DisplayFormat(sTemp);
	if(facing == -1) pos.x += (displacement - sprite->w);
	
}

SDL_Surface * character::draw(int facing){
	SDL_Surface * temp;
	if(freeze < 0) freeze = 0;
	temp = cMove->draw(facing, freeze);
	if(freeze > 0) freeze--;
	if(cMove->currentFrame == cMove->frames){
		cMove->init();
		cMove = NULL;
	}
	return temp;
}

SDL_Surface * move::draw(int facing, bool freeze)
{
	SDL_Surface * temp;
	if(facing == -1) temp = fSprite[currentFrame];
	else temp = sprite[currentFrame];
	if(freeze == 0) 
		step();
	return temp;
}
