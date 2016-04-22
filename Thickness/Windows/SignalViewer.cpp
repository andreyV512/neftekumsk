#include "stdafx.h"
#include "SignalViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "AppBase.h"

#include "DebugMess.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

SignalViewer::CursorLabel::CursorLabel(SignalViewer &o, double (&d)[1024])
	: data(d)
	, owner(o)
{
	label.fontHeight = 15;
	label.top = 10;	
	label.left = 30;
	o.cursorLabel.label.fontHeight = 8;
}
#pragma warning(disable : 4996)
bool SignalViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	
	char buf[128];
	double dX = (double)(owner.chart.rect.right - owner.chart.rect.left - owner.chart.offsetAxesLeft - owner.chart.offsetAxesRight) / (owner.chart.maxAxesX - owner.chart.minAxesX);
	double x = owner.chart.rect.left + owner.chart.offsetAxesLeft - dX / 2;
	double thickness = owner.chart.items.get<VCursor2>().value - owner.chart.items.get<VCursor1>().value;
	thickness *= owner.coefficientA;
	thickness += owner.coefficientB;
	if(thickness < 0) thickness = -thickness;
	char bufTh[128];
	bufTh[0] = 0;
	if(thickness > 3.0)
	{
		sprintf(bufTh, "<ff0000> ÒÎËÙÈÍÀ %.2f ", thickness);
	}
	
	unsigned leftOffs = unsigned(double(l.x - x) / dX);
	if(leftOffs > 0 && dimention_of(data) >= leftOffs)
	{
		double y = data[leftOffs - 1];
		sprintf(buf, "%s<0xff>X <0xff0000>%d   <0xff>Y <0xff0000>%0.1f", bufTh, leftOffs, y - 127);
		RECT r;
		GetClientRect(l.hwnd, &r);
		label = buf;
		label.Draw(g());
		return true;
	}
	return false;
}

SignalViewer::SignalViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(*this, data)
	, signalLength(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, mouseMove(true)
{
	chart.rect.left = 10;
	chart.rect.top = 30;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	chart.minAxesY = -128;
	chart.maxAxesY = 127;
	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void SignalViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || l.Height < 100) return;	
	
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
	chart.maxAxesX = signalLength - 1;
	chart.items.get<LineSeries>().SetData(data, signalLength, 0, signalLength - 1);
	chart.Draw(g);
	
	RECT r = {0, 0, l.Width, l.Height};
	FrameDraw(g, r);
	mouseMove = true;	
	
}
//----------------------------------------------------------------------------------------------------
void SignalViewer::operator()(TPaint &l)
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
void SignalViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.VerticalCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}
//----------------------------------------------------------------------------------------------------
void SignalViewer::operator()(TLButtonDown &l)
{
	chart.items.get<VCursor1>().SetCoordinate(*(POINTS *)&storedMouseMove.x);
	RepaintWindow(l.hwnd);
}
//----------------------------------------------------------------------------------------------------
void SignalViewer::operator()(TRButtonDown &l)
{
	chart.items.get<VCursor2>().SetCoordinate(*(POINTS *)&storedMouseMove.x);
	RepaintWindow(l.hwnd);
}
//----------------------------------------------------------------------------------------------------
void SignalViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x; 
	}
}
//----------------------------------------------------------------------------------------------------
void SignalViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;		
		storedMouseMove.x -= l.delta / 120;
		if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
	}
}
void SignalViewer::operator()(TKeyDown &l)
{
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{
		storedMouseMove.x -= offs;
		cursor.VerticalCursorNoTest(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
	}
}
//-----------------------------------------------------------------------------------------------------

