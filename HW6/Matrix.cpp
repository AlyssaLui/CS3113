#include "Matrix.h"

Matrix::Matrix() { identity(); }

void Matrix::identity() {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				m[i][j] = 1;
			}
			else
				m[i][j] = 0;
		}
	}
}

Matrix Matrix::inverse() {
	
	float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
	float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
	float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
	float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

	float v0 = m20 * m31 - m21 * m30;
	float v1 = m20 * m32 - m22 * m30;
	float v2 = m20 * m33 - m23 * m30;
	float v3 = m21 * m32 - m22 * m31;
	float v4 = m21 * m33 - m23 * m31;
	float v5 = m22 * m33 - m23 * m32;

	float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
	float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
	float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

	float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

	float d00 = t00 * invDet;
	float d10 = t10 * invDet;
	float d20 = t20 * invDet;
	float d30 = t30 * invDet;

	float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;

	float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;

	float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
	Matrix m2;
		
	m2.m[0][0] = d00;
	m2.m[0][1] = d01;
	m2.m[0][2] = d02;
	m2.m[0][3] = d03;
	m2.m[1][0] = d10;
	m2.m[1][1] = d11;
	m2.m[1][2] = d12;
	m2.m[1][3] = d13;
	m2.m[2][0] = d20;
	m2.m[2][1] = d21;
	m2.m[2][2] = d22;
	m2.m[2][3] = d23;
	m2.m[3][0] = d30;
	m2.m[3][1] = d31;
	m2.m[3][2] = d32;
	m2.m[3][3] = d33;
		
	return m2;
	
}

Matrix Matrix::operator * (const Matrix &m2) {
	
	float m00 = m[0][0], m01 = m[1][0], m02 = m[2][0], m03 = m[3][0];
	float m10 = m[0][1], m11 = m[1][1], m12 = m[2][1], m13 = m[3][1];
	float m20 = m[0][2], m21 = m[1][2], m22 = m[2][2], m23 = m[3][2];
	float m30 = m[0][3], m31 = m[1][3], m32 = m[2][3], m33 = m[3][3];

	float mt00 = m2.m[0][0], mt01 = m2.m[1][0], mt02 = m2.m[2][0], mt03 = m2.m[3][0];
	float mt10 = m2.m[0][1], mt11 = m2.m[1][1], mt12 = m2.m[2][1], mt13 = m2.m[3][1];
	float mt20 = m2.m[0][2], mt21 = m2.m[1][2], mt22 = m2.m[2][2], mt23 = m2.m[3][2];
	float mt30 = m2.m[0][3], mt31 = m2.m[1][3], mt32 = m2.m[2][3], mt33 = m2.m[3][3];

	//row 1
	float r11 = m00*mt00 + m01*mt10 + m02*mt20 + m03*mt30;
	float r12 = m00*mt01 + m01*mt11 + m02*mt21 + m03*mt31;
	float r13 = m00*mt02 + m01*mt12 + m02*mt22 + m03*mt32;
	float r14 = m00*mt03 + m01*mt13 + m02*mt23 + m03*mt33;

	//row 2
	float r21 = m10*mt00 + m11*mt10 + m12*mt20 + m13*mt30;
	float r22 = m10*mt01 + m11*mt11 + m12*mt21 + m13*mt31;
	float r23 = m10*mt02 + m11*mt12 + m12*mt22 + m13*mt32;
	float r24 = m10*mt03 + m11*mt13 + m12*mt23 + m13*mt33;

	//row 3
	float r31 = m20*mt00 + m21*mt10 + m22*mt20 + m23*mt30;
	float r32 = m20*mt01 + m21*mt11 + m22*mt21 + m23*mt31;
	float r33 = m20*mt02 + m21*mt12 + m22*mt22 + m23*mt32;
	float r34 = m20*mt03 + m21*mt13 + m22*mt23 + m23*mt33;

	//row 4
	float r41 = m30*mt00 + m31*mt10 + m32*mt20 + m33*mt30;
	float r42 = m30*mt01 + m31*mt11 + m32*mt21 + m33*mt31;
	float r43 = m30*mt02 + m31*mt12 + m32*mt22 + m33*mt32;
	float r44 = m30*mt03 + m31*mt13 + m32*mt23 + m33*mt33;

	Matrix m3;
	m3.m[0][0] = r11;
	m3.m[1][0] = r12;
	m3.m[2][0] = r13;
	m3.m[3][0] = r14;
	m3.m[0][1] = r21;
	m3.m[1][1] = r22;
	m3.m[2][1] = r23;
	m3.m[3][1] = r24;
	m3.m[0][2] = r31;
	m3.m[1][2] = r32;
	m3.m[2][2] = r33;
	m3.m[3][2] = r34;
	m3.m[0][3] = r41;
	m3.m[1][3] = r42;
	m3.m[2][3] = r43;
	m3.m[3][3] = r44;

	return m3;

}

Vector Matrix::operator * (const Vector &v2) {

	float m00 = m[0][0], m01 = m[1][0], m02 = m[2][0], m03 = m[3][0];
	float m10 = m[0][1], m11 = m[1][1], m12 = m[2][1], m13 = m[3][1];
	float m20 = m[0][2], m21 = m[1][2], m22 = m[2][2], m23 = m[3][2];
	float m30 = m[0][3], m31 = m[1][3], m32 = m[2][3], m33 = m[3][3];

	Vector v;

	v.x = m00*v2.x + m01*v2.y + m02*v2.z;
	v.y = m10*v2.x + m11*v2.y + m12*v2.z;
	v.z = m20*v2.x + m21*v2.y + m22*v2.z;
	

	return v;
}