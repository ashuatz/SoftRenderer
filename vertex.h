#pragma once

#include "vector.h"

typedef struct vertex 
{
	Vector2 pos;
	Vector3 color;

	vertex(vector2 pos, vector3 color) : pos(pos), color(color.Normalized()) {}
}Vertex;