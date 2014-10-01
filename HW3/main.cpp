//Alyssa Lui
//Game Programming
//Assignment: Space Invaders

#include "SpaceInvaders.h"

int main(int argc, char *argv[])
{

	SpaceInvaders newGame;
	while(!newGame.Update()) {}
	
	return 0;
}