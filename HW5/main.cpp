//Alyssa Lui
//Game Programming
//Assignment: Sidescrolling Platformer with Sound

#include "PlatformerGame.h"

int main(int argc, char *argv[])
{
	PlatformerGame newGame;
	while (!newGame.UpdateandRender()){}
	return 0;
}