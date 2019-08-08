#include "stdafx.h"
#include "matrix.h"
#include "vector.h"

#define isRightHandCoordination true
#define ToCoordination(angle) isRightHandCoordination ? -angle : angle

Matrix4x4::Matrix4x4()
{
	std::fill(data.begin(), data.end(), 0);
	//make to Identity
	data[0] = data[5] = data[10] = data[15] = 1;
}


Matrix4x4 Matrix4x4::CreateScale(vector4 scale)
{
	Matrix4x4 temp = Matrix4x4();
	temp.data[0] = scale.x;
	temp.data[5] = scale.y;
	temp.data[10] = scale.z;
	return temp;
}

Matrix4x4 Matrix4x4::CreateTranslation(vector4 translation)
{
	Matrix4x4 temp = Matrix4x4();
	temp.data[3] = translation.x;
	temp.data[7] = translation.y;
	temp.data[11] = translation.z;
	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationX(float angle)
{
	Matrix4x4 temp = Matrix4x4();
	temp.data[5] = std::cos(ToCoordination(angle));
	temp.data[6] = -std::sin(ToCoordination(angle));
	temp.data[9] = std::sin(ToCoordination(angle));
	temp.data[10] = std::cos(ToCoordination(angle));
	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationY(float angle)
{
	Matrix4x4 temp = Matrix4x4();
	temp.data[0] = std::cos(ToCoordination(angle));
	temp.data[2] = std::sin(ToCoordination(angle));
	temp.data[8] = -std::sin(ToCoordination(angle));
	temp.data[10] = std::cos(ToCoordination(angle));
	return temp;
}

Matrix4x4 Matrix4x4::CreateRotationZ(float angle)
{
	Matrix4x4 temp = Matrix4x4();
	temp.data[0] = std::cos(ToCoordination(angle));
	temp.data[1] = -std::sin(ToCoordination(angle));
	temp.data[4] = std::sin(ToCoordination(angle));
	temp.data[5] = std::cos(ToCoordination(angle));
	return temp;
}

Matrix4x4::Matrix4x4(vector4 column0, vector4 column1, vector4 column2, vector4 column3)
{
	for (int i = 0; i < 4; ++i)
	{
		data[0 + i] = column0[i];
		data[4 + i] = column1[i];
		data[8 + i] = column2[i];
		data[12 + i] = column3[i];
	}
}

inline Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 temp = Matrix4x4(lhs);
	temp += rhs;
	return temp;
}

inline Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 temp = Matrix4x4(lhs);
	temp -= rhs;
	return temp;
}

inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
{
	Matrix4x4 temp = Matrix4x4(lhs);
	temp *= rhs;
	return temp;
}

inline Matrix4x4 operator*(const Matrix4x4& lhs, const float& rhs)
{
	Matrix4x4 temp = Matrix4x4(lhs);
	temp *= rhs;
	return temp;
}