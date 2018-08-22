#include "stdafx.h"
#include "App.h"
#include <WindowsX.h>
#include "DebugMess.h"
#include "MainWindow.h"
//#include "Emptywindow.h"
//#include "CrossData.h"
//#include "AppBase.h"
//#include "SaveSizeWindow.h"
//#include "Compute.h"
#include "PrimaryData.h"
//#include "ArchiveEvents.h"
//#include "Device1730.h"
//#include "SQEvents.h"
//#include "Automat.h"
//#include "Lan.h"
//#include "AsyncServer.h"
//#include "Config.h"
//D:\Projects\EmptyProject\emul\emulLan\EmulLan.cpp
//D:\Projects\EmptyProject\Thickness\App\App.cpp
//#include "WindowsPosition.h"
//#include "ExpressBase.hpp"
#ifdef DEBUG_ITEMS
#include "../emul/emulLan/EmulLan.h"
#include "../emul/emulLan/TestLanBuffer.h"
#endif

extern HINSTANCE hInstance;

HANDLE App::ProgrammExitEvent;// = L"ProgrammExitEventName";
HANDLE App::hEventStrobes;
App app;
//---------------------------------------------------------------
//int DebugSensorCrossCount = 12;
//----------------------------------------------------------------
MainWindow m;
App::App()
	: mainWindow(m)
{}

void App::Init()
{
	zprint("%s %s", __DATE__, __TIME__);
	ProgrammExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hEventStrobes = CreateEvent(NULL, TRUE, FALSE, NULL);
}

void App::Destroy()
{
	zprint("");
	SetEvent(ProgrammExitEvent);
	Sleep(1000);
	CloseHandle(ProgrammExitEvent);
}

void App::MainWindowTopLabel(wchar_t *txt)
{
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
}

bool App::ChangeTypeSize(wchar_t *txt)
{ 
	return true;
}

PrimaryData primaryData;




