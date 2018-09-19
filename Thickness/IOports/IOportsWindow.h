#pragma once

#include <windows.h>
#include "message.h"
#include "IOportsViewer.h"
class IOportsWindow
{
public:
	static const int width = 680;
	static const int height = 260;
	Gdiplus::Bitmap *backScreen;
	IOportsViewer viewer;
public:
	HWND hWnd;
	IOportsWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TPaint &);
	void operator()(TLButtonDown &);
	static IOportsWindow &Instance();
};


