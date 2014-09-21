#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity {
private:

	float posX;
	float posY;

	float rotation;

	GLuint textureID;
	float scale;
	float width;
	float height;
	
	float speed;
	float directionX;
	float directionY;
	
public:
	Entity( float x, float y, float width, float height, GLuint texture );

	void draw();

	void setY( float newY );
	void setX( float newX );
	void setSpeed( float newSpeed );
	float getY();
	float getX();
	float getSpeed();
	float getWidth();
	float getHeight();
	

	float getDirectionX();
	float getDirectionY();
	void changeDirection( float newXDir, float newYDir );
	
};