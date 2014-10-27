#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <SDL_mixer.h>

#include "Entity.h"
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

	bool readHeader( ifstream &stream );
	bool readLayerData(ifstream &stream);
	bool readEntityData ( ifstream &stream );
	void placeEntity( string type, float placeX, float placeY);
	void drawMap(GLuint texture);
	void worldToTileCoord(float worldX, float worldY, int *gridX, int *gridY);
	bool isSolid(unsigned char tiles);
	float checkCollisionY(float x, float y);
	float checkCollisionX(float x, float y);
	void collidesX(Entity* entity);
	void collidesY(Entity* entity);

	GLuint loadTexture(const char *image_path);
	void drawText(GLuint textTexture, string text, float x, float y, float size, float r, float g, float b, float a);
	const Uint8 *keys;

private:
	GLuint texture;
	GLuint font;
	int score = 0;

	bool done = false;
	float timeLeftOver = 0.0f;

	int mapWidth;
	int mapHeight;
	unsigned char** levelData;
	vector<Entity*> entities;
	
	Mix_Chunk *coinSound;
	Mix_Chunk *jumpSound;
	Mix_Music *music;

	SDL_Window* displayWindow;
	
};