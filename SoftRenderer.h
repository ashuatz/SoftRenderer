#pragma once

#include "resource.h"

extern int g_nClientWidth;
extern int g_nClientHeight;
extern bool g_bIsActive;


#define MAX_LOADSTRING 100

extern HWND hWnd;
extern HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
extern WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
extern WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.
extern WCHAR szDeveloper[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.