#include "Vector.h"

Vector::Vector() {}

Vector::Vector(float x, float y, float z) 
	: x(x), y(y), z(z) {}

float Vector::length() {
	return sqrt((x*x) + (y*y));
}

void Vector::normalize() {
	float length = this->length();
	x = x / length;
	y = y / length;
	z = z / length;
}

float Vector::dotProduct(Vector vecOne) {
	return (x*vecOne.x) + (y*vecOne.y);
}