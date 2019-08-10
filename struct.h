#pragma once
#include "vector.h"
#include "vertex.h"
#include "stdafx.h"

struct Triangle
{
protected:
	ULONG* bitmap;
	vector2 bitmapSize;
	vertex V1, V2, V3;
	bool isDenInitialized;
	float BarycentricDenominator;

public:
	Triangle(const vertex& p1, const vertex& p2, const vertex& p3);
	virtual void SetBitmap(ULONG* bmp, vector2 size) { bitmap = bmp; bitmapSize = size; }
	virtual void RenderTriangle();
	virtual void RenderTriangle(const vertex& p1, const vertex& p2, const vertex& p3);

protected:
	void DrawScanline(const vector2Int & start, const vector2Int & end);
	virtual	void DrawLine(const vector2Int& start, const vector2Int& end);
	virtual void DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3);
	virtual vector3 GetBarycentricCoordinate(const vector2Int& p);
	virtual vector3 GetColorByBarycentricCoordinate(const vector2Int& p);

	virtual void PutPixel(int x, int y, vector3 color);

public:
	const vertex& operator[](const int& index) const;
	virtual ~Triangle() {}
};

struct Quad : protected Triangle
{
private:
	vertex V4;
	Triangle T1,T2;

	bool isUVDenInitialized;
	float UVBarycentricDenominator;

public:
	Quad(const vertex& p1, const vertex& p2, const vertex& p3, const vertex& p4);
	virtual void SetBitmap(ULONG* bmp, vector2 size) override;
	void RenderQuad();

protected:
	//const vector2 GetUVCoordinates(const vector2Int& p);
	//virtual vector3 GetColorByBarycentricCoordinate(const vector2Int& p) override;

public:
	vertex* operator[](const int& index) ;
	virtual ~Quad() {
	}
};