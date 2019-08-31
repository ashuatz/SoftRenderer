#include "stdafx.h"
#include "BaseRenderer.h"
#include "customMath.h"
#include "GDIHelper.h"

void Renderer::Render(std::vector<vertex>& vertexs)
{
	std::vector<vertex>::iterator it;
	it = vertexs.begin();

	while (it != vertexs.end())
	{
		std::vector<vertex> v;

		for (int i = 0; i < 3 && it != vertexs.end(); i++, it++)
			v.push_back((*it));

		currentTriangle = new TriangleData(v);

		std::sort(v.begin(), v.end(), [](const vertex& a, const vertex& b) {return a.pos.y > b.pos.y; });

		if (v[0].pos.y == v[2].pos.y)
		{
			DrawFlatSideTriangle(v[0], v[1], v[2]);
		}
		else if (v[0].pos.y == v[1].pos.y)
		{
			DrawFlatSideTriangle(v[2], v[0], v[1]);
		}
		else
		{
			float x = v[0].pos.x - (v[0].pos.y - v[1].pos.y) * (v[0].pos.x - v[2].pos.x) / (v[0].pos.y - v[2].pos.y);
			vertex v4(vector2(round(x), v[1].pos.y), vector3(1, 1, 1));

			DrawFlatSideTriangle(v[0], v[1], v4); //upside 
			DrawFlatSideTriangle(v[2], v[1], v4); //downside
		}

		delete currentTriangle;
	}
}

void Renderer::DrawFlatSideTriangle(const vertex & v1, const vertex & v2, const vertex & v3)
{
	vector2 sideA = v1.pos;
	vector2 sideB = v1.pos;

	vector2Int deltaA(abs(ceil(v2.pos.x - v1.pos.x)), abs(ceil(v2.pos.y - v1.pos.y)));
	vector2Int deltaB(abs(ceil(v3.pos.x - v1.pos.x)), abs(ceil(v3.pos.y - v1.pos.y)));

	const vector2Int signA(sign(v2.pos.x - v1.pos.x), sign(v2.pos.y - v1.pos.y));
	const vector2Int signB(sign(v3.pos.x - v1.pos.x), sign(v3.pos.y - v1.pos.y));

	//dy1 > dx1;
	const bool isChangedA = deltaA.y > deltaA.x;
	//dy2 > dx2;
	const bool isChangedB = deltaB.y > deltaB.x;

	if (isChangedA)
	{
		customSwap(deltaA.x, deltaA.y);
	}

	if (isChangedB)
	{
		customSwap(deltaB.x, deltaB.y);
	}

	int eA = 2 * deltaA.y - deltaA.x;
	int eB = 2 * deltaB.y - deltaB.x;

	for (int i = 0; i <= deltaA.x; ++i)
	{
		//edge 1
		while (eA >= 0)
		{
			if (isChangedA)
				sideA.x += signA.x;
			else
				sideA.y += signA.y;
			eA -= 2 * deltaA.x;
		}

		if (isChangedA)
			sideA.y += signA.y;
		else
			sideA.x += signA.x;

		eA += 2 * deltaA.y;

		//edge 2
		while (sideB.y != sideA.y)
		{
			while (eB >= 0)
			{
				if (isChangedB)
					sideB.x += signB.x;
				else
					sideB.y += signB.y;
				eB -= 2 * deltaB.x;
			}

			if (isChangedB)
				sideB.y += signB.y;
			else
				sideB.x += signB.x;

			eB += 2 * deltaB.y;
		}

		DrawScanline(sideA, sideB);
	}
}

void Renderer::DrawScanline(const vector2 & start, const vector2 & end)
{
	int dx = abs(ceil(end.x - start.x));
	const int signx = sign(end.x - start.x);

	vector2Int current = start;
	for (int i = 0; i <= dx; ++i)
	{
		PutPixel(current, GetColor(GetBarycentricCoordinate(current)));
		current.x += signx;
	}
}

vector3 Renderer::GetBarycentricCoordinate(const vector2Int& p)
{
	if (!currentTriangle->isDenInitialized)
	{
		currentTriangle->BarycentricDenominator = 1.f / 
			((currentTriangle->vertexs[1].pos.y - currentTriangle->vertexs[2].pos.y)
				* (currentTriangle->vertexs[0].pos.x - currentTriangle->vertexs[2].pos.x) 
				+ (currentTriangle->vertexs[2].pos.x - currentTriangle->vertexs[1].pos.x)
				* (currentTriangle->vertexs[0].pos.y - currentTriangle->vertexs[2].pos.y));
		currentTriangle->isDenInitialized = true;
	}
	vector3 l(0, 0, 0);

	l.x = ((currentTriangle->vertexs[1].pos.y - currentTriangle->vertexs[2].pos.y)
		* (p.x - currentTriangle->vertexs[2].pos.x) 
		+ (currentTriangle->vertexs[2].pos.x - currentTriangle->vertexs[1].pos.x) 
		* (p.y - currentTriangle->vertexs[2].pos.y)) * currentTriangle->BarycentricDenominator;

	l.y = ((currentTriangle->vertexs[2].pos.y - currentTriangle->vertexs[0].pos.y) 
		* (p.x - currentTriangle->vertexs[2].pos.x) +
		(currentTriangle->vertexs[0].pos.x - currentTriangle->vertexs[2].pos.x) 
		* (p.y - currentTriangle->vertexs[2].pos.y)) * currentTriangle->BarycentricDenominator;

	l.z = clamp(0.f, 1.f, 1 - l.x - l.y);

	return l;
}

vector4 Renderer::GetColor(const vector3& uv)
{
	vector3 color(0, 0, 0);
	color += currentTriangle->vertexs[0].color * uv.x;// u
	color += currentTriangle->vertexs[1].color * uv.y;// v 
	color += currentTriangle->vertexs[2].color * uv.z;
	return color * 255;
}

void Renderer::PutPixel(const vector2Int& pos, const vector4& color)
{
	if (!IsInRange(pos.x, pos.y))
		return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + pos.x + g_nClientWidth * -pos.y;
	*(dest + offset) = vector3::ToColor(color);
}

vector4 SpriteRenderer::GetColor(const vector3 & _uv)
{
	//filter
	vector2 uv = _uv;
	uv.x = clamp(0.f, 1.f, (currentTriangle->vertexs[0].uv.x - currentTriangle->vertexs[2].uv.x) > 0 ? uv.x : 1 - uv.x);
	uv.y = clamp(0.f, 1.f, (currentTriangle->vertexs[1].uv.y - currentTriangle->vertexs[2].uv.y) > 0 ? uv.y : 1 - uv.y);

	return currentSprite->GetColor(uv);
}
