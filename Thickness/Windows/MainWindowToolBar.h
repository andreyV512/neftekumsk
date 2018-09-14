#pragma once

class MainWindowToolBar
{
public:
	HWND hWnd;
	HWND Init(HWND);
	void Size();
	static const int Width();

	static void Cycle();
	static void NoCycle();
};





