#include "stdafx.h"
#include "CorrelationViewer.h"
#include <stdio.h>
#include "EmptyWindow.h"
#include "Compute.h"
#include "AppBase.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "MenuApi.h"
#include "DebugMess.h"
#include "DlgTemplates/EditDlg.h"
#include "templates/templates.hpp"
#include "ZonesWindow.h"
#include "Dlg.h"
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
		
		if( 
			PrimaryData::Nominal == owner.status
			|| PrimaryData::Defect == owner.status
			|| PrimaryData::Treshold2Class == owner.status
			|| PrimaryData::Treshold3Class == owner.status
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

	
	chart.items.get<PeakBorder>().value = paramPeak * data[0];
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
namespace CorrelationSpace
{
	bool MinEnergyOkBtn(HWND h, wchar_t *txt)
	{
		double t = Wchar_to<double>()(txt);
		if(t <= 0)
		{
			MessageBox(h, L"Параметр должен быть больше 0", L"Ошибка !!!", MB_ICONERROR);
			return false;
		}	
		CorrelationViewer &w = ZonesWindow::Instance().correlationViewer;
		if(t >= w.maxEnergy)
		{
			wchar_t buf[128];
			wsprintf(buf,  L"Параметр должен быть меньше %s", Wchar_from<double, 5>(w.maxEnergy)());
			MessageBox(h, buf, L"Ошибка !!!", MB_ICONERROR);
			return false;
		}	
		w.minEnergy = t;
		return true;
	}
	bool MaxEnergyOkBtn(HWND h, wchar_t *txt)
	{
		double t = Wchar_to<double>()(txt);
		if(t <= 0)
		{
			MessageBox(h, L"Параметр должен быть больше 0", L"Ошибка !!!", MB_ICONERROR);
			return false;
		}	
		CorrelationViewer &w = ZonesWindow::Instance().correlationViewer;
		if(t >= w.maxEnergy)
		{
			wchar_t buf[128];
			wsprintf(buf,  L"Параметр должен быть больше %s", Wchar_from<double, 5>(w.minEnergy)());
			MessageBox(h, buf, L"Ошибка !!!", MB_ICONERROR);
			return false;
		}	
		w.maxEnergy = t;
		return true;
	}
	bool PeakOkBtn(HWND h, wchar_t *txt)
	{
		double t = Wchar_to<double>()(txt);
		if(t <= 0)
		{
			MessageBox(h, L"Параметр должен быть больше 0", L"Ошибка !!!", MB_ICONERROR);
			return false;
		}	
		if(t >= 1.0)
		{
			MessageBox(h, L"Параметр должен быть меньше 1", L"Ошибка !!!", MB_ICONERROR);
			return false;
		}	
		ZonesWindow::Instance().correlationViewer.paramPeak = t;
		return true;
	}

	struct __save_data__
	{
		int id;
		double MinEnergy, MaxEnergy, Peak;
	};
	template<class O, class P>struct __save__;

#define SAVE(name)template<int N, class P>struct __save__<name<N>, P>\
{void operator()(name<N> &o, P &p){if(N == p.id)o.value = p.name;}};

SAVE(MinEnergy)
SAVE(MaxEnergy)
SAVE(Peak)

#undef SAVE	
template<class O, class P>struct __RecomputeProc__
{
	void operator()(O &o, P &p)
	{
		p.update.set<O>(o.value);
	}
};
}
void CorrelationViewer::ChangeParam()
{
	ZonesWindow &z = ZonesWindow::Instance();
	double tmpMinEnergy = compute.minEnergy[z.sensor];
	double tmpMaxEnergy = compute.maxEnergy[z.sensor];
	double tmpPeak      = compute.peak[z.sensor];     
	compute.minEnergy[z.sensor] = minEnergy;
	compute.maxEnergy[z.sensor]	= maxEnergy;
	compute.peak[z.sensor]     	= paramPeak;
	compute.CalculationZoneSensor(z.zone - 1, z.sensor, z.zonesViewer.data.zones, z.zonesViewer.data.status);
	compute.minEnergy[z.sensor] = tmpMinEnergy;
	compute.maxEnergy[z.sensor] = tmpMaxEnergy;
	compute.peak[z.sensor]      = tmpPeak     ;
	RepaintWindow(hWnd);
	RepaintWindow(ZonesWindow::Instance().zonesViewer.hWnd);
}
void CorrelationViewer::MinEnergy()
{
	Wchar_from<double, 5> buf(minEnergy);
	if(EditDlg(hWnd, L"Минимальная энергия", buf(), &CorrelationSpace::MinEnergyOkBtn).result)
	{
		ChangeParam();
	}
}
void CorrelationViewer::MaxEnergy()
{
	Wchar_from<double, 5> buf(maxEnergy);
	if(EditDlg(hWnd, L"Минимальная энергия", buf(), &CorrelationSpace::MaxEnergyOkBtn).result)
	{
		ChangeParam();
	}
}
void CorrelationViewer::Peak()
{
	Wchar_from<double, 5> buf(paramPeak);
	if(EditDlg(hWnd, L"Энергия к первому пику", buf(), &CorrelationSpace::PeakOkBtn).result)
	{
		ChangeParam();
	}
}
void CorrelationViewer::Save()
{
	if(OptionPasswordDlg().Do(hWnd))
	{
		CorrelationSpace::__save_data__ data = {ZonesWindow::Instance().sensor, minEnergy, maxEnergy, paramPeak};
		SignalParametersTable::TItems &items = Singleton<SignalParametersTable>::Instance().items;
		TL::foreach<SignalParametersTable::items_list, CorrelationSpace::__save__>()(items, data);
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			__update_data__<SignalParametersTable> _data(base);
			TL::foreach<SignalParametersTable::items_list, CorrelationSpace::__RecomputeProc__>()(items, _data);
			_data.update.Where().ID(1).Execute();
		}
		Recalculation::Do(0);
	}
}
//--------------------------------------------------------------------
namespace CorrelationSpace
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
		((CorrelationViewer *)GetWindowLong(h, GWL_USERDATA))->proc();\
	}									   \
};

CONTEXT_MENU(_MinEnergy, L"Минимальная энергия",   CorrelationViewer::MinEnergy )
CONTEXT_MENU(_MaxEnergy, L"Максимальная энергия", CorrelationViewer::MaxEnergy)
CONTEXT_MENU(_Peak, L"Энергия к первому пику", CorrelationViewer::Peak)
CONTEXT_MENU(_Save, L"Сохранить", CorrelationViewer::Save)
#undef CONTEXT_MENU

void RightButtonDown(HWND h)
{
	PopupMenu<TL::MkTlst<
		TopMenu<_MinEnergy>
		, TopMenu<_MaxEnergy>
		, TopMenu<_Peak>
		, Separator<0>
		, TopMenu<_Save>
	>::Result>::Do(h, h);
}
}
void CorrelationViewer::operator()(TRButtonDown &l)
{
	CorrelationSpace::RightButtonDown(l.hwnd);
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

