#include "Entity.h"

Entity::Entity(GLuint textureID, float uVal, float vVal, float widthVal, float heightVal, bool visibility)
	:u(uVal), v(vVal), width(widthVal), height(heightVal), visible(visibility), texture(textureID) {}

void Entity::FixedUpdate() {


	velocityX = lerp(velocityX, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocityY = lerp(velocityY, 0.0f, FIXED_TIMESTEP * 2.0f);
	velocityX += accelerationX * FIXED_TIMESTEP;
	velocityY += accelerationY * FIXED_TIMESTEP;
	x += velocityX * FIXED_TIMESTEP;
	y += velocityY * FIXED_TIMESTEP;
	
}

void Entity::render() {

	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
	GLfloat quad[] = { width*0.5*-1.0f, height*0.5, width*0.5*-1.0f, height*0.5*-1.0f, width*0.5, height*0.5*-1.0f, width*0.5, height*0.5};
		
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

void Entity::buildMatrix() {
	matrix.identity();

	Matrix scaleMatrix;
	scaleMatrix.m[0][0] = scaleX;
	scaleMatrix.m[1][1] = scaleY;
	
	Matrix translateMatrix;
	translateMatrix.m[3][0] = x;
	translateMatrix.m[3][1] = y;

	Matrix rotateMatrix;
	GLfloat rotate = ((rotation + rOffset) * PI) / 180.0f;
	rotateMatrix.m[0][0] = cos(rotate);
	rotateMatrix.m[1][0] = -sin(rotate);
	rotateMatrix.m[0][1] = sin(rotate);
	rotateMatrix.m[1][1] = cos(rotate);

	matrix = scaleMatrix * rotateMatrix * translateMatrix;
	//matrix = translateMatrix * rotateMatrix *  scaleMatrix;
	
}

bool Entity::satCollidesWith(Entity otherEntity) {
		
	buildMatrix();
	otherEntity.buildMatrix();

	vector<Vector> penDist;

	// My entity to world
	Vector topLeft = Vector(-width*0.5, height*0.5, 0.0);
	Vector topRight = Vector(width*0.5, height*0.5, 0.0);
	Vector bottomLeft = Vector(-width*0.5, -height*0.5, 0.0);
	Vector bottomRight = Vector(width*0.5, -height*0.5, 0.0);
	
	Vector objectToWorldTL = matrix * topLeft;
	Vector objectToWorldTR = matrix * topRight;
	Vector objectToWorldBL = matrix * bottomLeft;
	Vector objectToWorldBR = matrix * bottomRight;

	// Other entity to world
	Vector otherTopLeft = Vector(-otherEntity.getWidth()*0.5, otherEntity.getHeight()*0.5, 0.0);
	Vector otherTopRight = Vector(otherEntity.getWidth()*0.5, otherEntity.getHeight()*0.5, 0.0);
	Vector otherBottomLeft = Vector(-otherEntity.getWidth()*0.5, -otherEntity.getHeight()*0.5, 0.0);
	Vector otherBottomRight = Vector(otherEntity.getWidth()*0.5, -otherEntity.getHeight()*0.5, 0.0);

	Vector objectToWorldTL2 = otherEntity.matrix * topLeft;
	Vector objectToWorldTR2 = otherEntity.matrix * topRight;
	Vector objectToWorldBL2 = otherEntity.matrix * bottomLeft;
	Vector objectToWorldBR2 = otherEntity.matrix * bottomRight;

	// Normalized Axes
	float edgeX = objectToWorldTR.x - objectToWorldTL.x;
	float edgeY = objectToWorldTR.y - objectToWorldTL.y;
	Vector normalizedAxis = Vector(edgeY, -edgeX, 0.0);
	normalizedAxis.normalize();

	float edgeX2 = objectToWorldTL.x - objectToWorldBR.x;
	float edgeY2 = objectToWorldTL.y - objectToWorldBR.y;
	Vector normalizedAxis2 = Vector(edgeY2, -edgeX2, 0.0);
	normalizedAxis2.normalize();

	float edgeX3 = objectToWorldTR2.x - objectToWorldTL2.x;
	float edgeY3 = objectToWorldTR2.y - objectToWorldTL2.y;
	Vector normalizedAxis3 = Vector(edgeY3, -edgeX3, 0.0);
	normalizedAxis3.normalize();

	float edgeX4 = objectToWorldTL2.x - objectToWorldBR2.x;
	float edgeY4 = objectToWorldTL2.y - objectToWorldBR2.y;
	Vector normalizedAxis4 = Vector(edgeY4, -edgeX4, 0.0);
	normalizedAxis4.normalize();

	// normalizedAxis
	float myTLval = normalizedAxis.dotProduct(objectToWorldTL);
	float myTRval = normalizedAxis.dotProduct(objectToWorldTR);
	float myBLval = normalizedAxis.dotProduct(objectToWorldBL);
	float myBRval = normalizedAxis.dotProduct(objectToWorldBR);

	float minPos = min(min(min(myTLval, myTRval), myBLval), myBRval);
	float maxPos = max(min(min(myTLval, myTRval), myBLval), myBRval);

	float otherTLval = normalizedAxis.dotProduct(objectToWorldTL2);
	float otherTRval = normalizedAxis.dotProduct(objectToWorldTR2);
	float otherBLval = normalizedAxis.dotProduct(objectToWorldBL2);
	float otherBRval = normalizedAxis.dotProduct(objectToWorldBR2);

	float otherminPos = min(min(min(otherTLval, otherTRval), otherBLval), otherBRval);
	float othermaxPos = max(min(min(otherTLval, otherTRval), otherBLval), otherBRval);

	if (minPos <= othermaxPos && maxPos >= otherminPos) {
		float penetration = min(maxPos - otherminPos, othermaxPos - minPos);
		Vector distance = Vector(penetration*normalizedAxis.x, penetration*normalizedAxis.y, 0.0);
		penDist.push_back(distance);
	}
	else 
		return false;
	
	// normalizedAxis2
	float myTLval2 = normalizedAxis2.dotProduct(objectToWorldTL);
	float myTRval2 = normalizedAxis2.dotProduct(objectToWorldTR);
	float myBLval2 = normalizedAxis2.dotProduct(objectToWorldBL);
	float myBRval2 = normalizedAxis2.dotProduct(objectToWorldBR);

	float minPos2 = min(min(min(myTLval2, myTRval2), myBLval2), myBRval2);
	float maxPos2 = max(min(min(myTLval2, myTRval2), myBLval2), myBRval2);

	float otherTLval2 = normalizedAxis2.dotProduct(objectToWorldTL2);
	float otherTRval2 = normalizedAxis2.dotProduct(objectToWorldTR2);
	float otherBLval2 = normalizedAxis2.dotProduct(objectToWorldBL2);
	float otherBRval2 = normalizedAxis2.dotProduct(objectToWorldBR2);

	float otherminPos2 = min(min(min(otherTLval2, otherTRval2), otherBLval2), otherBRval2);
	float othermaxPos2 = max(min(min(otherTLval2, otherTRval2), otherBLval2), otherBRval2);

	if (minPos2 <= othermaxPos2 && maxPos2 >= otherminPos2) {
		float penetration2 = min(maxPos2 - otherminPos2, othermaxPos2 - minPos2);
		Vector distance2 = Vector(penetration2*normalizedAxis2.x, penetration2*normalizedAxis2.y, 0.0);
		penDist.push_back(distance2);
	}
	else
		return false;

	// normalizedAxis3
	float myTLval3 = normalizedAxis3.dotProduct(objectToWorldTL);
	float myTRval3 = normalizedAxis3.dotProduct(objectToWorldTR);
	float myBLval3 = normalizedAxis3.dotProduct(objectToWorldBL);
	float myBRval3 = normalizedAxis3.dotProduct(objectToWorldBR);

	float minPos3 = min(min(min(myTLval3, myTRval3), myBLval3), myBRval3);
	float maxPos3 = max(min(min(myTLval3, myTRval3), myBLval3), myBRval3);

	float otherTLval3 = normalizedAxis3.dotProduct(objectToWorldTL2);
	float otherTRval3 = normalizedAxis3.dotProduct(objectToWorldTR2);
	float otherBLval3 = normalizedAxis3.dotProduct(objectToWorldBL2);
	float otherBRval3 = normalizedAxis3.dotProduct(objectToWorldBR2);

	float otherminPos3 = min(min(min(otherTLval3, otherTRval3), otherBLval3), otherBRval3);
	float othermaxPos3 = max(min(min(otherTLval3, otherTRval3), otherBLval3), otherBRval3);

	if (minPos3 <= othermaxPos3 && maxPos3 >= otherminPos3) {
		float penetration3 = min(maxPos3 - otherminPos3, othermaxPos3 - minPos3);
		Vector distance3 = Vector(penetration3*normalizedAxis3.x, penetration3*normalizedAxis3.y, 0.0);
		penDist.push_back(distance3);
	}
	else
		return false;

	// normalizedAxis4
	float myTLval4 = normalizedAxis4.dotProduct(objectToWorldTL);
	float myTRval4 = normalizedAxis4.dotProduct(objectToWorldTR);
	float myBLval4 = normalizedAxis4.dotProduct(objectToWorldBL);
	float myBRval4 = normalizedAxis4.dotProduct(objectToWorldBR);

	float minPos4 = min(min(min(myTLval4, myTRval4), myBLval4), myBRval4);
	float maxPos4 = max(min(min(myTLval4, myTRval4), myBLval4), myBRval4);

	float otherTLval4 = normalizedAxis4.dotProduct(objectToWorldTL2);
	float otherTRval4 = normalizedAxis4.dotProduct(objectToWorldTR2);
	float otherBLval4 = normalizedAxis4.dotProduct(objectToWorldBL2);
	float otherBRval4 = normalizedAxis4.dotProduct(objectToWorldBR2);

	float otherminPos4 = min(min(min(otherTLval4, otherTRval4), otherBLval4), otherBRval4);
	float othermaxPos4 = max(min(min(otherTLval4, otherTRval4), otherBLval4), otherBRval4);

	if (minPos4 <= othermaxPos4 && maxPos4 >= otherminPos4) {
		float penetration4 = min(maxPos4 - otherminPos4, othermaxPos4 - minPos4);
		Vector distance4 = Vector(penetration4*normalizedAxis4.x, penetration4*normalizedAxis4.y, 0.0);
		penDist.push_back(distance4);
	}
	else
		return false;

	float shortestX = 0.0;
	float shortestY = 0.0;
	float shortestLength = penDist[0].length();
	for (int i = 0; i < penDist.size(); i++) {
		if (penDist[i].length() < shortestLength) {
			shortestX = penDist[i].x;
			shortestY = penDist[i].y;
			shortestLength = penDist[i].length();
		}
	}

	x = x - (shortestX / 2);
	y = y - (shortestY / 2);
	otherEntity.setX(otherEntity.getX() - (shortestX / 2));
	otherEntity.setY(otherEntity.getY() - (shortestY / 2));

	return true;
}


float Entity::lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
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

void Entity::setRotation(float newRotation) {
	rotation = newRotation;
}

float Entity::getRotation() {
	return rotation;
}

float Entity::getAccelX() {
	return accelerationX;
}
float Entity::getAccelY() {
	return accelerationY;
}

void Entity::setWidth(float newWidth) {
	width = newWidth;
}

void Entity::setHeight(float newHeight) {
	height = newHeight;
}