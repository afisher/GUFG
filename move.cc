#include "move.h"
int move::check(int meter, int state)
{
	if(meter < cost) return 0;
}

void move::execute()
{
	printf("Hook for %s detected\n", name);
}
