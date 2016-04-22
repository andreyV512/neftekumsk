#include "stdafx.h"
#include "DebugMess.h"
#include "MainWindow.h"
#include "Emptywindow.h"
//#include "SaveSizeWindow.h"
#include "AppBase.h"
#include "MSBase/MSBase.hpp"
#include "Base.hpp"
#include "MSBase/TestMS.h"
#include "ArchiveEvents.h"
#include "TestLanBuffer.h"
#include "EmulLan.h"

extern HINSTANCE hInstance;

App app;
//---------------------------------------------------------------
int DebugSensorCrossCount = 12;
//----------------------------------------------------------------
App::App()
//	: mainWindow(Singleton<MainWindow>::Instance())
{}

void App::Init()
{
	//defectoscope.udl
	CoInitializeEx(NULL,COINIT_MULTITHREADED);
	mapped.Init();
	emulLan.Init();
	//archiveEvents;
	zprint("%s %s", __DATE__, __TIME__);
	TestMS();
    RECT r;
	SizeWindow().Get(mainWindow, r);
	HWND h = WindowTemplate(&mainWindow, L"Emulator", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
}

void App::Destroy()
{
	CoUninitialize();
	emulLan.Destroy();
	mapped.Destroy();
	zprint("");
}

void App::MainWindowTopLabel(wchar_t *txt)
{
	//mainWindow.oTopLabelViewer.SetMessage(txt);
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}
#ifdef DEBUG_ITEMS
Mapped mapped;
EmulLan emulLan;
#endif
ArchiveEvents archiveEvents;
MainWindow mainWindow;

