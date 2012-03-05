#include "SDL.h"
#include "move.h"
#include <stdio.h>


class character
{
public:
	character(); 	//Load the entire character into memory
	~character();	//Free stuff

	moveTrie * head;

	/*Movement options. For now they are just moves, might treat them separately if it turns out that's 
	better*/
	
	void m5(); 	//Standing loop
	void m6(); 	//Walk forward
	void m4(); 	//Walk back/Block
	void m2(); 	//Crouch
	void m1();	//Crouchblock
	void m8(); 	//Jump
	void m66(); 	//Forward dash
	void m44(); 	//Backdash 

	/*Moves. I figured it's easiest to just make these functions. Actual characters will have specials 
	and command normals. I might end up making move a class rather than a function, depending on whether 
	it's needed.*/
	
	void m5A(); 
	void m5B();
	void m5C();
	void m5D();
	void m5E();
	void m2A();
	void m2B();
	void m2C();
	void m2D();
	void m2E();
	void j5A();
	void j5B();
	void j5C();
	void j5D();
	void j5E();

	/*I'm writing a special move hook in here for testing purposes. White will not have special moves once we implement any characters*/
	void m236A();

private:

	char * directory; //The name of the directory from which the character spawns. This is important for loading into memory
	int state; 	//The current state of the character. This might need to be more variables. We'll see.


};	
