#include "stdafx.h"
#include "CommunicationWapper.h"
#include "AppBase.h"
#include "..\Windows\SelectTypeSize.h"
#include "DebugMess.h"
#include "App.h"
#include "Automat.h"
#include "GetHostByName.h"
#include "MainWindow.h"

#include "ThicknessProtocol.h"

bool TcpServerTypeSize(wchar_t *receiveData)
{	
	bool result = app.ChangeTypeSize(receiveData);
	if(result)
	{
		zprint("");
		automat.AutomatMode();
	}
	return result;
}

bool SendThickhess()
{
	for(int i = 0;; ++i)
	{
		if(ThicknessProtocol::Client().Do(
			GetHostByName()(Singleton<IPAddressTable>::Instance().items.get<IPName>().value)
			, Singleton<IPAddressTable>::Instance().items.get<IPPort>().value
			)) return true;
		if(i > 5)
		{
			i = 0;
			if(IDNO == MessageBox(app.mainWindow.hWnd, L"Продолжить?", L"Ошибка передачи типоразмера", MB_ICONINFORMATION | MB_YESNO)) return false;
		}
		Sleep(1000);
	}
	return false;
}