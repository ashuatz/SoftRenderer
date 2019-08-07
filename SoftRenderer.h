#pragma once

#include "resource.h"

extern int g_nClientWidth;
extern int g_nClientHeight;
extern bool g_bIsActive;


#define MAX_LOADSTRING 100

extern HWND hWnd;
extern HINSTANCE hInst;                                // 현재 인스턴스입니다.
extern WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
extern WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
extern WCHAR szDeveloper[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.