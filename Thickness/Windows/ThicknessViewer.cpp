#include "stdafx.h"
#include "ThicknessViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "PrimaryData.h"
#include "LabelMessage.h"
#include "DetailedWindow.h" 
//#include "Automat.h"         
#include "DebugMess.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;


ThicknessViewer::CursorLabel::CursorLabel(Chart &chart)
	: chart(chart)	
{
	label.fontHeight = 10;
	label.top = 2;	
}

bool ThicknessViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	char buf[512];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = -0.5 * dX + chart.rect.left + chart.offsetAxesLeft;
	int leftOffs = int((double(l.x) - x) / dX);
	int status =  thicknessData.status[leftOffs - 1];///////////////////////////////////////////////
	if(status)
	{		
		char *txt = StatusLabel(status).text;
		if(status < PrimaryData::Undefined)// || status == PrimaryData::DefectMinMax || status == PrimaryData::DefectDifferentWall)
		{
			double yMin = thicknessData.zonesMin[leftOffs - 1];////////////////////////////////////////////////////
			double yMax = thicknessData.zonesMax[leftOffs - 1];
			double delta = yMax - yMin;
			sprintf(buf, "<ff>Зона <0xff0000>%d <ff>Смещение <ff0000>%.2f <ff>м толщ <0xff0000>%0.2f %s <ff>\"1 класс\" %.1f  <ff>\"2 класс\" %.1f"
				, leftOffs
				, 0.001 * zone_length * leftOffs 
				, yMin
			//	, yMax
			//	, delta
				, txt
				, Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value
				, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect>().value
			//	, Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value	
			//	, Singleton<ThresholdsTable>::Instance().items.get<DefectDifferentWallPercent>().value
				);
		}
		else
		{
			sprintf(buf, "<ff>Зона <ff0000>%d  %s  "
				, leftOffs
				, txt
				);
		}
		RECT r;
		GetClientRect(l.hwnd, &r);
		label.left = 40;
		label = buf;
		label.Draw(g());
		return true;
	}
	return false;
}

bool ThicknessViewer::CursorLabel::GetColorBar(int zone, double &data_, unsigned &color)
{
	if(zone)
	{
		int i = zone - 1;
		data_ = thicknessData.zonesMin[i];
		ColorBar(data_, color, thicknessData.status[i]);

		return 0 != thicknessData.status[i];
	}
	data_ = 0;
	return true;
}
//-----------------------------------------------------------------------------
ThicknessViewer::ThicknessViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, openDetailedWindow(false)
	, painting(true)
	, mouseMove(true)
	, cursorLabel(chart)
{
	chart.rect.left = 10;
	chart.rect.top = 17;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.maxAxesX = count_zones;
	chart.minAxesY = 0;

	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
	chart.items.get<BarSeries>().SetColorBarHandler(&cursorLabel, &ThicknessViewer::CursorLabel::GetColorBar);

	chart.items.get<BottomAxesMeters>().minBorder = 0;
	chart.items.get<BottomAxesMeters>().maxBorder = 0.001 * count_zones * zone_length;
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ThicknessViewer::operator()(TSize &l)
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
	g.FillRectangle(&solidBrush, 0, 29, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  

	chart.minAxesY = Singleton<AxesTable>::Instance().items.get<MinAxes>().value;
	chart.maxAxesY = Singleton<AxesTable>::Instance().items.get<MaxAxes>().value;
	chart.rect.right = l.Width - 10;
	chart.rect.bottom = l.Height - 10;
	chart.Draw(g);
}
//----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{	
		if(backScreen)
		{
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
		}
	}
	EndPaint(l.hwnd, &p);
}
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(backScreen)if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
}
//--------------------------------------------------------------------------
void ThicknessViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(backScreen)if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
}
void ThicknessViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;
		storedMouseMove.hwnd = l.hwnd;
		chart.items.get<BottomAxesMeters>().OffsetToPixel(storedMouseMove.x, l.delta / 120);
		if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
void ThicknessViewer::operator()(TKeyDown &l)
{	
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{
		mouseMove = false;
		chart.items.get<BottomAxesMeters>().OffsetToPixel(storedMouseMove.x, offs);
		if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//-----------------------------------------------------------------------------------------------------
void ThicknessViewer::Update()
{
	RepaintWindow(hWnd);
}
//------------------------------------------------------------------------------------------------------
void ThicknessViewer::DrawSomeBar()
{
	static unsigned lastTime;
	unsigned t = GetTickCount();
	if(t - lastTime > 1000)
	{
		lastTime = t;
		RepaintWindow(hWnd);
	}
}
//------------------------------------------------------------------------------------------------------
ThicknessViewer thicknessViewer;