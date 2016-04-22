#pragma once
#include <windows.h>
#include "message.h"
#include "MainWindowToolBar.h"

class MainWindow
{
	Gdiplus::Bitmap *backScreen;
public:
	MainWindowToolBar toolBar;
public:
	HWND hWnd;
	HWND hStatusWindow;
	MainWindow();
	void operator()(TPaint &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
};
