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
	MatrixTranslation(matrixT, 1, 0);
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

	static bool isObjectGenerated = false;
	if (isObjectGenerated == false)
	{
		Object* obj = new Object();
		obj->transform = new Transform();
		obj->vertexes = std::vector<vertex>(V, V + 6);
		obj->Renderer = new SpriteRenderer();
		((SpriteRenderer*)obj->Renderer)->currentSprite = ResourceManager::GetInstance().GetSprite("img.bmp");

		World::GetInstance().AddObject(obj);
		isObjectGenerated = true;
	}

	//World::GetInstance().GetRegistedObject(0)->transform->objectMatrix


	World::GetInstance().RenderAll();
}
