#include "Bullet.h"

Bullet::Bullet() {}

Bullet::Bullet(float xVal, float yVal, float uVal, float vVal, float widthVal, float heightVal, GLuint spriteTexture) {
	x = xVal;
	y = yVal;
	u = uVal;
	v = vVal;
	width = widthVal;
	height = heightVal;
	texture = spriteTexture;
	visible = false;
}

void Bullet::draw( float scale ) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	GLfloat quad[] = { width*-1.0f*scale, height*scale, width*-1.0f*scale, height*-1.0f*scale, width*scale, height*-1.0f*scale, width*scale, height*scale };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
};

void Bullet::setX( float xPos ) {
	x = xPos;
}

void Bullet::setY( float yPos ) {
	y = yPos;
}

void Bullet::setVisible( bool isVisible ) {
	visible = isVisible;
}

void Bullet::setTexure(GLuint newTexture) {
	texture = newTexture;
}

float Bullet::getX() {
	return x;
}

float Bullet::getY() {
	return y;
}

bool Bullet::getVisible() {
	return visible;
}