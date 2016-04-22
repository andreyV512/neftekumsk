#include "stdafx.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "Emptywindow.h"
#include "CrossData.h"
#include "AppBase.h"
#include "SaveSizeWindow.h"

extern HINSTANCE hInstance;

App app;
//---------------------------------------------------------------
int DebugSensorCrossCount = 12;
//----------------------------------------------------------------
App::App()
	: sensorCrossCount(DebugSensorCrossCount)
	, crossData(Singleton<CrossData>::Instance())
	, crossViewerData(Singleton<CrossViewerData>::Instance())
	, mainWindow(Singleton<MainWindow>::Instance())
{}

void App::Init()
{
	zprint("%s %s", __DATE__, __TIME__);
	AppBase().Init();
    RECT r;
	SizeWindow().Get(mainWindow, r);
	HWND h = WindowTemplate(&mainWindow, L"MainWindow", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
}

void App::Destroy()
{
	zprint("");
}

void App::MainWindowTopLabel(wchar_t *txt)
{
	mainWindow.oTopLabelViewer.SetMessage(txt);
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}

