//Alyssa Lui
//Game Programming
//Assignment: Asteroids

#include "PlatformerGame.h"

int main(int argc, char *argv[])
{
	PlatformerGame newGame;
	while (!newGame.UpdateandRender()){}
	return 0;
}