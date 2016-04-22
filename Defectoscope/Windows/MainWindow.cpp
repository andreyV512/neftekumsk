#include "stdafx.h"
#include "MainWindow.h"
#include "MenuApi.h"
#include "MainWindowMenu.hpp"
#include "EmptyWindow.h"

#include "DebugMess.h"
//------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	RECT rt;
	GetWindowRect(toolBar.hWnd, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	

	static const int width = toolBar.Width();
	select.Size(width, 5, 400);

	testCheckBox0.Size(width + 525, 52, 200, 20);
	testCheckBox1.Size(width, 52, 525, 20);
	testCheckBox2.Size(width, 69, 525, 20);

	static const int topLabelHeight = 28;
	int y = rt.bottom - rt.top - 1;
	int t = (r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight);
	MoveWindow(oTopLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	MoveWindow(oCrossViewer.hWnd , 0, y, r.right, t, true);
	oTopLabelViewer.SetMessage(L"<ff>TopLabel");
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
	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);

	toolBar.Init(m.hwnd);
	select.Create(toolBar.hWnd);

	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {550,900, 3000};
	SendMessage(hStatusWindow, SB_SETPARTS, 3, (LPARAM)pParts);

	CREATE_CHILD_WINDOW(m.hwnd, TopLabelViewer);
	oTopLabelViewer.label.fontHeight = 16;
	CREATE_CHILD_WINDOW(m.hwnd, CrossViewer);
	//test
	testCheckBox0.Init(toolBar.hWnd, L"test check box 0");
	testCheckBox1.Init(toolBar.hWnd, L"test check box 1");
	testCheckBox2.Init(toolBar.hWnd, L"test check box 2");
	//test end
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

