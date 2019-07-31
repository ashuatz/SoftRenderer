#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "vector.h"
#include "vertex.h"
#include <random>
#include <ctime>
#include <functional>
#include <chrono>
#include <future>

bool IsInRange(int x, int y);

void PutPixel(const vector2& point);
void PutPixel(int x, int y);
void PutPixel(int x, int y, vector3 color);

void PutLine(const vector2& start, const vector2& end);
void PutLine(const vector2& start, const vector2& dir, float length);

void RenderRandomTriangle();

void DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3);
void DrawTrianglePixel(const vertex& _v1, const vertex& _v2, const vertex& _v3);

void CheckTriangle(const vertex& v1, const vertex& v2, const vertex& v3, bool isInner);

template <typename T> int sign(const T& val) {
	return (T(0) < val) - (val < T(0));
}

template<typename T>
void customSwap(T& a, T& b)
{
	T Temp = a;
	a = b;
	b = Temp;
}

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void DrawTrianglePixel(const vertex& _v1, const vertex& _v2, const vertex& _v3)
{
	//sort by y axis
	vertex vts[3]{ _v1,_v2,_v3 };
	std::sort(vts, vts + 3, [](const vertex& a, const vertex& b) {return a.pos.y > b.pos.y; });
	vertex v1(vts[0]);
	vertex v2(vts[1]);
	vertex v3(vts[2]);

	if (v2.pos.y == v3.pos.y)
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

void DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3)
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
		PutLine(vt1, vt2);

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

void PutLine(const vector2& start, const vector2& dir, float length)
{
	PutLine(start, start + dir * length);
}

void PutLine(const vector2& start, const vector2& end)
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

	vector2 current = start;

	int e = 2 * dy - dx;
	for (int i = 0; i <= dx; ++i)
	{
		PutPixel(current);
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

void PutPixel(const vector2& point)
{
	PutPixel(point.x, point.y);
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y))
		return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void PutPixel(int x, int y, vector3 color)
{
	if (!IsInRange(x, y))
		return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = RGB(color.x, color.y, color.z);
}

void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(0, 0, 0);
	RenderRandomTriangle();

	// Buffer Swap 
	BufferSwap();
}

void RenderRandomTriangle()
{
	static bool isWaiting = false;
	static vertex V1(vector2(0, 0), vector3(0, 0, 0));
	static vertex V2(vector2(0, 0), vector3(0, 0, 0));
	static vertex V3(vector2(0, 0), vector3(0, 0, 0));

	if (!isWaiting)
	{
		isWaiting = true;
		std::mt19937 random((unsigned int)time(0));

		std::uniform_int<> range_x(-200, 200);
		std::uniform_int<> range_y(-200, 200);
		V1 = vertex(vector2(range_x(random), range_y(random)), vector3(1, 0, 0)); //Blue
		V2 = vertex(vector2(range_x(random), range_y(random)), vector3(0, 1, 0));	//Gre
		V3 = vertex(vector2(range_x(random), range_y(random)), vector3(0, 0, 1)); // RED

		std::thread([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			isWaiting = false;
		}).detach();
	}

	DrawTrianglePixel(vertex(V1), vertex(V2), vertex(V3));
}

void CheckTriangle(const vertex& v1, const vertex& v2, const vertex& v3, bool isInner = false)
{
	float Xstart = min(min(v1.pos.x, v2.pos.x), v3.pos.x);
	float Ystart = min(min(v1.pos.y, v2.pos.y), v3.pos.y);
	float Xend = max(max(v1.pos.x, v2.pos.x), v3.pos.x);
	float Yend = max(max(v1.pos.y, v2.pos.y), v3.pos.y);

	vector2 start(Xstart, Ystart);
	vector2 end(Xend, Yend);

	for (int y = 0; y < end.y - start.y; ++y)
	{
		for (int x = 0; x < end.x - start.x; ++x)
		{
			vector2 p = (start + vector2(x, y));

			bool pass = false;
			if (isInner)
				pass = vector2::Cross((v1.pos - v2.pos), p - v2.pos) > 0 && vector2::Cross((v2.pos - v3.pos), p - v3.pos) > 0 && vector2::Cross((v3.pos - v1.pos), p - v1.pos) > 0;
			else
				pass = vector2::Cross((v1.pos - v2.pos), p - v2.pos) >= 0 && vector2::Cross((v2.pos - v3.pos), p - v3.pos) >= 0 && vector2::Cross((v3.pos - v1.pos), p - v1.pos) >= 0;

			if (pass)
			{
				//getColor;
				auto totalLength = (v1.pos - p).GetMagnitude() + (v2.pos - p).GetMagnitude() + (v3.pos - p).GetMagnitude();
				vector3 color(1, 1, 1);
				color *= vector3(1, 1, 1) - (v1.color * ((v1.pos - p).GetMagnitude() / totalLength));
				color *= vector3(1, 1, 1) - (v2.color*((v2.pos - p).GetMagnitude() / totalLength));
				color *= vector3(1, 1, 1) - (v3.color*((v3.pos - p).GetMagnitude() / totalLength));

				PutPixel(p.x, p.y, color * 255);
			}
		}
	}
}