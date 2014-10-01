#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Bullet {
public:
	Bullet();
	Bullet( float x, float y, float u, float v, float width, float height, GLuint texture );
	void draw( float scale );

	void setX( float posX );
	void setY( float posY );
	void setVisible( bool visible );
	void setTexure(GLuint newTexture);
	float getX();
	float getY();
	bool getVisible();

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

