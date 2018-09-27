#include "stdafx.h"
#include "ACFViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "Compute.h"
#include "MenuApi.h"
#include "ZonesWindow.h"
#include "Pass.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "Dlg.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

ACFViewer::CursorLabel::CursorLabel(Chart &chart, ACFViewer &o)
	: owner(o)
	, data(o.data)
	, chart(chart)
{
	label.fontHeight = 10;
	label.top = 10;	
	label.left = 20;
}
#pragma warning(disable : 4996)
bool ACFViewer::CursorLabel::Draw(TMouseMove &l, VGraphics &g)
{
	char buf[256];
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2;
	int leftOffs = int(double(l.x - x) / dX);
	if(leftOffs >= 0 && dimention_of(data) >= leftOffs)
	{
		double y = data[leftOffs];
		sprintf(buf, "<ff0000>АЧХ  <ff>x<ff0000>%d <ff>y <ff0000>%f <ff>левяя %.0f <ff>правая %.0f"
			, leftOffs
			, y
			, owner.chart.items.get<LeftBorder>().value
			, owner.chart.items.get<RightBorder>().value
			);
		RECT r;
		GetClientRect(l.hwnd, &r);		
		label = buf;
		label.Draw(g());
		return true;
	}
	return false;
}

ACFViewer::ACFViewer()
	: backScreen(NULL)
	, chart(backScreen)
	, cursor(chart)
	, cursorLabel(chart, *this)
	, mouseMove(true)
	, bordersProc(NULL)
{
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
void ACFViewer::operator()(TSize &l)
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
	chart.items.get<LeftBorder>().value =  acfBorderLeft;
	chart.items.get<RightBorder>().value = acfBorderRight;

	chart.Draw(g);
	RECT r = {0, 0, l.Width, l.Height};
	FrameDraw(g, r);
	mouseMove = true;
}
//----------------------------------------------------------------------------------------------------
void ACFViewer::operator()(TPaint &l)
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
void ACFViewer::operator()(TMouseMove &l)
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
void  ACFViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x; 
	}
}
//-----------------------------------------------------------------
void ACFViewer::LeftBorderProc (int x)
{
	int t = (int)acfBorderLeft;
	t -= x;
	if(t > 0 && t < acfBorderRight)acfBorderLeft = t;	
	ZonesWindow &z = ZonesWindow::Instance();
	int tmpLeft = compute.acfBorderLeft[z.sensor];
	compute.acfBorderLeft[z.sensor] = (int)acfBorderLeft;
	int tmpRight = compute.acfBorderRight[z.sensor];
	compute.acfBorderRight[z.sensor] = (int)acfBorderRight;
	compute.CalculationZoneSensor(z.zone - 1, z.sensor, z.zonesViewer.data.zones, z.zonesViewer.data.status, z.zonesViewer.data.offs);
	RepaintWindow(z.zonesViewer.hWnd);
	z.MouseMoveHandler(z.currentOffsetInZone);
	compute.acfBorderLeft[z.sensor] = tmpLeft;
	compute.acfBorderRight[z.sensor] = tmpRight;

	wchar_t buf[512];
	wsprintf(buf, L"Кадров в зоне: %d", compute.allData[z.sensor]);
	SetWindowText(z.hLabelAllFrames, buf); 
	wsprintf(buf, L"Измеренных кадров: %d  %s%%", compute.goodData[z.sensor], Wchar_from<double, 4>(100.0 * compute.goodData[z.sensor]/compute.allData[z.sensor])());
	SetWindowText(z.hLabelCalculatedFrames, buf);

}
void ACFViewer::RightBorderProc(int x)
{
	int t = acfBorderRight;
	t -= x;
	if(acfBorderRight > acfBorderLeft && acfBorderRight < chart.maxAxesX)acfBorderRight = t;
	ZonesWindow &z = ZonesWindow::Instance();
	int tmpLeft = compute.acfBorderLeft[z.sensor];
	compute.acfBorderLeft[z.sensor] = (int)acfBorderLeft;
	int tmpRight = compute.acfBorderRight[z.sensor];
	compute.acfBorderRight[z.sensor] = (int)acfBorderRight;
	compute.CalculationZoneSensor(z.zone - 1, z.sensor, z.zonesViewer.data.zones, z.zonesViewer.data.status, z.zonesViewer.data.offs);
	RepaintWindow(z.zonesViewer.hWnd);
	z.MouseMoveHandler(z.currentOffsetInZone);	
	compute.acfBorderLeft[z.sensor] = tmpLeft;
	compute.acfBorderRight[z.sensor] = tmpRight;

	wchar_t buf[512];
	wsprintf(buf, L"Кадров в зоне: %d", compute.allData[z.sensor]);
	SetWindowText(z.hLabelAllFrames, buf); 
	wsprintf(buf, L"Измеренных кадров: %d  %s%%", compute.goodData[z.sensor], Wchar_from<double, 4>(100.0 * compute.goodData[z.sensor]/compute.allData[z.sensor])());
	SetWindowText(z.hLabelCalculatedFrames, buf);
}
namespace ACF_Space
{
template<class O, class P>struct __RecomputeProc__
{
	void operator()(O &o, P &p)
	{
		p.update.set<O>(o.value);
	}
};
struct __save_data__
{
	int id, acfBorderLeft, acfBorderRight;
};
template<class O, class P>struct __save__;
template<int N, class P>struct __save__<ACFBorderLeft<N>, P>
{
	void operator()(ACFBorderLeft<N> &o, P &p)
	{
		if(N == p.id)
		{
			o.value = p.acfBorderLeft;
			compute.acfBorderLeft[N] = p.acfBorderLeft;
		}
	}
};
template<int N, class P>struct __save__<ACFBorderRight<N>, P>
{
	void operator()(ACFBorderRight<N> &o, P &p)
	{
		if(N == p.id)
		{
			o.value = p.acfBorderRight;
			compute.acfBorderRight[N] = p.acfBorderRight;
		}
	}
};
}
void ACFViewer::RecomputeProc  (int x)
{
	bordersProc = NULL;
	if(TypesizePasswordDlg().Do(hWnd))
	{
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			ACFBorderTable &t = Singleton<ACFBorderTable>::Instance();
			ACF_Space::__save_data__ save_data = {ZonesWindow::Instance().sensor, acfBorderLeft, acfBorderRight};
			TL::foreach<ACFBorderTable::items_list, ACF_Space::__save__>()(t.items, save_data);
			__update_data__<ACFBorderTable> _data(base);
			TL::foreach<ACFBorderTable::items_list, ACF_Space::__RecomputeProc__>()(t.items, _data);
			_data.update.Where().ID(1).Execute();
			Recalculation::Do(0);
		}
	}
}
//-----------------------------------------------------------------
namespace ACF_Space
{
#define CONTEXT_MENU(name, txt, proc)\
struct name{};\
template<>struct TopMenu<name>{typedef NullType list;};\
MENU_TEXT(txt, TopMenu<name>)\
template<>struct Event<TopMenu<name> >	   \
{										   \
	static void Do(HWND h)				   \
	{									   \
		zprint("\n");					   \
		((ACFViewer *)GetWindowLong(h, GWL_USERDATA))->bordersProc = proc;\
	}									   \
};
CONTEXT_MENU(LeftBorder, L"Левая граница",   &ACFViewer::LeftBorderProc )
CONTEXT_MENU(RightBorder, L"Правая граница", &ACFViewer::RightBorderProc)

struct Recompute{};
template<>struct TopMenu<Recompute>{typedef NullType list;};
MENU_TEXT(L"Сохранить", TopMenu<Recompute>)
template<>struct Event<TopMenu<Recompute> >	   
{										   
	static void Do(HWND h)				   
	{									   
		((ACFViewer *)GetWindowLong(h, GWL_USERDATA))->RecomputeProc(0);
	}									   
};
									   
#undef CONTEXT_MENU

void RightButtonDown(HWND h)
{
	PopupMenu<TL::MkTlst<
		TopMenu<LeftBorder>
		, TopMenu<RightBorder>
		, Separator<0>
		, TopMenu<Recompute>
	>::Result>::Do(h, h);
}

}
void ACFViewer::operator()(TRButtonDown &l)
{
	ACF_Space::RightButtonDown(l.hwnd);
}
//--------------------------------------------------------------------------------------------------
void ACFViewer::operator()(TLButtonDbClk &l)
{
	mouseMove = true;
	if(cursor.VerticalCursor(*(TMouseMove *)&l, HDCGraphics(l.hwnd, backScreen)))
	{
		storedMouseMove.x = l.x; 
	}
}
//----------------------------------------------------------------------------------------------------
void ACFViewer::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		int dx = l.delta / 120;
		if(NULL != bordersProc) (this->*bordersProc)(dx);
		mouseMove = false;
		storedMouseMove.x -= dx;
		cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
	}
}
//----------------------------------------------------------------------------------------------------
void ACFViewer::operator()(TKeyDown &l)
{
	int offs = VK_RIGHT == l.VirtKey ? -1 : VK_LEFT == l.VirtKey ? 1 : 0;
	if(offs)
	{
		storedMouseMove.x -= offs;
		cursor.VerticalCursorNoTest(storedMouseMove, HDCGraphics(l.hwnd, backScreen));
	}
}

