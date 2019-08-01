#pragma once

struct vector2
{
public:
	float x;
	float y;


	vector2() : x(0), y(0) {}
	vector2(float x, float y) : x(x), y(y) {}
	vector2(const vector2& rhs) : x(rhs.x), y(rhs.y) {}

	vector2* Normalize()
	{
		float w = sqrt(this->x * this->x + this->y * this->y);
		this->x /= w;
		this->y /= w;
		return this;
	}

	vector2 Normalized()
	{
		vector2 temp{ this->x,this->y };
		return *temp.Normalize();
	}

	float GetMagnitude() const
	{
		float value = sqrt(this->x * this->x + this->y * this->y);
		return value;
	}

	vector2& operator-=(const vector2& rhv)
	{
		this->x -= rhv.x;
		this->y -= rhv.y;

		return *this;
	}

	const vector2 operator-(const vector2& rhv) const
	{
		vector2 temp{ this->x,this->y };
		temp -= rhv;
		return temp;
	}

	vector2& operator+=(const vector2& rhv)
	{
		this->x += rhv.x;
		this->y += rhv.y;

		return *this;
	}

	const vector2 operator+(const vector2& rhv) const
	{
		vector2 temp{ this->x,this->y };
		temp += rhv;
		return temp;
	}

	vector2& operator*=(const float& value)
	{
		this->x *= value;
		this->y *= value;

		return *this;
	}

	vector2& operator*=(const vector2& rhv)
	{
		this->x *= rhv.x;
		this->y *= rhv.y;

		return *this;
	}

	const vector2 operator*(const float& rhv) const
	{
		vector2 temp{ this->x,this->y };
		temp *= rhv;
		return temp;
	}

	const vector2 operator*(const vector2& rhv) const
	{
		vector2 temp{ this->x,this->y };
		temp *= rhv;
		return temp;
	}

	static float Dot(const vector2& lhv, const vector2& rhv)
	{
		return lhv.x * rhv.x + lhv.y * rhv.y;
	}

	static float Cross(const vector2& lhv, const vector2& rhv)
	{
		return lhv.x * rhv.y - lhv.y * rhv.x;
	}

	static const float Abs(const float& value)
	{
		return value > 0 ? value : -value;
	}

	static const float Max(const float& a, const float& b)
	{
		return a > b ? a : b;
	}

	virtual ~vector2() 
	{

	}
};

struct vector2Int : public vector2
{
	int x;
	int y;

	vector2Int(int x,int y) : x(x), y(y) {}
	vector2Int(const vector2& rhs) : x(rhs.x), y(rhs.y) {}
	vector2Int(const vector2Int& rhs) : x(rhs.x), y(rhs.y) {}

	virtual ~vector2Int() {}
};


struct vector3
{
public:
	float x;
	float y;
	float z;

	vector3(float x, float y,float z) : x(x), y(y),z(z) {}

	vector3* Normalize()
	{
		float w = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		this->x /= w;
		this->y /= w;
		this->z /= w;
		return this;
	}

	vector3 Normalized()
	{
		vector3 temp{ this->x,this->y,this->z };
		return *temp.Normalize();
	}

	vector3& operator+=(const vector3& rhv)
	{
		this->x += rhv.x;
		this->y += rhv.y;
		this->z += rhv.z;

		return *this;
	}

	vector3& operator-=(const vector3& rhv)
	{
		this->x -= rhv.x;
		this->y -= rhv.y;
		this->z -= rhv.z;

		return *this;
	}

	vector3& operator*=(const float& value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;

		return *this;
	}

	vector3& operator*=(const vector3& rhv)
	{
		this->x *= rhv.x;
		this->y *= rhv.y;
		this->z *= rhv.z;

		return *this;
	}

	const vector3 operator+(const vector3& rhv) const
	{
		vector3 temp{ this->x,this->y,this->z };
		temp += rhv;
		return temp;
	}

	const vector3 operator-(const vector3& rhv) const
	{
		vector3 temp{ this->x,this->y,this->z };
		temp -= rhv;
		return temp;
	}

	const vector3 operator*(const float& value) const
	{
		vector3 temp{ this->x,this->y,this->z };
		temp *= value;
		return temp;
	}

	static COLORREF ToColor(const vector3& vector)
	{
		return ((COLORREF)(((BYTE)(vector.z) | ((WORD)((BYTE)(vector.y)) << 8)) | (((DWORD)(BYTE)(vector.x)) << 16)));
	}
};

