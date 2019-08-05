#pragma once
#include "stdafx.h"
#include "SoftRenderer.h"


template <typename T>
static int sign(const T& val)
{
	return (T(0) < val) - (val < T(0));
}

template<typename T>
static void customSwap(T& a, T& b)
{
	T Temp = a;
	a = b;
	b = Temp;
}

static bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

static float lerp(const float& a, const float& b, const float& f)
{
	return a + f * (b - a);
}