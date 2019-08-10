#include "stdafx.h"
#include "Matrix.h"

Matrix2x2 &MatrixIdentity(Matrix2x2 &matrix)
{
	matrix.m[0][1] = matrix.m[1][0] = 0.0f;

	matrix.m[0][0] = matrix.m[1][1] = 1.0f;

	return matrix;
}

Matrix3x3 &MatrixIdentity(Matrix3x3 &matrix)
{
	matrix.m[0][1] = matrix.m[0][2] =
		matrix.m[1][0] = matrix.m[1][2] =
			matrix.m[2][0] = matrix.m[2][1] = 0.0f;

	matrix.m[0][0] = matrix.m[1][1] = matrix.m[2][2] = 1.0f;

	return matrix;
}

Matrix4x4 &MatrixIdentity(Matrix4x4 &matrix)
{
	matrix.m[0][1] = matrix.m[0][2] = matrix.m[0][3] =
		matrix.m[1][0] = matrix.m[1][2] = matrix.m[1][3] =
			matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][3] =
				matrix.m[3][0] = matrix.m[3][1] = matrix.m[3][2] = 0.0f;

	matrix.m[0][0] = matrix.m[1][1] = matrix.m[2][2] = matrix.m[3][3] = 1.0f;

	return matrix;
}

Matrix3x3 &MatrixTranslation(Matrix3x3 &matrix, const float &x, const float &y)
{
	MatrixIdentity(matrix);

	matrix.m[2][0] = x;
	matrix.m[2][1] = y;

	return matrix;
}

Matrix4x4 &MatrixTranslation(Matrix4x4 &matrix, const float &x, const float &y, const float &z)
{
	MatrixIdentity(matrix);

	matrix.m[3][0] = x;
	matrix.m[3][1] = y;
	matrix.m[3][2] = z;

	return matrix;
}

Matrix3x3 &MatrixScale(Matrix3x3 &matrix, const float &x, const float &y)
{
	MatrixIdentity(matrix);

	matrix.m[0][0] = x;
	matrix.m[1][1] = y;

	return matrix;
}

Matrix4x4 &MatrixScale(Matrix4x4 &matrix, const float &x, const float &y, const float &z)
{
	MatrixIdentity(matrix);

	matrix.m[0][0] = x;
	matrix.m[1][1] = y;
	matrix.m[2][2] = z;

	return matrix;
}

Matrix3x3 &MatrixRotationDir(Matrix3x3 &matrix, const float &dir)
{
	MatrixIdentity(matrix);

	matrix.m[0][0] = cosf(dir);
	matrix.m[0][1] = sinf(dir);
	matrix.m[1][0] = -sinf(dir);
	matrix.m[1][1] = cosf(dir);

	return matrix;
}

Matrix4x4 &MatrixRotationYawPitchRoll(Matrix4x4 &matrix, const float &yaw, const float &pitch, const float roll)
{
	return matrix;
}
