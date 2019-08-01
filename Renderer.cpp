#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "vector.h"
#include "vertex.h"
#include "customMath.h"
#include "struct.h"

#include <random>
#include <ctime>
#include <functional>
#include <chrono>
#include <future>

void RenderRandomTriangle();

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
		V1 = vertex(vector2(range_x(random), range_y(random)), vector3(1, 0, 0) * 255); //Blue
		V2 = vertex(vector2(range_x(random), range_y(random)), vector3(0, 1, 0) * 255);	//Gre
		V3 = vertex(vector2(range_x(random), range_y(random)), vector3(0, 0, 1) * 255); // RED

		std::thread([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
			isWaiting = false;
		}).detach();
	}

	Triangle tri(V1, V2, V3);
	tri.RenderTriangle();
}
