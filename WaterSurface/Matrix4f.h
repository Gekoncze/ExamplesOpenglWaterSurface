#pragma once
class Matrix4f
{
private:
	float data[16];

public:
	Matrix4f(
		float f0, float f1, float f2, float f3,
		float f4, float f5, float f6, float f7,
		float f8, float f9, float f10, float f11,
		float f12, float f13, float f14, float f15
	);
	Matrix4f();
	~Matrix4f();

	float* getData();

	static Matrix4f identity();
	static Matrix4f translation(float x, float y, float z);
	static Matrix4f scale(float x, float y, float z);
	static Matrix4f rotation(float angle, float x, float y, float z);
	static Matrix4f rotationX(float angle);
	static Matrix4f rotationY(float angle);
	static Matrix4f rotationZ(float angle);
	static Matrix4f ortho(float left, float right, float bottom, float top, float near, float far);
	static Matrix4f frustum(float left, float right, float bottom, float top, float near, float far);
	static Matrix4f perspective(float angle, float aspect, float near, float far);
	static Matrix4f yawPitchRoll(float yaw, float pitch, float roll);
	static Matrix4f multiply(Matrix4f left, Matrix4f right);
	static Matrix4f multiply(Matrix4f left, Matrix4f middle, Matrix4f right);
};

inline Matrix4f operator*(Matrix4f left, Matrix4f right)
{
    return Matrix4f::multiply(left, right);
}