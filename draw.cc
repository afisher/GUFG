#include "interface.h"
void interface::draw()
{
	SDL_SetColorKey(p1->sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_SetColorKey(p2->sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
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
	if(current != NULL){
		if(facing == -1) { 
			sprite = SDL_DisplayFormat(current->fSprite);
			pos.x += (displacement - sprite->w);
		}
		else sprite = SDL_DisplayFormat(current->sprite);
		current = current->next;
		sFlag = 0;
	}
	else if(!sFlag){
		char nsprt[strlen(pick->name)+4];
		strcpy(nsprt, pick->name);
		strcat(nsprt, "/");
		strcat(nsprt, "N");
		if(facing == -1) strcat(nsprt, "F");
		sTemp = SDL_LoadBMP(nsprt);
		sprite = SDL_DisplayFormat(sTemp);
		if (facing == -1)
			pos.x += (displacement - sprite->w);
		SDL_FreeSurface(sTemp);
		sFlag = 1;
	}
}
