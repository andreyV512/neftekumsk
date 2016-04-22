#include "stdafx.h"
#include <WindowsX.h>
#include "DebugMess.h"
#include "MainWindow.h"
#include "Emptywindow.h"
#include "CrossData.h"
#include "AppBase.h"
//#include "SaveSizeWindow.h"
#include "Compute.h"
#include "PrimaryData.h"
#include "ArchiveEvents.h"
#include "Device1730.h"
#include "SQEvents.h"
#include "Automat.h"
#include "Lan.h"
#include "AsyncServer.h"
#include "Config.h"
//D:\Projects\EmptyProject\emul\emulLan\EmulLan.cpp
//D:\Projects\EmptyProject\Thickness\App\App.cpp
#include "WindowsPosition.h"
#include "ExpressBase.hpp"
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
App::App()
	//: sensorCrossCount(DebugSensorCrossCount)
	//, crossData(Singleton<CrossData>::Instance())
	//, crossViewerData(Singleton<CrossViewerData>::Instance())
	: mainWindow(Singleton<MainWindow>::Instance())
{}

void App::Init()
{
	zprint("%s %s", __DATE__, __TIME__);
	ProgrammExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	hEventStrobes = CreateEvent(NULL, FALSE, FALSE, NULL);

#ifdef DEBUG_ITEMS
	mapped.Init();
	emulLan.Init();
#endif

	bool initOk = true;
	AppBase().Init();
	compute.InitParam();
	int num = Singleton<NamePlate1730ParametersTable>::Instance().items.get<NamePlate1730>().value;
	if(device1730.Init(num))
	{
		  sQEvents.Init();
	}
	else
	{
		MessageBox(0, L"Не могу инициировать плату 1730", L"Ошибка !!!", MB_ICONERROR);
		initOk = false;
	}
	dprint("num 1730 %d", num);
	unsigned res = Lan::Init();
	if(0 != res)
	{
		wchar_t buf[256];
		wsprintf(buf, L"Не могу инициировать плату Lan10m8-100 номер %d", res);
		MessageBox(0, buf, L"Ошибка !!!", MB_ICONERROR);
		initOk = false;
	}

    RECT r;
	WindowPosition::Get<MainWindow>(r);
	HWND h = WindowTemplate(&mainWindow, L"Толщинометр", r.left, r.top, r.right, r.bottom);
	ShowWindow(h, SW_SHOWNORMAL);
	AsyncServer::Create(Singleton<IPAddressTable>::Instance().items.get<IPPort>().value);
	if(initOk)
	{
		automat.Init();
	}
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
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
	mainWindow.oTopLabelViewer.SetMessage(txt);
}

void App::MainWindowBottomLabel(int n, wchar_t *text)
{
	SendMessage(mainWindow.hStatusWindow, SB_SETTEXT, n, (LONG)text);
}

bool App::ChangeTypeSize(wchar_t *txt)
{ 
	HWND h = mainWindow.select.hWnd;
	wchar_t buf[512];
	ComboBox_GetText(h, buf, 512);
	if(0 == wcscmp(buf, txt)) return true;
	int res = ComboBox_FindString(h, -1, txt);
	if(res != CB_ERR)
	{
		ComboBox_SetCurSel(h, res);
		TCommand c;
		c.hControl = h;
		c.isAcselerator = 1;
        mainWindow.select.Do(c);
		return true;
	}
	return false;
}

#ifdef DEBUG_ITEMS
Mapped mapped;
EmulLan emulLan;
#endif

ArchiveEvents archiveEvents;
Device1730 device1730;
SQEvents sQEvents;
PrimaryData primaryData;
Compute compute(primaryData);
Automat automat;




