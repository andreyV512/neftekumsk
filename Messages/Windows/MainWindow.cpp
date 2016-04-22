#include "stdafx.h"
#include "MainWindow.h"
#include "MenuApi.h"
#include "EmptyWindow.h"
#include "LogBuffer.h"
#include "MainWindowMenu.hpp"

#include "DebugMess.h"
//------------------------------------------------------------------------
MainWindow::MainWindow()
{
	lastIndex = -1;
}
//---------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	RECT r;
	GetClientRect(m.hwnd, &r);
	MoveWindow(mainWindowGrid.hWnd, 0,  0, r.right, r.bottom, TRUE);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//-------------------------------------------------------------------------------------------
unsigned MainWindow::operator()(TNotify &m)
{
	NotifyHandler *x = (NotifyHandler *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(0 == IsBadReadPtr(x, sizeof(x)))return (*x)(m);
	return 0;
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 400;
		m.pMinMaxInfo->ptMinTrackSize.y = 300;
		m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
		m.pMinMaxInfo->ptMaxTrackSize.y = 500;		
	}		
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);
	mainWindowGrid.Init(m.hwnd);
	SetTimer(m.hwnd,             // хэндл главного окна
		IDT_TIMER1,            // идентификатор таймера
		300,                 // интервал - 1 секунд
		(TIMERPROC) NULL);     // процедуры таймера нет
	return 0;
}
//-------------------------------------------------------------------------
void MainWindow::operator()(TKeyDown &l)
{
	dprint(__FUNCTION__);
}
//-------------------------------------------------------------------------
void MainWindow::operator()(TRButtonDown &l)
{
  dprint(__FUNCTION__);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TDestroy &m)
{
#if 0
	dprint("TDestroy");
	DestroyGlobalData();
	Sleep(1000);
#endif
	KillTimer(m.hwnd, IDT_TIMER1);
	PostQuitMessage(0);
}
//--------------------------------------------------------------------------
//void MainWindow::BottomLabel(int n, wchar_t *text)
//{
//	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
//}
//---------------------------------------------------------------------------
typedef void(*TptrMess)(void *);
void MainWindow::operator()(TMessage &m)
{
	//if(m.wParam)((TptrMess )(m.wParam))((void *)m.lParam);
	dprint(__FUNCTION__);
}
//------------------------------------------------------------------------------
void MainWindow::operator()(TTimer &m)
{
	switch(m.ID)
	{
	case IDT_TIMER1:
		{
			unsigned last = Log::LastMessageIndex();
			if(lastIndex == last) break;
			lastIndex = last;
			int i = ListView_GetTopIndex(mainWindowGrid.hWnd);
			int count = i + ListView_GetCountPerPage(mainWindowGrid.hWnd);
			for(int z = i; z < count; ++z)
			{
				ListView_Update(mainWindowGrid.hWnd, z);
			}
		}
		break;
	}
}

