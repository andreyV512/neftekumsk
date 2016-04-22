#pragma once
#include "message.h"
#include "MainWindowGrid.h"

class MainWindow
{
	int lastIndex;
	MainWindowGrid mainWindowGrid;
public:
	static const int IDT_TIMER1 = 999;
	HWND hWnd;
	MainWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
	void operator()(TTimer &);
	unsigned operator()(TNotify &);
};
