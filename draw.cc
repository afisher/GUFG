#include "interface.h"
void interface::draw()
{
	SDL_SetColorKey(p1sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_SetColorKey(p2sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorKey);
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 255, 212, 120));
	SDL_BlitSurface(p1sprite, NULL, screen, &p1->pos);
	SDL_BlitSurface(p2sprite, NULL, screen, &p2->pos);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void interface::spriteInit()
{
	int displacement;
	SDL_Surface *sTemp;
	if(p1sprite) displacement = p1sprite->w;

	/*Doing moves*/
	if(p1->cMove != NULL){
		if(p1->facing == -1) { 
			p1sprite = SDL_DisplayFormat(p1->current->fSprite);
			p1->pos.x += (displacement - p1sprite->w);
		}
		else p1sprite = SDL_DisplayFormat(p1->current->sprite);
		p1->current = p1->current->next;
		p1->sFlag = 0;
	}
	else if(!p1->sFlag){
		char nsprt[strlen(p1->pick->name)+4];
		strcpy(nsprt, p1->pick->name);
		strcat(nsprt, "/");
		strcat(nsprt, "N");
		if(p1->facing == -1) strcat(nsprt, "F");
		sTemp = SDL_LoadBMP(nsprt);
		p1sprite = SDL_DisplayFormat(sTemp);
		if (p1->facing == -1)
			p1->pos.x += (displacement - p1sprite->w);
		SDL_FreeSurface(sTemp);
		p1->sFlag = 1;
	}

	if(p2sprite) displacement = p2sprite->w;

	/*Doing moves*/
	if(p2->cMove != NULL){
		if(p2->facing == -1) { 
			p2sprite = SDL_DisplayFormat(p2->current->fSprite);
			p2->pos.x += (displacement - p2sprite->w);
		}
		else p2sprite = SDL_DisplayFormat(p2->current->sprite);
		p2->current = p2->current->next;
		p2->sFlag = 0;
	}
	else if(!p2->sFlag){
		char nsprt[strlen(p2->pick->name)+4];
		strcpy(nsprt, p2->pick->name);
		strcat(nsprt, "/");
		strcat(nsprt, "N");
		if(p2->facing == -1) strcat(nsprt, "F");
		sTemp = SDL_LoadBMP(nsprt);
		p2sprite = SDL_DisplayFormat(sTemp);
		if (p2->facing == -1)
			p2->pos.x += (displacement - p2sprite->w);
		SDL_FreeSurface(sTemp);
		p2->sFlag = 1;
	}
}
