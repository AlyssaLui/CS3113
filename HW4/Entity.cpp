#include "Entity.h"

Entity::Entity(GLuint textureID, float uVal, float vVal, float widthVal, float heightVal, bool visibility, bool isStatic )
	:u(uVal), v(vVal), width(widthVal), height(heightVal), visible(visibility), texture(textureID), isStatic(isStatic) {}

void Entity::FixedUpdate() {
	accelerationY = GRAVITY;
	
	velocityX = lerp(velocityX, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocityY = lerp(velocityY, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocityX += accelerationX * FIXED_TIMESTEP;
	velocityY += accelerationY * FIXED_TIMESTEP;
	x += velocityX * FIXED_TIMESTEP;
	y += velocityY * FIXED_TIMESTEP;
	
}


void Entity::draw( float scale ) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	glDisable(GL_TEXTURE_2D);

	
}

bool Entity::collidesWith(Entity entity) {
	

	if (y - (height*scale*0.5) > (entity.getY() + entity.getHeight()*entity.getScale()*0.5) ||
		y + (height*scale*0.5) < (entity.getY() - entity.getHeight()*entity.getScale()*0.5) ||
		x - (width*scale*0.5) > (entity.getX() + entity.getWidth()*entity.getScale()*0.5) ||
		x + (width*scale*0.5) < (entity.getX() - entity.getWidth()*entity.getScale()*0.5)) {
		return false;
	}
	return true;
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

void Entity::setStatic(bool isStaticObj) {
	isStatic = isStaticObj;
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