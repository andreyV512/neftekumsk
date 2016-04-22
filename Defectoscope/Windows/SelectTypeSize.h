#pragma once
//#include <windows.h>
#include "message.h"



class SelectTypeSize
{
public:
   HWND hWnd;
public:   
	void Create(HWND);
	void Size(int x, int y, int width);
	static void Do(TCommand &);
};