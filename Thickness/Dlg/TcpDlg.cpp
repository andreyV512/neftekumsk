#include "stdafx.h"
#include "Dlg.h"
#include "AppBase.h"
#include "..\DlgTemplates\ParamDlg.hpp"

PARAM_TITLE(IPPort, L"IP порт")
PARAM_TITLE(IPName, L"Имя компьютера")

DO_NOT_CHECK(IPPort)
DO_NOT_CHECK(IPName)

void TcpDlg::Do(HWND h)
{
   if(TemplDialog<IPAddressTable>(Singleton<IPAddressTable>::Instance()).Do(h, L"Настройки TCP"))
	{
#pragma message("Инициализировать TCP")
	}
}