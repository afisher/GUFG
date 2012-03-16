#include "move.h"

class hitstun : public move {
public:
	void init(int);
	int counter;
	void step();
	hitstun(char *, int);
};
