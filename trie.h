/*Header file for "move" class in project Ground Up Fighting Game
 *
 *Written by Alex Kelly in 2012. 
 *Licensed under MIT OSI, see "COPYING" for details
 */

#include "move.h"
#include <SDL/SDL.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

/*Move trie class. Essentially a recursive trie-search algorithm that looks for input hooks*/

class moveTrie{
public:
	moveTrie();
	moveTrie(move*);
	moveTrie(move*, char*);
	~moveTrie();
	move * moveHook(int[40], int, int, bool[5], bool[5], move *);
	moveTrie * child[10];
	move ** fish; 		//Because it's what a hook catches! Okay, this name probably needs to change.
	moveTrie * insert(int);
	moveTrie * insert(int, move*);
	void insert(move *);
private:
	int occupants;
	int * button;
};
