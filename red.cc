#include "red.h"
red::red()
{
	name = "Red";
	head = new moveTrie;
	head->insert(new move("Red/A", "A", 0, 10));
	head->fish[0].debugStateInit(1, 1);
}
