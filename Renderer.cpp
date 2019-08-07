#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "vector.h"
#include "vertex.h"
#include "customMath.h"
#include "struct.h"
#include "Bmp.h"

#include <random>
#include <ctime>
#include <functional>
#include <chrono>
#include <future>
#include <Windows.h>
#include <stdexcept>
#include "performanceCounter.h"
#include <sstream>

void RenderRandomTriangle();

void UpdateFrame(void)
{
	//check counter
	double counter = PerformanceCounter::GetInstance().GetCounter();
	auto titleName = (std::wstringstream() << counter).str().append(_T("ms"));
	//Settext
	SetWindowText(hWnd, titleName.c_str());
	PerformanceCounter::GetInstance().StartCounter();

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

	static vertex V[4]{ {vector2(-50, 50), vector3(1, 0, 0)},{vector2(50, 50), vector3(0, 1, 0)},{vector2(-50, -50), vector3(0, 0, 1)},{vector2(50, -50), vector3(1, 1, 1)} };

	if (!isWaiting)
	{
		isWaiting = true;
		std::mt19937 random((unsigned int)time(0));

		std::uniform_int<> range_x(-200, 200);
		std::uniform_int<> range_y(-200, 200);
		for (int i = 0; i < 3; ++i)
		{
			V[i] = vertex(vector2(range_x(random), range_y(random)), vector3((int)i == 0, (int)i == 1, (int)i == 2) * 255);
		}
		std::sort(V, V + 3, [&](const vertex& a, const vertex& b) {return a.pos.y > b.pos.y; });
		vector2 temp = V[1].pos - V[0].pos;
		V[3] = vertex(V[2].pos + (V[0].pos - V[1].pos), vector3(1, 0, 1));

		std::thread([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			isWaiting = false;
		}).detach();
	}

	Triangle tri(V[0], V[1], V[2]);
	tri.RenderTriangle();

	//getBitmap
	int width, height;
	auto bmp = OpenBMP("img.bmp", &width, &height);

	Quad quad(V[0], V[1], V[3], V[2]);
	quad.SetBitmap(bmp, vector2(width, height));
	quad.RenderQuad();
}
