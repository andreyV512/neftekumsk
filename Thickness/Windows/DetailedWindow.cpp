#include "stdafx.h"
#include <windowsx.h>
#include "DetailedWindow.h"
#include <typeinfo.h>
#include "SensorViewer.h"
#include "typelist.hpp"
#include "WindowsEventTemplate.hpp"
#include "EmptyWindow.h"
#include "MenuApi.h"
#include "DetailedWindowMenu.hpp"
#include "WindowsPosition.h"

#include "DebugMess.h"
//-------------------------------------------------------------------------
DetailedWindow::DetailedWindow() : sensorsWindow(topLabelViewer), hWnd(NULL){}
//-----------------------------------------------------------------------------
struct __detailed_window_size_data__
{
	int y;
	int dHeight;
	int width;
	__detailed_window_size_data__(int y_, int heigth_, int width_) 
		: y(y_), dHeight(heigth_), width(width_){}
};
template<class O, class P>struct __detailed_window_size__
{
	void operator()(O *o, P *p)
	{
       MoveWindow(o->hWnd , 0, p->y, p->width, p->dHeight, true);
	   p->y += p->dHeight;
	}
};
void DetailedWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	int dy = 30;
	RECT r;
	GetClientRect(m.hwnd, &r);	
	TL::foreach<sensors_list, __detailed_window_size__>()(&sensorsWindow
		, &__detailed_window_size_data__(dy, (r.bottom - dy) / TL::Length<sensors_list>::value + 1, r.right)
		);
	 MoveWindow(topLabelViewer.hWnd , 0, 0, r.right, dy, true);
}
//-------------------------------------------------------------------------
template<class O, class P>struct __detailed_window_create__
{
	void operator()(O *o, P *p)
	{		
		o->hWnd = CreateChildWindow(*p, (WNDPROC)&Viewer<SensorViewer>::Proc, L"SensorViewer", o);
	}
};
unsigned DetailedWindow::operator()(TCreate &m)
{
	Menu<DetailedWindowMenu::MainMenu>().Init(m.hwnd);
	TL::foreach<sensors_list, __detailed_window_create__>()(&sensorsWindow, &m.hwnd);
	topLabelViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<TopLabelViewer>::Proc, L"TopLabelWindow", &topLabelViewer);
	char buf[128];
	sprintf(buf,  "<ff>Порог \"1 класса %.2f\"  <ff>Порог\"2 класса<ff0000>%.2f\""
		, Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value
		, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect>().value
		//, Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value
		//, Singleton<ThresholdsTable>::Instance().items.get<DefectDifferentWallPercent>().value
		);
	topLabelViewer.label = buf;
	topLabelViewer.label.fontHeight = 15;
	return 0;
}
//-------------------------------------------------------------------------
void DetailedWindow::operator()(TMessage &m)
{
}
//-------------------------------------------------------------------------
void DetailedWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 500;
		RepaintWindow(m.hwnd);
	}	
}
//-------------------------------------------------------------------------
void DetailedWindow::operator()(TDestroy &m)
{
	hWnd = 0;
}
//-----------------------------------------------------------------------------
template<class O, class P>struct __detailed_window_repaint__
{
	void operator()(O *o, P *)
	{
       o->Repaint();
	}
};
void DetailedWindow::Open()
{
	DetailedWindow &x = Instance();
	if(NULL == x.hWnd)
	{
		RECT r;
		WindowPosition::Get<DetailedWindow>(r);
		WindowTemplate(&x, L"Просмотр датчиков"
			, r.left
			, r.top
			, r.right
			, r.bottom
			);
	}
	x.Update();
}
//------------------------------------------------------------------------------------------------------
void DetailedWindow::Update()
{
	if(NULL != hWnd)
	{
		TL::foreach<sensors_list, __detailed_window_repaint__>()(&sensorsWindow, (int *)0);
	}
	SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	SetForegroundWindow(hWnd);
}
//------------------------------------------------------------------------------
DetailedWindow &DetailedWindow::Instance()
{
	static DetailedWindow x; return x;
}
//------------------------------------------------------------------------------
void DetailedWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
	dprint("TCommand %s", __FUNCTION__);
}
//------------------------------------------------------------------------
template<class O, class P>struct __detailedWindow_mouse_well__
{
	void operator()(O *o, P *p)
	{
		p->hwnd = o->hWnd;
		SendMessage(MESSAGE(*p));
	}
};

void DetailedWindow::operator()(TKeyDown &l)
{
	TL::foreach<sensors_list, __detailedWindow_mouse_well__>()(&sensorsWindow, &l);
}
//------------------------------------------------------------------------------------
void DetailedWindow::operator()(TMouseWell &l)
{
	RECT r;
	GetWindowRect(l.hwnd, &r);
	if(InRect(l.x, l.y, r))
	{
		TL::foreach<sensors_list, __detailedWindow_mouse_well__>()(&sensorsWindow, &l);
	}
}
//--------------------------------------------------------------------------------------------
