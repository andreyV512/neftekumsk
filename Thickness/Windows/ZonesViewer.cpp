#include "stdafx.h"
#include "ZonesViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "PrimaryData.h"
#include "LabelMessage.h"
#include "DebugMess.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

ZonesViewer::CursorLabel::CursorLabel(Chart &chart, ZonesViewer &d)
	: owner(d)
    , data(d.data)
	, chart(chart)
	, offset(0)
{
	label.fontHeight = 15;
	label.top = 10;	
}
#pragma warning(disable : 4996)
bool ZonesViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	char buf[512];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);	
	double x = -0.5 * dX + chart.rect.left + chart.offsetAxesLeft;	
	int leftOffs = int((double(l.x) - x) / dX);
	if((unsigned)leftOffs > dimention_of(data.status)) return false;
	int status = data.status[leftOffs];
	if(status)
	{
		offset = leftOffs;
		if(status == PrimaryData::Nominal || status == PrimaryData::Defect || status == PrimaryData::Treshold2Class)
		{
			sprintf(buf, "<0xff>Смещение <0xff0000>%d   <0xff>толщина <0xff0000>%0.2f %s   <ff>\"1 класс\" %.2f \"2 класс\" <ff0000>%.2f"//%  <ff>плюс <ff0000>%.2f%%"
				, offset + owner.zonesOffset
				, data.zones[offset]
			    , StatusLabel(status).text
				, Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value
				, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect>().value
				//, Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value
				);
		}
		else
		{
			sprintf(buf, "<0xff>Смещение <0xff0000>%d   %s   "
				, offset + owner.zonesOffset
				, StatusLabel(status).text
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

bool ZonesViewer::CursorLabel::GetColorBar(int zone, double &data_, unsigned &color)
{
	data_ = data.zones[zone];
	ColorBar(data_, color, data.status[zone]);
	return 0 != data.status[zone];
}

ZonesViewer::ZonesViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(chart, *this)
	, offset(cursorLabel.offset)
	, obj(NULL)
	, ptr(NULL)
{
	chart.rect.left = 10;
	chart.rect.top = 30;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.minAxesY = 0;
	
	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
	chart.items.get<BarSeries>().SetColorBarHandler(&cursorLabel, &CursorLabel::GetColorBar);
	mouseMove = true; 
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void ZonesViewer::operator()(TSize &l)
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
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, l.Width, l.Height); ////
	
	chart.rect.right = l.Width - 10;
	chart.rect.bottom = l.Height - 10;

	chart.minAxesY = Singleton<AxesTable>::Instance().items.get<MinAxes>().value;
	chart.maxAxesY = Singleton<AxesTable>::Instance().items.get<MaxAxes>().value;
	chart.minAxesX = zonesOffset;
	chart.maxAxesX = zonesOffset + data.countZones - 1;

	chart.Draw(g);
	RECT r = {0, 0, l.Width, l.Height};
	FrameDraw(g, r);
	mouseMove = true;
}
//----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}
//-----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TMouseMove &l)
{
	
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
		else
		{
			cursor.VerticalCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
		}
		if(obj&&ptr)(obj->*ptr)(offset);
	}
}
void ZonesViewer::operator()(TKeyDown &l)
{
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{		
		chart.items.get<BottomAxesInt>().OffsetToPixel(storedMouseMove.x, offs);
		cursor.VerticalCursorNoTest(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
		if(obj&&ptr)(obj->*ptr)(offset);
	}
}
//-----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
	if(obj&&ptr)(obj->*ptr)(offset);
}
//----------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x;
	}
	if(obj&&ptr)(obj->*ptr)(offset);
}
//---------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;

		chart.items.get<BottomAxesInt>().OffsetToPixel(storedMouseMove.x, l.delta / 120);
		if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
		if(obj&&ptr)(obj->*ptr)(offset);
	}
}
//---------------------------------------------------------------------------------------------------
void ZonesViewer::operator()(TDestroy &l)
{
}
//------------------------------------------------------------------------------------

