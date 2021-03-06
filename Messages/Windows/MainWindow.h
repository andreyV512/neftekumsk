#pragma once
#include "message.h"
#include "MainWindowGrid.h"

class MessWindow
{
	int lastIndex;
	MainWindowGrid mainWindowGrid;
public:
	static const int IDT_TIMER1 = 999;
	HWND hWnd;
	MessWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	//void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
	void operator()(TTimer &);
	unsigned operator()(TNotify &);
};
