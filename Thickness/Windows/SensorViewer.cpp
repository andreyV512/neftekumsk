#include "stdafx.h"
#include "SensorViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "ZonesWindow.h"
#include "PrimaryData.h"
#include "LabelMessage.h" 
#include "DetailedWindow.h"
#include "Compute.h"
//#include "Automat.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

SensorViewer::CursorLabel::CursorLabel(Chart &chart, int n, TopLabelViewer &l)
	: chart(chart)
	, num(n)
	, topLabel(l)
{
	label.fontHeight = 10;
	label.top = 2;	
}
#pragma warning(disable : 4996)

bool SensorViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	char buf[512];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2;
	int leftOffs = int(double(l.x - x) / dX);
	int status = compute.sensorsData[num].status[leftOffs - 1];
	if(status)
	{
		char *txt = StatusLabel(status).text;		
		if(status < PrimaryData::Undefined)
		{
			double yMin =  compute.sensorsData[num].zonesMin[leftOffs - 1];
			double yMax =  compute.sensorsData[num].zonesMax[leftOffs - 1];
			double delta = compute.thicknessData.zonesMax[leftOffs - 1] - compute.thicknessData.zonesMin[leftOffs - 1];
			sprintf(buf, "<0xff>Зона <0xff0000>%d   <0xff>толщина минимум<0xff0000>%0.2f  <0xff>толщина максимум<0xff0000>%0.2f  <0xff>разностенность<0xff0000>%0.2f %s"
				, leftOffs
				, yMin
				, yMax
				, delta
				, txt
				);
		}
		else
		{
			sprintf(buf, "<0xff>Зона <0xff0000>%d  %s  "
				, leftOffs
				, txt
				);
		}
		RECT r;
		GetClientRect(l.hwnd, &r);
		label.left = 30;
		label = buf;
		label.Draw(g());
		return true;
	}
	return false;
}

bool SensorViewer::CursorLabel::GetColorBar(int zone, double &data_, unsigned &color, double &data_1, unsigned &color1)
{
	if(zone)
	{
		int i = zone - 1;
		data_1 = compute.sensorsData[num].zonesMin[i];
		data_ =  compute.sensorsData[num].zonesMax[i];
		ColorBar(data_, color1, compute.sensorsData[num].status[i]);
		
		color = color1;
		unsigned char *x = (unsigned char *) &color;
		x[0] = unsigned char(x[0] / 4 * 3);
		x[1] = unsigned char(x[1] / 4 * 3);
		x[2] = unsigned char(x[2] / 4 * 3);
		return 0 != compute.sensorsData[num].status[i];
	}
	data_ = 0;
	return true;
}

SensorViewer::SensorViewer(int n, TopLabelViewer &l)
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(chart, n, l)
	, num(n)
{
	chart.rect.left = 10;
	chart.rect.top = 20;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.maxAxesX = count_zones + 1;
	chart.minAxesY = 0;
	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
	chart.items.get<BarSeriesDouble>().SetColorBarHandler(&cursorLabel, &SensorViewer::CursorLabel::GetColorBar);
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void SensorViewer::operator()(TSize &l)
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
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, l.Width, l.Height);   
	chart.rect.right = l.Width - 10;
	chart.rect.bottom = l.Height - 10;
	chart.minAxesY = Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value;
	chart.maxAxesY = Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value;
	chart.Draw(g);
	//Pen penUp(-1, 1);
	//g.DrawLine(&penUp, 0, 0, l.Width, 0);
	//Pen penDown(0xff000000, 1);
	//g.DrawLine(&penDown, 0, l.Height - 1, l.Width, l.Height - 1);
	mouseMove = true;
}
//----------------------------------------------------------------------------------------------------
void SensorViewer::operator()(TPaint &l)
{
	if(NULL != backScreen)
	{
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(l.hwnd, &p);
		{		
			Graphics g(hdc);		
			if(backScreen)g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
			cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
		}
		EndPaint(l.hwnd, &p);
	}
}
//-----------------------------------------------------------------------------------------------------
template<class O, class P>struct __sent_mouse_move__
{
	void operator()(O *o, P *p)
	{
		p->hwnd = o->hWnd;
		if(o->backScreen)o->cursor.VerticalCursor(*p, HDCGraphics(o->hWnd, o->backScreen));
		o->storedMouseMove = *p;
	}
};
void SensorViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
	  TL::foreach<DetailedWindow::sensors_list, __sent_mouse_move__>()(&DetailedWindow::Instance().sensorsWindow, &l);
	}
}
//------------------------------------------------------------------------------------
template<class O, class P>struct sensor_viewer_button_dbl_clk__
{
	void operator()(O *o, P *p)
	{
		o->mouseMove = true;
		p->hwnd = o->hWnd;
		if(o->backScreen)o->cursor.VerticalCursor(*(TMouseMove *)p, HDCGraphics(o->hWnd, o->backScreen));
		o->storedMouseMove.x = p->x;
	}
};
void SensorViewer::operator()(TLButtonDbClk &l)
{
	SetWindowText(GetParent(l.hwnd), L"Детализированный просмотр");
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2;
	int leftOffs = int(double(l.x - x) / dX);
	if(leftOffs > 0 && compute.sensorsData[num].status[leftOffs - 1])
	{
		ZonesWindow::Open(num, leftOffs - 1);
	}
}
//-----------------------------------------------------------------------------------------
void SensorViewer::Repaint()
{
	RECT r;
	GetClientRect(hWnd, &r);
	TSize l = {hWnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
	(*this)(l);
    InvalidateRect(hWnd, NULL, true);
}
//---------------------------------------------------------------------------------------------------
void SensorViewer::operator()(TMouseWell &l)
{
	mouseMove = false;
	storedMouseMove.hwnd = l.hwnd;
	storedMouseMove.y = (WORD)(chart.rect.top + chart.rect.bottom)/2;
	chart.items.get<BottomAxes>().OffsetToPixel(storedMouseMove.x, l.delta / 120);
	if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
}
void SensorViewer::operator()(TKeyDown &l)
{
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{
		mouseMove = false;
		storedMouseMove.hwnd = l.hwnd;
		storedMouseMove.y = (WORD)(chart.rect.top + chart.rect.bottom)/2;
		chart.items.get<BottomAxes>().OffsetToPixel(storedMouseMove.x, offs);
		if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
	}
}
//---------------------------------------------------------------------------------------------------------
void SensorViewer::operator()(TLButtonDown &l)
{
	mouseMove = true;
	TL::foreach<DetailedWindow::sensors_list, sensor_viewer_button_dbl_clk__>()(&DetailedWindow::Instance().sensorsWindow, &l);
}
//-----------------------------------------------------------------------------------------------------------


