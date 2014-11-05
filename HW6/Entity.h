#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <algorithm>
#include "Vector.h"
#include "Matrix.h"
using namespace std;

#define FIXED_TIMESTEP 0.01666666f
#define MAX_TIMESTEPS 6
//#define GRAVITY -1.0f
#define PI 3.14195

class Entity{
public:
	Entity( GLuint texture, float uVal, float vVal, float widthVal, float heightVal, bool visibility);
	
	void FixedUpdate();

	void render();
	void buildMatrix();

	bool satCollidesWith(Entity entity);

	//bool collidesWith( Entity entity );
	float lerp(float v0, float v1, float t);
	

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
	void setRotation(float newRotation);
	void setWidth(float newWidth);
	void setHeight(float newHeight);

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
	float getRotation();
	float getAccelX();
	float getAccelY();

	void setCollidedTop(bool collided);
	void setCollidedBottom(bool collided);
	void setCollidedLeft(bool collided);
	void setCollidedRight(bool collided);

private:

	GLuint texture;
	
	float x;
	float y;
	float scaleX = 1.0;
	float scaleY = 1.0;
	float rotation = 0.0;

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
	float accelerationX = 0.0;
	float accelerationY = 0.0;

	float mass;
	bool isStatic;
	float timeLeftOver = 0.0f;
	//float scale;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

	vector<float> animation;
	Matrix matrix;
};