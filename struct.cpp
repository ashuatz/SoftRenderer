#include "stdafx.h"
#include "struct.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Bmp.h"
#include "customMath.h"

#pragma region Triangle
void Triangle::DrawScanline(const vector2Int& start, const vector2Int& end)
{
	int dx = abs(end.x - start.x);
	const int signx = sign(end.x - start.x);

	vector2Int current = start;
	for (int i = 0; i < dx; ++i)
	{
		PutPixel(current.x, current.y, GetColorByBarycentricCoordinate(current));
		current.x += signx;
	}
}

void Triangle::DrawLine(const vector2Int& start, const vector2Int& end)
{
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);

	const int signx = sign(end.x - start.x);
	const int signy = sign(end.y - start.y);

	const bool isChanged = dy > dx;

	if (isChanged)
	{
		customSwap(dx, dy);
	}

	vector2Int current = start;

	int e = 2 * dy - dx;
	for (int i = 0; i <= dx; ++i)
	{
		PutPixel(current.x, current.y, GetColorByBarycentricCoordinate(current));
		if (e >= 0)
		{
			if (isChanged)
				current.x += signx;
			else
				current.y += signy;

			e -= 2 * dx;
		}

		if (isChanged)
			current.y += signy;
		else
			current.x += signx;

		e += 2 * dy;
	}
}

void Triangle::DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3)
{
	vector2 vt1 = v1.pos;
	vector2 vt2 = v1.pos;

	int dx1 = abs(v2.pos.x - v1.pos.x);
	int dy1 = abs(v2.pos.y - v1.pos.y);

	int dx2 = abs(v3.pos.x - v1.pos.x);
	int dy2 = abs(v3.pos.y - v1.pos.y);

	const int signx1 = sign(v2.pos.x - v1.pos.x);
	const int signx2 = sign(v3.pos.x - v1.pos.x);

	const int signy1 = sign(v2.pos.y - v1.pos.y);
	const int signy2 = sign(v3.pos.y - v1.pos.y);

	const bool isChanged1 = dy1 > dx1;
	const bool isChanged2 = dy2 > dx2;

	if (isChanged1)
	{
		customSwap(dx1, dy1);
	}

	if (isChanged2)
	{
		customSwap(dx2, dy2);
	}

	int e1 = 2 * dy1 - dx1;
	int e2 = 2 * dy2 - dx2;

	for (int i = 0; i <= dx1; ++i)
	{
		//edge 1
		while (e1 >= 0)
		{
			if (isChanged1)
				vt1.x += signx1;
			else
				vt1.y += signy1;
			e1 -= 2 * dx1;
		}

		if (isChanged1)
			vt1.y += signy1;
		else
			vt1.x += signx1;

		e1 += 2 * dy1;

		//edge 2
		while (vt2.y != vt1.y)
		{
			while (e2 >= 0)
			{
				if (isChanged2)
					vt2.x += signx2;
				else
					vt2.y += signy2;
				e2 -= 2 * dx2;
			}

			if (isChanged2)
				vt2.y += signy2;
			else
				vt2.x += signx2;

			e2 += 2 * dy2;
		}

		DrawScanline(vt1, vt2);
	}
}

//use 2D only
//TODO: if 3D Coordinates, use 'dot' operator instead.
vector3 Triangle::GetBarycentricCoordinate(const vector2Int& p)
{
	if (!isDenInitialized)
	{
		BarycentricDenominator = 1.f / ((V2.pos.y - V3.pos.y) * (V1.pos.x - V3.pos.x) + (V3.pos.x - V2.pos.x) * (V1.pos.y - V3.pos.y));
		isDenInitialized = true;
	}
	vector3 l(0, 0, 0);

	l.x = ((V2.pos.y - V3.pos.y) * (p.x - V3.pos.x) + (V3.pos.x - V2.pos.x) * (p.y - V3.pos.y)) * BarycentricDenominator;
		l.x = clamp(0.f, 1.f, (V3.uv.x - V1.uv.x)  < 0 ? l.x : 1 - l.x);
	l.y = ((V3.pos.y - V1.pos.y) * (p.x - V3.pos.x) + (V1.pos.x - V3.pos.x) * (p.y - V3.pos.y)) * BarycentricDenominator;
		l.y = clamp(0.f, 1.f, (V2.uv.y - V1.uv.y) < 0 ? 1 - l.y : l.y);
	l.z = clamp(0.f, 1.f, 1 - l.x - l.y);

	return l;
}

vector3 Triangle::GetColorByBarycentricCoordinate(const vector2Int& p)
{
	vector3 lambda = GetBarycentricCoordinate(p);
	if (bitmap != nullptr)
	{
		return vector3::ToVector3(GetPixel(lambda.x * bitmapSize.x - 0.5f, bitmapSize.x, lambda.y * bitmapSize.y - 0.5f, bitmap));
	}

	else
	{
		vector3 color(0, 0, 0);
		color += V1.color * lambda.x;// u
		color += V2.color * lambda.y;// v 
		color += V3.color * lambda.z; 
		return color * 255;
	}
}

Triangle::Triangle(const vertex & p1, const vertex & p2, const vertex & p3) :V1(p1), V2(p2), V3(p3), bitmap(nullptr), isDenInitialized(false)
{

}

void Triangle::RenderTriangle()
{
	RenderTriangle(V1, V2, V3);
}

void Triangle::RenderTriangle(const vertex & _v1, const vertex & _v2, const vertex & _v3)
{	//sort by y axis

	vertex vts[3]{ _v1,_v2,_v3 };
	std::sort(vts, vts + 3, [&](const vertex& a, const vertex& b) {return a.pos.y > b.pos.y; });

	auto& v1 = vts[0];
	auto& v2 = vts[1];
	auto& v3 = vts[2];

	if (v1.pos.y == v3.pos.y)
	{
		DrawFlatSideTriangle(v1, v2, v3);
	}
	else if (v1.pos.y == v2.pos.y)
	{
		DrawFlatSideTriangle(v3, v1, v2);
	}
	else
	{
		float x = v1.pos.x - (v1.pos.y - v2.pos.y) * (v1.pos.x - v3.pos.x) / (v1.pos.y - v3.pos.y);
		vertex v4(vector2(round(x), v2.pos.y), vector3(1, 1, 1));
		DrawFlatSideTriangle(v1, v2, v4); // up-side
		DrawFlatSideTriangle(v3, v2, v4); // down-side
	}
}


void Triangle::PutPixel(int x, int y, vector3 color)
{
	if (!IsInRange(x, y))
		return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = vector3::ToColor(color);
}

const vertex& Triangle::operator[](const int& index) const
{
	switch (index)
	{
		case 0: return V1;
		case 1: return V2;
		case 2: return V3;
		default:throw std::invalid_argument("index out of range");
	}
}

#pragma endregion

#pragma region Quad

Quad::Quad(const vertex & p1, const vertex & p2, const vertex & p3, const vertex & p4)
	: Triangle(p1, p2, p3), V4(p4), T1(p1, p2, p3), T2(p1, p2, p3), isUVDenInitialized(false)
{
	T1 = Triangle(V2, V4, V1);
	T2 = Triangle(V4, V2, V3);
}

void Quad::SetBitmap(ULONG * bmp, vector2 size)
{
	bitmap = bmp; 
	bitmapSize = size;
	T1.SetBitmap(bitmap, bitmapSize);
	T2.SetBitmap(bitmap, bitmapSize);
}

void Quad::RenderQuad()
{
	T1.RenderTriangle();
	T2.RenderTriangle();
}

vertex* Quad::operator[](const int& index)
{
	switch (index)
	{
		case 0: return &V1;
		case 1: return &V2;
		case 2: return &V3;
		case 3: return &V4;
		default:throw std::invalid_argument("index out of range");
	}
}

#pragma endregion