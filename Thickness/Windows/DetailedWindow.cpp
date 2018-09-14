#include "stdafx.h"
#include <windowsx.h>
#include "Compute/Compute.h"
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
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	RECT st;
	GetClientRect(hStatusWindow, &st);	
	int dy = 30;
	RECT r;
	GetClientRect(m.hwnd, &r);	
	TL::foreach<sensors_list, __detailed_window_size__>()(&sensorsWindow
		, &__detailed_window_size_data__(dy, (r.bottom - st.bottom - dy) / TL::Length<sensors_list>::value + 1, r.right)
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
	sprintf(buf,  "<ff>����� \"2 ������ %.2f\"  <ff>�����\"3 ������<ffff00>%.2f\" <ff>�����\"����<ff0000>%.2f\""
		, Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value
		, Singleton<ThresholdsTable>::Instance().items.get<Border3Class>().value
		, Singleton<ThresholdsTable>::Instance().items.get<BorderDefect>().value
		);
	topLabelViewer.label = buf;
	topLabelViewer.label.fontHeight = 15;
	
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);
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
		WindowTemplate(&x, L"�������� ��������"
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

	wchar_t txt[128];
	wsprintf(txt, L"%d %d %d"
		, int(100.0 * compute.goodData[0] / compute.allData[0])
		, int(100.0 * compute.goodData[1] / compute.allData[1])
		, int(100.0 * compute.goodData[2] / compute.allData[2])
	);
	//wchar_t txt[128];
	//mbstowcs(txt, buf, 1 + strlen(buf));
	SendMessage(hStatusWindow, SB_SETTEXT, 0, (LONG)txt);
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
