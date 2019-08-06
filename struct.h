#pragma once
#include "vector.h"
#include "vertex.h"
#include "stdafx.h"

struct Triangle
{
private:
	vertex V1, V2, V3;
	bool isDenInitialized;
	float BarycentricDenominator;
public:
	Triangle(const vertex& p1, const vertex& p2, const vertex& p3) :V1(p1), V2(p2), V3(p3), isDenInitialized(false) {}
	void RenderTriangle();

private:

	void DrawLine(const vector2Int& start, const vector2Int& end);
	void DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3);
	vector3 GetBarycentricCoordinate(const vector2Int& p);
	vector3 GetColorByBarycentricCoordinate(const vector2Int& p);

	void PutPixel(int x, int y, vector3 color);
};

struct Quad
{
private:
	vertex m_v1, m_v2, m_v3, m_v4;

public:
	Quad(const vertex& p1, const vertex& p2, const vertex& p3, const vertex& p4) :m_v1(p1), m_v2(p2), m_v3(p3),m_v4(p4) {}
	void RenderQuad();

private:

	void DrawLine(const vector2Int& start, const vector2Int& end);
	void DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3);
	vector3 GetBarycentricCoordinate(const vector2& point);
	vector3 GetColorByBarycentricCoordinate(const vector2& p);

	void PutPixel(int x, int y, vector3 color);

};