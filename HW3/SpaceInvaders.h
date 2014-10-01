#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <algorithm>
#include "Bullet.h"
#include "Sprite.h"
using namespace std;

#define MAX_BULLETS 100

enum GameState { STATE_MAIN_MENU , STATE_GAME_LEVEL, STATE_GAME_OVER };

class SpaceInvaders {
public:
	SpaceInvaders();

	void Setup();
	bool Update();
	void Render();	
	
	void renderMain();
	void renderLevel();
	void renderEnd();
	void dropBone();

	vector<Sprite> puppies;
	
	GLuint loadTexture(const char *image_path);
	void drawText(unsigned textTexture, string text, float x, float y, float size, float r, float g, float b, float a);
	void shootBullet( Sprite obj);

	GLuint textSheet;
	const Uint8 *keys;
	

private:
	bool done;
	int state;
	int movedirection;
	int bulletIndex;
	int score;
	int cooldown = 0;
	int dropBuffer = 0;

	vector<Bullet> bullets;
	vector<Sprite> kitties;
	vector<Bullet> bones;
	
	SDL_Window* displayWindow;
};
