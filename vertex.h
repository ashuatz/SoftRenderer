#pragma once

#include "vector.h"

struct vertex 
{
	vector2 pos;
	vector3 color;

	vertex(vector2 pos, vector3 color) : pos(pos), color(color.Normalized()) {}
};