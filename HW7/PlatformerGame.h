#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
//#include <SDL_mixer.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "Entity.h"
#include "Vector.h"
#include "Matrix.h"

using namespace std;

#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6
#define TILE_SIZE 0.1f

class PlatformerGame {
public:
	PlatformerGame();
	~PlatformerGame();
	
	void Init();
	bool UpdateandRender();
	void FixedUpdate();
	void Render();
	float easeIn(float from, float to, float time);

	float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax);
	GLuint loadTexture(const char *image_path);
	const Uint8 *keys;

private:
	GLuint texture;
	GLuint font;
	int score = 0;

	bool done = false;


	float animationTime = 0.0;
	float animationValue;
	float animationTime2 = 0.0;
	float animationValue2;

	float screenShakeSpeed = 20.0;
	float screenShakeIntensity = 0.1;

	float timeLeftOver = 0.0f;
	float lastFrameTicks;
	
	Entity* rocket;
	vector<Entity*> asteroids;

	GLuint effectTexture;
	Entity* glow;
	Entity* word;

	SDL_Window* displayWindow;
	
};