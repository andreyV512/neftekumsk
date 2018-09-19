#include "stdafx.h"
#include "IOportsViewer.h"
#include "ColorLabel.h"
#include "AppBase.h"
#include "DebugMess.h"
#include "Chart.h"
#include "Device1730.h"
#include "App.h"
#include "DlgTemplates\ParamDlg.hpp"
using namespace Gdiplus;
//-----------------------------------------------------------------------------
namespace
{
struct __draw_data__
{
	int x, dY;
	Graphics &g;
	int colorOn;
	int value;
	__draw_data__(int x, Graphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};

#define OFFS(z, txt)\
PARAM_TITLE(z, txt)\
template<>struct DlgSubItems<z, unsigned >: UpDownSubItem<z>{};

OFFS(SQ1BitIn           , L"SQ1 ������������ ����������(���� �����)")
OFFS(SQ3BitIn           , L"SQ3 ������������ ����������(����� �����)")
OFFS(StateBBitIn        , L"�(��������� ���������� ���������������)")
OFFS(StateABitIn        , L"�(��������� ���������� ���������������)")
OFFS(RunBitIn           , L"RUN(��������� ���������� ���������������)")
OFFS(ControlCircuitBitIn, L"���� ����������")
OFFS(WorkBitIn          , L"������(�� ����� 9955-191/16)")
OFFS(SycleBitIn         , L"����(�� ����� 9955-191/16)")
OFFS(BaseBitIn    	   	, L"����(�� ����� 9955-191/16)")

OFFS(PowerInverterBitOut, L"������� ���������� ���������������")
OFFS(SpeedRLBitOut    	, L"�������� RL")
OFFS(SpeedRMBitOut    	, L"�������� RM")
OFFS(SpeedRHBitOut    	, L"�������� RH")
OFFS(STFBitOut        	, L"STF")
OFFS(PowerScannerBitOut	, L"������� ������������ ����������")
OFFS(ReadyBitOut      	, L"����������(� ���� 9955-191/16)")
OFFS(ControlBitOut    	, L"��������(� ���� 9955-191/16)")
OFFS(ResultBitOut     	, L"���������(� ���� 9955-191/16)")

#undef OFFS

template<class O, class P>struct __draw__
{
	void operator()(O *o, P *p)
	{
		ColorLabel l;
		l.left = p->x;
		l.top = p->dY;
		wsprintf(l.buffer, L"<55>%s",  ParamTitle<O>()());
		l.Draw(p->g);
		int color = 0xffcccccc;
		p->g.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
//------------------------------------------------------------------------------------
struct __update_data_x__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	int value;
	__update_data_x__(int x, HDCGraphics &g, int colorOn, int value)  : x(x), dY(15), g(g), colorOn(colorOn), value(value) {}
};
template<class O, class P>struct __update__
{
	void operator()(O *o, P *p)
	{
		int color = 0xffcccccc;
		if(o->value & p->value) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), p->x - 20, p->dY - 3, 15, 15);
		p->dY += 20;
	}
};
struct __mouse_down_data__
{
	int x, dY;
	HDCGraphics &g;
	int colorOn;
	unsigned &value;
	TLButtonDown &l;
	Device1730 &device1730;
	__mouse_down_data__(int x, HDCGraphics &g, int colorOn, unsigned &value, TLButtonDown &l, Device1730 &device1730)  
		: x(x), dY(15), g(g), colorOn(colorOn), value(value), l(l) 
		, device1730(device1730)
	{}
};
template<class O, class P>struct __mouse_down__
{
	void operator()(O *o, P *p)
	{
		int x0 = p->x - 20;
		int x1 = x0 + 15;
		int y0 = p->dY - 3;
		int y1 = y0 + 15;
		if(p->l.x > x0 && p->l.x < x1 && p->l.y > y0 && p->l.y < y1)
		{
			p->value ^= o->value;
			p->device1730.Write(p->value);
		}
		int color = 0xffcccccc;
		unsigned output = p->device1730.ReadOutput();
		if(o->value & output) color = p->colorOn;
		p->g.graphics.FillRectangle(&SolidBrush(Color(color)), x0, y0, 15, 15);
		p->dY += 20;
	}
};
VOID CALLBACK __Update__(PVOID oo, BOOLEAN)
{	
	IOportsViewer *o = (IOportsViewer *)oo;
	if(app.measurementOfRunning != o->lastTitle)
	{
		o->lastTitle = app.measurementOfRunning;
		wchar_t *title = o->lastTitle
			? L"�������� ���������� ������-�������"
			: L"���������� ����������� ��������"
		;
		SetWindowText(o->hWnd, title);
	}
	HDCGraphics g(o->hWnd, o->backScreen);
    unsigned input = device1730.Read();
	unsigned output = device1730.ReadOutput(); 
	TL::foreach<InputBitsTable::items_list, __update__>()(&Singleton<InputBitsTable>::Instance().items, &__update_data_x__(40, g, 0xff0000ff, input));
	TL::foreach<OutputBitsTable::items_list, __update__>()(&Singleton<OutputBitsTable>::Instance().items, &__update_data_x__( o->width / 2, g, 0xffff0000, output));
}
}
//--------------------------------------------------------------------------------------
IOportsViewer::IOportsViewer(HWND &h, Bitmap *&bs)
	: hWnd(h)
	, backScreen(bs)
	, hTimer(NULL)
{}
//----------------------------------------------------------------------------------------
void IOportsViewer::Size(Graphics &g, int width_, int height)
{
	width = width_;
	g.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), 0, 0, width, height);
	unsigned input = device1730.Read();
	unsigned output = device1730.ReadOutput(); 
	TL::foreach<InputBitsTable::items_list, __draw__>()(&Singleton<InputBitsTable>::Instance().items, &__draw_data__(40, g, 0xff0000ff, input));
	TL::foreach<OutputBitsTable::items_list, __draw__>()(&Singleton<OutputBitsTable>::Instance().items, &__draw_data__( width / 2, g, 0xffff0000, output));
}
//---------------------------------------------------------------------------------------
void IOportsViewer::Start()
{
	lastTitle = !app.measurementOfRunning;
	CreateTimerQueueTimer(&hTimer, NULL, __Update__, this, 2000, 500, WT_EXECUTEDEFAULT);
}
//------------------------------------------------------------------------------------------
void IOportsViewer::Stop()
{
	if(hTimer)DeleteTimerQueueTimer( NULL, hTimer, NULL );
}
//-------------------------------------------------------------------------------------------------
void IOportsViewer::MouseLDown(TLButtonDown &l)
{
	if(app.measurementOfRunning) return;
	HDCGraphics g(hWnd, backScreen);
	unsigned input = device1730.Read();
	unsigned output = device1730.ReadOutput();
	TL::foreach<InputBitsTable::items_list, __update__>()(&Singleton<InputBitsTable>::Instance().items, &__update_data_x__(40, g, 0xff0000ff, input));
	TL::foreach<OutputBitsTable::items_list, __mouse_down__>()(&Singleton<OutputBitsTable>::Instance().items
		, &__mouse_down_data__( width / 2, g, 0xffff0000, output, l, device1730)
		);
}
//-------------------------------------------------------------------------------------------------

