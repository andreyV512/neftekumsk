#pragma once
#include <typeinfo>
#include <windows.h>
#include "message.h"
#include "resource.h"
#include "WindowsEventTemplate.hpp"
//---------------------------------------------------------------------------------
ATOM MyRegisterClass(WNDPROC WndProc
	, const wchar_t *windowClassName
	, int style = CS_HREDRAW | CS_VREDRAW
	, int idIcon = 0
	, int idSmIcon = 0
	, HBRUSH backGround = NULL					 
	);

HWND MyCreateWindow(const wchar_t *windowClass, const wchar_t *title = NULL
	, void *data = NULL
	, int style = WS_OVERLAPPEDWINDOW
	, HWND hParent = NULL
	, int x = CW_USEDEFAULT
	, int y = CW_USEDEFAULT
	, int width = CW_USEDEFAULT
	, int height = CW_USEDEFAULT
	);

HWND CreateChildWindow(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data);
HWND CreateChildWindowBackground(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data);

#define CREATE_CHILD_WINDOW(hParent, type) o##type.hWnd = CreateChildWindow(hParent, (WNDPROC)&Viewer<type>::Proc, L###type, &o##type);

#pragma warning(disable :  4996)
template<class T>HWND WindowTemplate(
	T *self
	, wchar_t *title	
	, int x = CW_USEDEFAULT
	, int y = CW_USEDEFAULT
	, int width = CW_USEDEFAULT
	, int height = CW_USEDEFAULT
	, int styleS = IDI_UPLOAD
	, HBRUSH backGround = NULL	
	)
{
	wchar_t buf[512];
	mbstowcs(buf, &(typeid(T).name())[6], dimention_of(buf));
	MyRegisterClass(Viewer<T>::Proc, buf,  CS_HREDRAW | CS_VREDRAW, styleS, styleS, backGround);
	HWND hWnd = MyCreateWindow(buf, title, self, WS_OVERLAPPEDWINDOW, NULL, x, y, width, height);
	return hWnd;
}

void RepaintWindow(HWND h);


