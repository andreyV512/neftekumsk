#include "stdafx.h"
#include "MainWindow.h"
#include "MenuApi.h"
#include "DebugMess.h"
#include "EmptyWindow.h"

using namespace Gdiplus;
//------------------------------------------------------------------------
MainWindow::MainWindow()
	: forwardCheckBox(oEmulViewer.forward)
	, backwardCheckBox(oEmulViewer.backward)
{}
//--------------------------------------------------------------------------
void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();
	RECT rt, rs;
	GetClientRect(toolBar.hWnd, &rt);
	GetClientRect(hStatusWindow, &rs);	
	MoveWindow(oEmulViewer.hWnd , 0, rt.bottom, m.Width, m.Height -  rt.bottom - rs.bottom, true);

	static const int width = toolBar.Width();
	forwardCheckBox.Size(width, 52, 180, 17);
	backwardCheckBox.Size(width, 69, 180, 17);
	//testCheckBox2.Size(width, 69, 525, 20);
	inverterOk    .Size(width + 190, 0, 220, 17);
	controlCircuit.Size(width + 190, 17, 220, 17);
	signalWork	  .Size(width + 190, 34, 220, 17);
	signalSycle	  .Size(width + 190, 51, 220, 17);
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
		m.pMinMaxInfo->ptMaxTrackSize.x = 5000;
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

	CREATE_CHILD_WINDOW(m.hwnd, EmulViewer);

	forwardCheckBox.Init(toolBar.hWnd, L"Рольганг вперёд");
	backwardCheckBox.Init(toolBar.hWnd, L"Рольганг назад");

	unsigned t = oEmulViewer.inputPort;
	inverterOk    .initValue = 0 != (t & (1 << inverterOk.bit));
	controlCircuit.initValue = 0 != (t & (1 << controlCircuit.bit));
	signalWork    .initValue = 0 != (t & (1 << signalWork.bit));
	signalSycle   .initValue = 0 != (t & (1 << signalSycle.bit));

	inverterOk    .Init(toolBar.hWnd, L"Частотный преобразователь");
	controlCircuit.Init(toolBar.hWnd, L"Цепи управления");
	signalWork    .Init(toolBar.hWnd, L"Работа");
	signalSycle   .Init(toolBar.hWnd, L"Цикл");
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

