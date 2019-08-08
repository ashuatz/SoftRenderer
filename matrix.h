#pragma once

#include "vector.h"
#include <array>

struct Matrix4x4
{
private :
	std::array<float,16> data;

public :
	Matrix4x4();
	Matrix4x4 CreateScale(vector4 scale);
	Matrix4x4 CreateTranslation(vector4 translation);
	Matrix4x4 CreateRotationX(float angle);
	Matrix4x4 CreateRotationY(float angle);
	Matrix4x4 CreateRotationZ(float angle);
	Matrix4x4(vector4 column0, vector4 column1, vector4 column2, vector4 column3);

	float& operator()(const int& x, const int& y)
	{
		return data[y * 4 + x];
	}

	const float& operator()(const int& x, const int& y)const
	{
		return data[y * 4 + x];
	}

	inline Matrix4x4& operator+=(const Matrix4x4& rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			data[i] += rhs.data[i];
		}
		return *this;
	}

	inline Matrix4x4& operator-=(const Matrix4x4& rhs)
	{
		for (int i = 0; i < 16; i++)
		{
			data[i] -= rhs.data[i];
		}
		return *this;
	}

	inline Matrix4x4& operator*=(const Matrix4x4& rhs)
	{
		Matrix4x4 temp;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 0; k < 4; ++k)
				{
					temp(i,j) += (*this)(i,k) * rhs(k,j);
				}
			}
		}

		(*this) = temp;

		return *this;
	}

	inline Matrix4x4& operator*=(const float& rhs)
	{
		for (auto& datum : data)
		{
			datum *= rhs;
		}

		return *this;
	}

	//inline Matrix4x4& operator=(const Matrix4x4& rhs) 
	//{
	//	std::copy(rhs.data.begin(), rhs.data.end(), data);
	//}
};

extern inline Matrix4x4 operator+(const Matrix4x4& lhs, const Matrix4x4& rhs);
extern inline Matrix4x4 operator-(const Matrix4x4& lhs, const Matrix4x4& rhs);
extern inline Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs);
extern inline Matrix4x4 operator*(const Matrix4x4& lhs, const float& rhs);