#pragma once

#include "vector.h"

struct vertex
{
	vector2 pos;
	vector3 color;
	vector2 uv;

	vertex(vector2 pos, vector3 color) : pos(pos), color(color.Normalized()) {}
	vertex(vector2 pos, vector3 color, vector2 uv) : pos(pos), color(color.Normalized()), uv(uv) {}
	vertex(const vertex& rhs) : color(rhs.color), pos(rhs.pos), uv(rhs.uv) {}
};