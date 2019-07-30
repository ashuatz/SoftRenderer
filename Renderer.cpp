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
void PutPixel(int x, int y);
void PutPixel(int x, int y, vector3 color);

void PutLine(const vector2& start, const vector2& end);
void PutLine(const vector2& start, const vector2& dir, float length);

void RenderRandomTriangle();
void CheckTriangle(const vertex& v1, const vertex& v2, const vertex& v3, bool isInner);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}


void DrawTriangle(const vertex& v1, const vertex& v2, const vertex& v3)
{
	vector2 rdir = v2.pos - v1.pos;
	vector2 mdir = v3.pos - v1.pos;

	float rLen = rdir.GetMagnitude();
	float mLen = mdir.GetMagnitude();
	rdir.Normalize();
	mdir.Normalize();

	for (int m = 0; m <= mLen; ++m)
	{
		vector2 op = (v1.pos + (mdir * m));
		PutLine(op, rdir, (1 - m / mLen) * rLen);
	}
}

void DrawTrianglePixel(const vertex& v1, const vertex& v2, const vertex& v3)
{
	vector2 rdir = v2.pos - v1.pos;
	vector2 mdir = v3.pos - v1.pos;

	float rLen = rdir.GetMagnitude();
	float mLen = (mdir.GetPixelAxisMagnitude());
	rdir.Normalize();// *= 1 / rLen;
	mdir.PixelNormalize();

	for (int m = 0; m <= mLen; ++m)
	{
		vector2 start = (v1.pos + (mdir * m));
		int length = ceil((1 - m / mLen) * rLen);
		for (int i = 0; i < length; ++i)
		{
			PutPixel(round(start.x + i * rdir.x), floor(start.y + i * rdir.y));
		}
		PutPixel(floor(start.x + length * (rdir.x)), floor(start.y + length * (rdir.y)));
	}
}

void PutLine(const vector2& start, const vector2& dir, float length)
{
	for (int i = 0; i <= length; ++i)
	{
		PutPixel(round(start.x + i * dir.x),floor(start.y + i * dir.y));
	}
}

void PutLine(const vector2& start, const vector2& end)
{
	vector2 dir = (end)-start;
	int magnitude = (dir.GetMagnitude());
	dir.Normalize();

	PutLine(start, dir, magnitude);
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

		std::uniform_int<> range_x(-g_nClientWidth / 2 + 1, g_nClientWidth / 2 - 1);
		std::uniform_int<> range_y(-g_nClientHeight / 2 + 1, g_nClientHeight / 2 - 1);
		V1 = vertex(vector2(range_x(random), range_y(random)), vector3(1, 0, 0)); //Blue
		V2 = vertex(vector2(range_x(random), range_y(random)), vector3(0, 1, 0));	//Gre
		V3 = vertex(vector2(range_x(random), range_y(random)), vector3(0, 0, 1)); // RED

		std::thread([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			isWaiting = false;
		}).detach();
	}

	DrawTrianglePixel(V1, V2, V3);

	PutPixel(V1.pos.x, V1.pos.y, V1.color * 255);
	PutPixel(V2.pos.x, V2.pos.y, V2.color * 255);
	PutPixel(V3.pos.x, V3.pos.y, V3.color * 255);
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