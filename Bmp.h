#pragma once

#include "stdafx.h"

ULONG* OpenBMP(char *filename, int *pwidth, int *pheight);
ULONG GetPixel(int x, int width, int y, ULONG* bmp);