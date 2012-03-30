/*Header for frame class for project: Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012
 *Under the MIT OSI software license.
 *For more licensing information, see the file COPYING in this directory
 */

#include "SDL.h"

class frame{
public:
	frame();
	void init(char *);
	frame(char *, int);
	~frame();
	SDL_Surface *sprite, *fSprite;
	frame * next;
	frame * operator[](const int&);
	frame * get(int);
};
