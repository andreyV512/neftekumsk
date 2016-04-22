#include "stdafx.h"
#include "CrossViewer.h"
#include "EmptyWindow.h"
#include "CrossViewer.h"
#include "DebugMess.h"

using namespace Gdiplus;

CrossViewer::CrossViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, crossViewerData(app.crossViewerData)
{
	
	chart.items.get<GridSeries>().sensorCount = app.sensorCrossCount;
	chart.minAxesY = 0;
	chart.maxAxesY = app.sensorCrossCount;
	chart.minAxesX = 0;
	chart.maxAxesX = App::zonesCount;
	cursor.SetMouseMoveHandler(this, &CrossViewer::CursorHandler);
	label.fontHeight = 12;
	label.top = 0;
}
//--------------------------------------------------------------------------
void CrossViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
    Graphics g(backScreen);
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
	
	chart.rect.top = 17;
	chart.rect.right = l.Width;
	chart.rect.bottom = l.Height;
	label.Draw(g);
	chart.Draw(g);

}
//-----------------------------------------------------------------------
void CrossViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		//cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//------------------------------------------------------------------------
void CrossViewer::operator()(TMouseMove &l)
{
	int x, y;
	HDCGraphics g(hWnd, backScreen);
	chart.items.get<GridSeries>().CoordCell(l.x, l.y, x, y);	
	wsprintf(label.buffer, L"<ff>зона %d  датчик %d        ", 1 + x, 1 + y);
	label.Draw(g.graphics);
	label.buffer[0] = 0;
	cursor.CrossCursor(l, g);
}
//------------------------------------------------------------------------------
void CrossViewer::operator()(TLButtonDbClk &)
{
}
//--------------------------------------------------------------------------------
void CrossViewer::operator()(TMouseWell &)
{
}
//--------------------------------------------------------------------------------------
void CrossViewer::operator()(TLButtonDown &)
{
}
//-----------------------------------------------------------------------------------------
void CrossViewer::Update()
{
	RepaintWindow(hWnd);
}
//------------------------------------------------------------------------------------------------
bool CrossViewer::CursorHandler(TMouseMove &m, VGraphics &g)
{
//	zprint("x %d y %d", m.x, m.y);

	return true;
}