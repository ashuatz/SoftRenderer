#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"
#include "vector.h"
#include "vertex.h"
#include "customMath.h"
#include "struct.h"
#include "Bmp.h"
#include "Time.h"

#include "BaseRenderer.h"

void RenderRandomTriangle();

void UpdateFrame(void)
{
	//test
	//Object::LoadResources();

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

	static vector2 pos;
	static float rotation;
	static vector2 scale(1, 1);
	static Matrix3x3 matrixT, matrixR, matrixS;
	static Matrix3x3 matrix;

	rotation = 0.0321;

	MatrixIdentity(matrix);
	MatrixTranslation(matrixT, 0, 0);
	MatrixRotationDir(matrixR, rotation);
	MatrixScale(matrixS, scale.x, scale.y);

	matrix = matrixS * matrixR * matrixT;

	static vertex V[6]{
		{vector2(50, 50), vector3(0, 1, 0),vector2(1,0)}, //2
		{vector2(-50, -50), vector3(0, 0, 1),vector2(0,1)}, //4
		{vector2(-50, 50), vector3(1, 0, 0),vector2(0,0)}, //1

		{vector2(-50, -50), vector3(0, 0, 1),vector2(0,1)}, //4
		{vector2(50, 50), vector3(0, 1, 0),vector2(1,0)}, //2
		{vector2(50, -50), vector3(1, 1, 1),vector2(1,1)}, //3
	};

	for each (auto& var in V)
	{
		var.pos = var.pos * matrix;
	}

	//if (!isWaiting)
	//{
	//	isWaiting = true;
	//	std::mt19937 random((unsigned int)time(0));

	//	std::uniform_int<> range_x(-200, 200);
	//	std::uniform_int<> range_y(-200, 200);
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		V[i] = vertex(vector2(range_x(random), range_y(random)), vector3((int)i == 0, (int)i == 1, (int)i == 2) * 255);
	//	}
	//	std::sort(V, V + 3, [&](const vertex& a, const vertex& b) {return a.pos.y > b.pos.y; });
	//	vector2 temp = V[1].pos - V[0].pos;
	//	V[3] = vertex(V[2].pos + (V[0].pos - V[1].pos), vector3(1, 0, 1));

	//	std::thread([]() {
	//		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	//		isWaiting = false;
	//	}).detach();
	//}


	static Sprite spr("img.bmp");
	std::vector<vertex> vertexs(V, V + 6);
	static Renderer* renderer = new SpriteRenderer();
	((SpriteRenderer*)renderer)->currentSprite = &spr;

	renderer->Render(vertexs);
}
