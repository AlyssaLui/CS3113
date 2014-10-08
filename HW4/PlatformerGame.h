#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include "Entity.h"
using namespace std;

#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6
class PlatformerGame {
public:
	PlatformerGame();
	
	void Init();
	bool UpdateandRender();
	void FixedUpdate();
	void Render();


	GLuint loadTexture(const char *image_path);
	const Uint8 *keys;

private:
	bool done = false;
	float timeLeftOver = 0.0f;

	SDL_Window* displayWindow;
	
	vector<Entity> staticEntities;
	vector<Entity> dynamicEntities;
};