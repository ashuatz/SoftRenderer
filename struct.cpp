#include "stdafx.h"
#include "struct.h"
#include "customMath.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include <algorithm>
#include <cstdlib>



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
		DrawLine(vt1, vt2);

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
	}
}

vector3 Triangle::GetBarycentricCoordinate(const vector2Int& p)
{
	vector2 a(m_v1.pos);
	vector2 b(m_v2.pos);
	vector2 c(m_v3.pos);
	vector3 l(0, 0, 0);

	if (!isDenInitialized)
	{
		m_Den = 1.f / ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
		isDenInitialized = true;
	}

	l.x = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) * m_Den;
	l.y = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) * m_Den;
	l.z = 1 - l.x - l.y;

	return l;
}

vector3 Triangle::GetColorByBarycentricCoordinate(const vector2Int& p)
{
	vector3 lambda = GetBarycentricCoordinate(p);
	vector3 color(0, 0, 0);
	color += m_v1.color * lambda.x;
	color += m_v2.color * lambda.y;
	color += m_v3.color * lambda.z;
	return color * 255;
}


void Triangle::RenderTriangle()
{
	//sort by y axis
	vertex vts[3]{ m_v1,m_v2,m_v3 };
	std::sort(vts, vts + 3, [&](const vertex& a, const vertex& b) {return a.pos.y > b.pos.y; });

	m_v1 = vts[0];
	m_v2 = vts[1];
	m_v3 = vts[2];

	if (m_v1.pos.y == m_v3.pos.y)
	{
		DrawFlatSideTriangle(m_v1, m_v2, m_v3);
	}
	else if (m_v1.pos.y == m_v2.pos.y)
	{
		DrawFlatSideTriangle(m_v3, m_v1, m_v2);
	}
	else
	{
		float x = m_v1.pos.x - (m_v1.pos.y - m_v2.pos.y) * (m_v1.pos.x - m_v3.pos.x) / (m_v1.pos.y - m_v3.pos.y);
		vertex v4(vector2(round(x), m_v2.pos.y), vector3(1, 1, 1));
		DrawFlatSideTriangle(m_v1, m_v2, v4); // up-side
		DrawFlatSideTriangle(m_v3, m_v2, v4); // down-side

		PutPixel(v4.pos.x, v4.pos.y, v4.color * 0);
	}

	PutPixel(m_v1.pos.x, m_v1.pos.y, m_v1.color * 0);
	PutPixel(m_v2.pos.x, m_v2.pos.y, m_v2.color * 0);
	PutPixel(m_v3.pos.x, m_v3.pos.y, m_v3.color * 0);
}


void Triangle::PutPixel(int x, int y, vector3 color)
{
	if (!IsInRange(x, y))
		return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = vector3::ToColor(color);
}