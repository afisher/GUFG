#include "special.h"

bool special::check(bool pos[5], bool neg[5], int t)
{
	for(int i = 0; i < 5; i++){
		if(button[i] == 1){
			if(!pos[i] && !neg[i]) return 0;
		}				
	}
	if(t > tolerance) return 0;
	return 1;
}
