/*Drawing routines for project: Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012, under MIT OSI
 *For detailed license information, see the file COPYING in this directory
 */

#include "interface.h"
void interface::draw()
{
	SDL_Rect bar1, bar2;
	if(p1->pick->health >= 0) bar1.w = p1->pick->health*3; else bar1.w = 1; 
	if(p1->pick->health >= 0) bar2.w = p2->pick->health*3; else bar2.w = 1;
	bar1.x = 50 + (300 - bar1.w); bar2.x = 450;
	bar1.h = 5; bar2.h = 5;
	bar1.y = 5; bar2.y = 5;
	SDL_SetColorKey(p1->sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_SetColorKey(p2->sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_FillRect(screen, &bar1, SDL_MapRGB(screen->format, 255, 0, 0));
	SDL_FillRect(screen, &bar2, SDL_MapRGB(screen->format, 255, 0, 0));
	SDL_BlitSurface(p1->sprite, NULL, screen, &p1->pos);
	SDL_BlitSurface(p2->sprite, NULL, screen, &p2->pos);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void player::spriteInit()
{
	int displacement;
	SDL_Surface *sTemp;
	if(sprite) displacement = sprite->w;

	/*Doing moves*/
	if(current == NULL) pick->neutral->execute(current);
	if(facing == -1) { 
		sprite = SDL_DisplayFormat(current->fSprite);
		pos.x += (displacement - sprite->w);
	}
	else sprite = SDL_DisplayFormat(current->sprite);
	current = current->next;
}
