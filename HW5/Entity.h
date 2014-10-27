#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
using namespace std;

#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6
#define GRAVITY -1.0f

class Entity{
public:
	Entity( GLuint texture, float x, float y, float uVal, float vVal, float widthVal, float heightVal, bool visibility);
	
	void FixedUpdate();
	void drawRight( float scale );
	void drawLeft(float scale);
		
	bool collidesWith( Entity entity );
	float lerp(float v0, float v1, float t);
	void insertUVPoints(float u, float v);

	void setX(float newX);
	void setY(float newy);
	void setU(float newU);
	void setV(float newV);
	void setVelocityX(float velX);
	void setVelocityY(float velY);
	void setAccelX(float accelerationX);
	void setAccelY(float accelerationY);
	void setScale(float scale);
	void setVisible(bool isVisible);
	void setFacingLeft(bool isFacingLeft);

	float getX();
	float getY();
	float getWidth();
	float getHeight();
	bool getCollidedTop();
	bool getCollidedBottom();
	bool getCollidedLeft();
	bool getCollidedRight();
	float getVelocityY();
	float getScale();
	bool getVisible();
	bool getFacingLeft();

	void setCollidedTop(bool collided);
	void setCollidedBottom(bool collided);
	void setCollidedLeft(bool collided);
	void setCollidedRight(bool collided);

private:

	GLuint texture;
	float x;
	float y;
	float u;
	float v;
	float width;
	float height;
	bool visible;
	bool facingLeft;

	float velocityX;
	float velocityY;
	float frictionX;
	float frictionY;
	float accelerationX;
	float accelerationY;

	float mass;
	bool isStatic;
	float timeLeftOver = 0.0f;
	float scale;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

	vector<float> animation;

};