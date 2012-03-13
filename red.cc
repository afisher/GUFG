#include "red.h"
red::red()
{
	name = "Red\0";
	head = new moveTrie;
	head->insert(new move("Red/A\0", "A", 0, 10));
}
