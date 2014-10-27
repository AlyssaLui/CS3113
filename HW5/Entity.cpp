#include "Entity.h"

Entity::Entity(GLuint textureID, float xVal, float yVal, float uVal, float vVal, float widthVal, float heightVal, bool visibility)
	:x(xVal), y(yVal), u(uVal), v(vVal), width(widthVal), height(heightVal), visible(visibility), texture(textureID) {}

void Entity::FixedUpdate() {
	accelerationY = GRAVITY;
	
	velocityX = lerp(velocityX, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocityY = lerp(velocityY, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocityX += accelerationX * FIXED_TIMESTEP;
	velocityY += accelerationY * FIXED_TIMESTEP;
	x += velocityX * FIXED_TIMESTEP;
	y += velocityY * FIXED_TIMESTEP;
	
}


void Entity::drawRight( float scale ) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	
	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	GLfloat quad[] = { width*0.5*-1.0f*scale, height*0.5*scale, width*0.5*-1.0f*scale, height*0.5*-1.0f*scale, width*0.5*scale, height*0.5*-1.0f*scale, width*0.5*scale, height*0.5*scale };
	

	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();
}

void Entity::drawLeft(float scale) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLfloat quad[] = { width*0.5*-1.0f*scale, height*0.5*scale, width*0.5*-1.0f*scale, height*0.5*-1.0f*scale, width*0.5*scale, height*0.5*-1.0f*scale, width*0.5*scale, height*0.5*scale };


	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u + width, v, u + width, v + height, u, v + height, u, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);

	glPopMatrix();
}
bool Entity::collidesWith(Entity entity) {
	
	if (x > entity.getX() - entity.getWidth()*0.5f && x < entity.getX() + entity.getWidth()*0.5f &&
		y > entity.getY() - entity.getHeight()*0.5 && y < entity.getY() + entity.getHeight()*0.5) {
		return true;
	}
	return false;
}

float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

void Entity::insertUVPoints(float u, float v) {
	animation.push_back(u);
	animation.push_back(v);
}

void Entity::setX(float newX) {
	x = newX;
}

void Entity::setY(float newY) {
	y = newY;
}

void Entity::setU(float newU) {
	u = newU;
}

void Entity::setV(float newV) {
	v = newV;
}

void Entity::setVelocityX(float velX) {
	velocityX = velX;
}

void Entity::setVelocityY(float velY) {
	velocityY = velY;
}

void Entity::setAccelX(float accelX) {
	accelerationX = accelX;
}

void Entity::setAccelY(float accelY){
	accelerationY = accelY;
}

void Entity::setScale(float newScale) {
	scale = newScale;
}

float Entity::getWidth() {
	return width;
}

float Entity::getHeight() {
	return height;
}

float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

bool Entity::getCollidedTop() {
	return collidedTop;
}

bool Entity::getCollidedBottom() {
	return collidedBottom;
}

bool Entity::getCollidedLeft() {
	return collidedLeft;
}

bool Entity::getCollidedRight() {
	return collidedRight;
}

float Entity::getVelocityY() {
	return velocityY;
}

float Entity::getScale() {
	return scale;
}

void Entity::setCollidedTop(bool collided) {
	collidedTop = collided;
}

void Entity::setCollidedBottom(bool collided) {
	collidedBottom = collided;
}

void Entity::setCollidedLeft(bool collided) {
	collidedLeft = collided;
}

void Entity::setCollidedRight(bool collided) {
	collidedRight = collided;
}

void Entity::setVisible(bool isVisible) {
	visible = isVisible;
}

bool Entity::getVisible() {
	return visible;
}

void Entity::setFacingLeft(bool isFacingLeft) {
	facingLeft = isFacingLeft;
}

bool Entity::getFacingLeft() {
	return facingLeft;
}