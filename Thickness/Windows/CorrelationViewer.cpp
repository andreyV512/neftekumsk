#include "stdafx.h"
#include "CorrelationViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "Compute.h"
#include "AppBase.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

CorrelationViewer::CursorLabel::CursorLabel(Chart &chart, CorrelationViewer &o)
	: owner(o)
	, data(o.data)
	, chart(chart)
{
	label.fontHeight = 10;
	label.top = 10;	
	label.left = 20;
}
#pragma warning(disable : 4996)
bool CorrelationViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	char buf[256];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2;
	int leftOffs = int(double(l.x - x) / dX);
	if(leftOffs >= 0 && dimention_of(data) >= leftOffs)
	{
		double y = data[leftOffs];
		char subBuf[128];
		/*
		if(1000.0 == owner.thickness)
		{
			sprintf(subBuf, "<ff0000>ТОЛЩИНА не определена ");
		}
		else 
		{
			sprintf(subBuf, "<ff>ТОЛЩИНА <ff0000>%.2f ", owner.thickness);
		}
		*/
		if( 
			PrimaryData::Nominal == owner.status
			|| PrimaryData::Defect == owner.status
			|| PrimaryData::Treshold2Class == owner.status
			)
		{
			int thickness = int((owner.thickness - owner.coefficientB) / owner.coefficientA); 
			sprintf(subBuf, "<ff>ТОЛЩИНА <ff0000>%.2f  <ff>см.макс.<ff0000>%d ", owner.thickness, thickness);
		}
		else 
		{
			sprintf(subBuf, "<ff0000>ТОЛЩИНА не определена ");
		}
		
		sprintf(buf, "%s<ff>энергия <ff0000>%.2f  <ff>x<ff0000>%d <ff>y <ff0000>%0.2f <555555>(толщ.%.2f)"
			, subBuf
			, data[0]
			, leftOffs
			, y
			, (double)leftOffs * owner.coefficientA + owner.coefficientB
			);
		RECT r;
		GetClientRect(l.hwnd, &r);		
		label = buf;
		label.Draw(g());
		return true;
	}
	return false;
}

CorrelationViewer::CorrelationViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(chart, *this)
	, mouseMove(true)
{
	cursorLabel.label.fontHeight = 8;
	chart.rect.left = 10;
	chart.rect.top = 30;
	chart.offsetAxesBottom = 10;
	
	chart.minAxesX = 0;
	
	chart.minAxesY = 0;
	chart.maxAxesY = 12;
	cursor.SetMouseMoveHandler(&cursorLabel, &CursorLabel::Draw);

	int correlationLength = 2;
	for(int i = Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value;i >>= 1;)correlationLength <<= 1;

	chart.items.get<LineSeries>().SetData(data, correlationLength / 2,0, 0);//, 0, correlationLength / 2 - 1);
}
//----------------------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
void CorrelationViewer::operator()(TSize &l)
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
	chart.items.get<LeftBorder>().value = 
		(Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value
		  - coefficientB) / coefficientA;
	chart.items.get<RightBorder>().value = 
		(Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value
		  - coefficientB) / coefficientA;

	
	chart.items.get<PeakBorder>().value = peak;
	chart.items.get<BottomBorder>().value = bottomBorder;
	chart.items.get<MinEnergyBorder>().value = minEnergy;

	chart.Draw(g);
	RECT r = {0, 0, l.Width, l.Height};
	FrameDraw(g, r);
	mouseMove = true;
}
//----------------------------------------------------------------------------------------------------
void CorrelationViewer::operator()(TPaint &l)
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
void CorrelationViewer::operator()(TMouseMove &l)
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
void  CorrelationViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x; 
	}
}
//-----------------------------------------------------------------
void CorrelationViewer::operator()(TRButtonDown &l)
{
}
//--------------------------------------------------------------------------------------------------
void CorrelationViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x; 
	}
}
//----------------------------------------------------------------------------------------------------
void CorrelationViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		mouseMove = false;
		storedMouseMove.x -= l.delta / 120;
		if(backScreen)cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//----------------------------------------------------------------------------------------------------
void CorrelationViewer::operator()(TKeyDown &l)
{
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{
		storedMouseMove.x -= offs;
		cursor.VerticalCursorNoTest(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
	}
}
//-----------------------------------------------------------------------------------------------------

