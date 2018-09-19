#pragma once
#include <GdiPlus.h>
#include "message.h"
class IOportsViewer
{
	HANDLE hTimer;	
public:
	int width;
public:
	HWND &hWnd;
	Gdiplus::Bitmap *&backScreen;
	bool lastTitle;
public:
	IOportsViewer(HWND &, Gdiplus::Bitmap *&);
	void Size(Gdiplus::Graphics &, int, int);
	void Start();
	void Stop();
	void MouseLDown(TLButtonDown &);
};
