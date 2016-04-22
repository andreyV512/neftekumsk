#pragma once
#include <windows.h>
#include <commctrl.h>
#include "message.h"
//-----------------------------------------------------------------
void SetRow(HWND hWnd, int count);
//-----------------------------------------------------------------
struct GridHandlers
{
	virtual void SetDataToGrid(LV_DISPINFO *){}
	virtual void SetColorToGrid(NMLVCUSTOMDRAW *){}
	virtual void SetHeaderToGrid(HWND){}
	virtual void ItemChanged(HWND){}
	virtual void Timer(HWND){}
	virtual void LClick(LPNMITEMACTIVATE ){}
	virtual void RClick(LPNMITEMACTIVATE ){}
	virtual ~GridHandlers(){}
};
//----------------------------------------------------------------