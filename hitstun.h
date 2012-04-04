#include "moveTrie.h"

class hitstun : public move {
public:
	void init(int);
	int counter;
	void step();
	void blockSuccess(int);
	hitstun();
	hitstun(char *, int);
	hitstun(char *);
};
