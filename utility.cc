#include "utility.h"
utility::utility(char * n)
{
	build(n);
	init();
	tolerance = 30;
	activation = 30;
	xLock = 0;
	yLock = 0;
}

bool utility::check(bool pos[5], bool neg[5], int t, int f)
{
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(!pos[i]) return 0;
		} else {
			if(pos[i]) return 0;
				
	}
	if(t > tolerance) return 0;
	if(f > activation) return 0;
	return 1;
}

}
