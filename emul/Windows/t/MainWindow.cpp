#include "stdafx.h"
#include "MainWindow.h"
#include "MenuApi.h"
#include "EmptyWindow.h"
#include "Chart.h"
#include "Chart.hpp"

#include "DebugMess.h"
using namespace Gdiplus;
//------------------------------------------------------------------------
MainWindow::MainWindow()
	: backScreen(NULL)
{
}
//-----------------------------------------------------------------------------
void MainWindow::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
}
//--------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < m.Width || backScreen->GetHeight() < m.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(m.Width, m.Height);
		}
	}
	else if(m.Width > 0 && m.Height > 0)
	{
		backScreen = new Bitmap(m.Width, m.Height);
	}
	else
	{
		return;
	}
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	RECT sr, rb;
	GetClientRect(hStatusWindow, &sr);
	GetClientRect(toolBar.hWnd, &rb);

	Graphics g(backScreen);
	SolidBrush solidBrush(Color(0xffaaaaaa));
	g.FillRectangle(&solidBrush, 0, rb.bottom, m.Width, m.Height - sr.bottom - rb.bottom);   
}
//------------------------------------------------------------------------
void MainWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//------------------------------------------------------------------------
void MainWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 300;
		m.pMinMaxInfo->ptMaxTrackSize.x = 2000;
		m.pMinMaxInfo->ptMaxTrackSize.y = 500;		
	}		
}
//------------------------------------------------------------------------
unsigned MainWindow::operator()(TCreate &m)
{
	toolBar.Init(m.hwnd);
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);
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
void MainWindow::operator()(TDestroy &)
{
#if 0
	dprint("TDestroy");
	DestroyGlobalData();
	Sleep(1000);
#endif
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

