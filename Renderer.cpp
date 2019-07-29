#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "vector.h"
#include "vertex.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);
void PutPixel(int x, int y, Vector3 color);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void DrawTriangle(const vertex& v1, const vertex& v2, const vertex& v3)
{
	float Xstart = min(min(v1.pos.x, v2.pos.x), v3.pos.x);
	float Ystart = min(min(v1.pos.y, v2.pos.y), v3.pos.y);
	float Xend = max(max(v1.pos.x, v2.pos.x), v3.pos.x);
	float Yend = max(max(v1.pos.y, v2.pos.y), v3.pos.y);

	Vector2 start(Xstart, Ystart);
	Vector2 end(Xend, Yend);

	for (int y = 0; y < end.y - start.y; ++y)
	{
		for (int x = 0; x < end.x - start.x; ++x)
		{
			vector2 p = (start + Vector2(x, y));

			if (vector2::Cross((v1.pos - v2.pos), p - v2.pos) >= 0 && vector2::Cross((v2.pos - v3.pos), p - v3.pos) >= 0 && vector2::Cross((v3.pos - v1.pos), p - v1.pos) >= 0)
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

void PutLine(const vector2& start, const vector2& dir, float length)
{
	for (int i = 0; i < length; ++i)
	{
		PutPixel(start.x + i * dir.x, start.y + i * dir.y);
	}
}

void PutLine(const vector2& start, const vector2& end)
{
	vector2 dir = end- start;
	int magnitude = dir.GetMagnitude();
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

void PutPixel(int x, int y, Vector3 color)
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
	SetColor(255, 0, 0);
	//PutPixel(1,1);

	Vertex V1(vector2(-100, -100), vector3(1, 0, 0));
	Vertex V2(vector2(-70, 80), vector3(0, 1, 0));
	Vertex V3(vector2(100, 100), vector3(0, 0, 1));
	Vertex V4(vector2(70, -80), vector3(0, 1, 0));

	DrawTriangle(V1, V2, V3);
	//DrawTriangle(V1, V3, V4);

	// Buffer Swap 
	BufferSwap();
}