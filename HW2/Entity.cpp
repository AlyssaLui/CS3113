#include "Entity.h"

Entity::Entity( float x, float y, float width, float height, GLuint texture) { 

	posX = x;
	posY = y;
	Entity::width = width;
	Entity::height = height;
	rotation = 0.0f;
	textureID = texture;

	
	float speed;
	float directionX = 0.0;
	float directionY = 0.0;
	
}

void Entity::draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(posX, posY, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { width*-0.5f , height*0.5f , width*-0.5f, height*-0.5f, width*0.5f, height*-0.5f, width*0.5f, height*0.5f }; //this line will scale the sprite not the next line
	//GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void Entity::setY( float newY ) {
	posY = newY;
}

float Entity::getY() {
	return posY;
}

void Entity::setX( float newX ) {
	posX = newX;
}

float Entity::getX() {
	return posX;
}

float Entity::getDirectionX() {
	return directionX;
}

float Entity::getDirectionY() {
	return directionY;
}

void Entity::changeDirection( float newXDir, float newYDir ) {
	directionX = newXDir;
	directionY = newYDir;
}

void Entity::setSpeed( float newSpeed ) {
	speed = newSpeed;
}

float Entity::getSpeed() {
	return speed;
}

float Entity::getWidth() {
	return width;
}

float Entity::getHeight() {
	return height;
}