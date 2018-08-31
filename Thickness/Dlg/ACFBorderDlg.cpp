#include "stdafx.h"
#include "Dlg.h"
#include "..\DlgTemplates\ParamDlg.hpp"
namespace{
MIN_EQUAL_VALUE(ACFBorderLeft<0>, 0)
MAX_EQUAL_VALUE(ACFBorderLeft<0>, 512)
MIN_EQUAL_VALUE(ACFBorderLeft<1>, 0)
MAX_EQUAL_VALUE(ACFBorderLeft<1>, 512)
MIN_EQUAL_VALUE(ACFBorderLeft<2>, 0)
MAX_EQUAL_VALUE(ACFBorderLeft<2>, 512)

MIN_EQUAL_VALUE(ACFBorderRight<0>, 0)
MAX_EQUAL_VALUE(ACFBorderRight<0>, 512)
MIN_EQUAL_VALUE(ACFBorderRight<1>, 0)
MAX_EQUAL_VALUE(ACFBorderRight<1>, 512)
MIN_EQUAL_VALUE(ACFBorderRight<2>, 0)
MAX_EQUAL_VALUE(ACFBorderRight<2>, 512)

PARAM_TITLE(ACFBorderLeft<0>, L"������ 1 ��������� ����� ������� ���")
PARAM_TITLE(ACFBorderRight<0>, L"������ 1 ��������� ������ ������� ���")
PARAM_TITLE(ACFBorderLeft<1>, L"������ 2 ��������� ����� ������� ���")
PARAM_TITLE(ACFBorderRight<1>, L"������ 2 ��������� ������ ������� ���")
PARAM_TITLE(ACFBorderLeft<2>, L"������ 3 ��������� ����� ������� ���")
PARAM_TITLE(ACFBorderRight<2>, L"������ 3 ��������� ������ ������� ���")
}
void ACFBorderDlg::Do(HWND h)
{
	if(TemplDialog<ACFBorderTable>(Singleton<ACFBorderTable>::Instance()).Do(h, L"������� ���"))
	{
	}
}