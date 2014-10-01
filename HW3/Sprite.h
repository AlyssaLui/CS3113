#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Sprite {
public:
	Sprite( float x, float y, float u, float v, float width, float height, bool visible, GLuint texture );
	void drawSprite(float scale);

	float getX();
	float getY();
	float getWidth();
	float getHeight();

	void setX( float newX );
	void setY( float newY );

private:
	float x;
	float y;
	float u;
	float v;
	float width;
	float height;
	bool visible;
	GLuint texture;


};