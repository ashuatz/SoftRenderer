#pragma once

#include "stdafx.h"
#include "vertex.h"
#include "vector.h"
#include "matrix.h"
#include "customMath.h"
#include "Singleton.h"

#include <vector>
#include <map>
/*
오브젝트(정보집합)

렌더러(처리 대리자)
	정점을 받아서 처리
	내부에서 알아서 다 처리하고 픽셀찍을때만 오버라이딩이 가능한 함수 호출

*/

//하나의 객체에 유일

//렌더러는 오브젝트를 알아야하나? 아니 몰라도 됨


static class Resources
{
public:

	static ULONG* OpenBMP(const char *filename, int *width, int *height)
	{
		FILE* fp;
		errno_t err;

		err = fopen_s(&fp, filename, "rb");
		if (!fp) return NULL;

		BITMAPFILEHEADER bmpfh;
		size_t size = fread(&bmpfh, 1, sizeof(BITMAPFILEHEADER), fp);
		if (size != sizeof(BITMAPFILEHEADER)) return NULL;

		BITMAPINFOHEADER bmpih;
		size = fread(&bmpih, 1, sizeof(BITMAPINFOHEADER), fp);
		if (size != sizeof(BITMAPINFOHEADER)) return NULL;

		int bytePerLine = ((bmpih.biWidth * 3) + 3) &~3;
		size_t imageSize = bytePerLine * bmpih.biHeight;
		BYTE *pBmp = new BYTE[imageSize];
		if (!pBmp) return NULL;

		*width = bmpih.biWidth;
		*height = bmpih.biHeight;
		size = fread(pBmp, 1, imageSize, fp);
		if (size != imageSize)
		{
			delete[] pBmp;
			return NULL;
		}

		fclose(fp);

		ULONG *pImageBuf = new ULONG[bmpih.biWidth * bmpih.biHeight];

		for (int j = 0; j < bmpih.biHeight; j++)
		{
			for (int i = 0; i < bmpih.biWidth; i++)
			{
				int ColorIndex = bytePerLine * (bmpih.biHeight - j - 1) + (i * 3);
				BYTE BValue = pBmp[ColorIndex];
				BYTE GValue = pBmp[ColorIndex + 1];
				BYTE RValue = pBmp[ColorIndex + 2];

				int DestIndex = j * bmpih.biWidth + i;

				BYTE *CurrentColor = (BYTE *)(pImageBuf + DestIndex);
				pImageBuf[DestIndex] = RGB(RValue, GValue, BValue);
			}
		}

		if (pBmp)
		{
			delete[] pBmp;
		}

		return (ULONG*)pImageBuf;
	}

};

class TriangleData
{
public:
	bool isDenInitialized;
	float BarycentricDenominator;
	std::vector<vertex> vertexs;

	TriangleData(const std::vector<vertex>& v) : vertexs(v), isDenInitialized(false) {}
};

class Shader
{

};

class Material
{

};

class Renderer
{
protected:
	TriangleData* currentTriangle;

public:
	void Render(std::vector<vertex>& vertexs);

protected:
	virtual vector4 GetColor(const vector3& uv);

	void PutPixel(const vector2Int& pos, const vector4& color);
	void DrawScanline(const vector2& start, const vector2& end);
	void DrawFlatSideTriangle(const vertex& v1, const vertex& v2, const vertex& v3);
	vector3 GetBarycentricCoordinate(const vector2Int& p);
};


class Sprite
{
public:
	vector2Int size;
	std::vector<ULONG> buffer;

	ULONG GetPixel(const vector2& uv)
	{
		int value = clamp(0, size.x * size.y - 1, (int)(uv.y * (size.y) - 0.5f) * (size.x) + (int)(uv.x * (size.x) - 0.5f));
		return buffer[value];
	}


	vector3 GetColor(const vector3& uv)
	{
		return vector3::ToVector3(GetPixel(uv));
	}

private:
	Sprite() = delete;

	friend class Resource;
	friend class ResourceManager;

	Sprite(std::string fileName)
	{
		auto data = Resources::OpenBMP(fileName.c_str(), &size.x, &size.y);
		buffer = std::vector<ULONG>(data, data + (size.x * size.y));
	}
};


class SpriteRenderer : public Renderer
{
public:
	Sprite* currentSprite;

protected:
	virtual vector4 GetColor(const vector3& uv) override;
};


class Transform
{
public:
	Transform* parent;
	Matrix3x3 objectMatrix;

public:
	void SetPosition(const vector2& position)
	{
		MatrixTranslation(objectMatrix, position.x, position.y);
	}

	void SetRotation(const float&  rotation)
	{
		MatrixRotationDir(objectMatrix, rotation); \
	}

	void SetScale(const vector2& scale)
	{
		MatrixScale(objectMatrix, scale.x, scale.y);
	}


	void AddPosition(const vector2& position)
	{
		Matrix3x3 temp;
		MatrixIdentity(temp);
		MatrixTranslation(temp, position.x, position.y);
		objectMatrix += temp;
	}

	void AddRotation(const float& rotation)
	{
		MatrixRotationDir(objectMatrix, rotation);
	}

	void AddScale(const vector2& scale)
	{
		MatrixScale(objectMatrix, scale.x, scale.y);
	}

};

class Object
{
public:
	std::vector<vertex> vertexes;
	Transform* transform;
	Renderer* Renderer;
};


//로드한 리소스 정보를 캐싱하고 있는 객체
class ResourceManager : public Singleton<ResourceManager>
{
private:
	std::map<std::string, Sprite*> SpriteMap;

public:
	Sprite* GetSprite(const std::string& fileName)
	{
		auto obj = SpriteMap[fileName];
		if (obj == nullptr)
		{
			obj = new Sprite(fileName);
			SpriteMap[fileName] = obj;
		}

		return obj;
	}
};

//오브젝트 정보를 캐싱하고 있는 객체
class World : public Singleton<World>
{
private:
	std::vector<Object*> RegistedObject;

public:
	Object* GetRegistedObject(int index)
	{
		return RegistedObject[index];
	}

	void AddObject(Object * const obj)
	{
		RegistedObject.push_back(obj);
	}

	void RemoveObject(Object* const obj)
	{
		this->RegistedObject.erase(std::remove_if(RegistedObject.begin(), RegistedObject.end(),
			[=](Object * const t) {
			return t == obj;
		}), RegistedObject.end());
	}

	void RenderAll()
	{
		for (auto& it : RegistedObject)
		{
			it->Renderer->Render(it->vertexes);
		}
	}
};