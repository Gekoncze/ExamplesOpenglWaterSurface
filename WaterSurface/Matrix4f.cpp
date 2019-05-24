#include "Matrix4f.h"

#include <math.h>

Matrix4f::Matrix4f(
	float f0, float f1, float f2, float f3,
	float f4, float f5, float f6, float f7,
	float f8, float f9, float f10, float f11,
	float f12, float f13, float f14, float f15
)
{
	data[0] = f0;
	data[1] = f1;
	data[2] = f2;
	data[3] = f3;
	data[4] = f4;
	data[5] = f5;
	data[6] = f6;
	data[7] = f7;
	data[8] = f8;
	data[9] = f9;
	data[10] = f10;
	data[11] = f11;
	data[12] = f12;
	data[13] = f13;
	data[14] = f14;
	data[15] = f15;
}

Matrix4f::Matrix4f()
{
}

Matrix4f::~Matrix4f()
{
}

float* Matrix4f::getData()
{
	return this->data;
}

Matrix4f Matrix4f::identity()
{
	return Matrix4f(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::translation(float x, float y, float z)
{
	return Matrix4f(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	);
}

Matrix4f Matrix4f::scale(float x, float y, float z)
{
	return Matrix4f(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotation(float angle, float x, float y, float z)
{
	float s = (float)sin(angle);
	float c = (float)cos(angle);
	float d = 1 - c;

	return Matrix4f(
		x* x* d + c, x* y* d + z * s, x* z* d - y * s, 0,
		x* y* d - z * s, y* y* d + c, y* z* d + x * s, 0,
		x* z* d + y * s, y* z* d - x * s, z* z* d + c, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotationX(float angle)
{
	float s = (float)sin(angle);
	float c = (float)cos(angle);

	return Matrix4f(
		1, 0, 0, 0,
		0, c, s, 0,
		0, -s, c, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotationY(float angle)
{
	float s = (float)sin(angle);
	float c = (float)cos(angle);

	return Matrix4f(
		c, 0, -s, 0,
		0, 1, 0, 0,
		s, 0, c, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::rotationZ(float angle)
{
	float s = (float)sin(angle);
	float c = (float)cos(angle);

	return Matrix4f(
		c, s, 0, 0,
		-s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

Matrix4f Matrix4f::ortho(float left, float right, float bottom, float top, float near, float far)
{
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(far + near) / (far - near);

	return Matrix4f(
		2.0f / (right - left), 0, 0, 0,
		0, 2.0f / (top - bottom), 0, 0,
		0, 0, -2.0f / (far - near), 0,
		tx, ty, tz, 1
	);
}

Matrix4f Matrix4f::frustum(float left, float right, float bottom, float top, float near, float far)
{
	float a, b, c, d;
	a = (right + left) / (right - left);
	b = (top + bottom) / (top - bottom);
	c = -(far + near) / (far - near);
	d = -(2 * far * near) / (far - near);

	return Matrix4f(
		2 * near / (right - left), 0, 0, 0,
		0, 2 * near / (top - bottom), 0, 0,
		a, b, c, -1,
		0, 0, d, 1
	);
}

Matrix4f Matrix4f::perspective(float angle, float aspect, float near, float far)
{
	float f = (float)(1.0f / tan(angle / 2.0f));
	float nf = near - far;

	return Matrix4f(
		f / aspect, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (far + near) / nf, -1,
		0, 0, 2 * far * near / nf, 1
	);
}

Matrix4f Matrix4f::yawPitchRoll(float yaw, float pitch, float roll)
{
	float cy = (float)cos(yaw);
	float sy = (float)sin(yaw);

	Matrix4f myaw = Matrix4f(
		cy, sy, 0, 0,
		-sy, cy, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	float cp = (float)cos(pitch);
	float sp = (float)sin(pitch);

	Matrix4f mpitch = Matrix4f(
		1, 0, 0, 0,
		0, cp, sp, 0,
		0, -sp, cp, 0,
		0, 0, 0, 1
	);

	float cr = (float)cos(roll);
	float sr = (float)sin(roll);

	Matrix4f mroll = Matrix4f(
		cr, sr, 0, 0,
		-sr, cr, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	return multiply(mroll, mpitch, myaw);
}

Matrix4f Matrix4f::multiply(Matrix4f left, Matrix4f right) {
	Matrix4f result;

	float* r = result.data;
	float* a = left.data;
	float* b = right.getData();

	r[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
	r[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
	r[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
	r[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];

	r[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
	r[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
	r[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
	r[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];

	r[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
	r[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
	r[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
	r[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];

	r[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	r[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	r[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
	r[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

	return result;
}

Matrix4f Matrix4f::multiply(Matrix4f left, Matrix4f middle, Matrix4f right) {
	return multiply(multiply(left, middle), right);
}
