#include "stdafx.h"
#include "Dlg.h"
#include "..\DlgTemplates\ParamDlg.hpp"

namespace{
template<>struct allowable_limit_on_the_parameter<MedianFilter>
{
	typedef MedianFilter T;
	bool operator()(T::type_value &t, HWND h)
	{
		if(t < 2)
		{
			unsigned res = MessageBox(h, L"Медианный фильтр будет отключен", L"Сообщение", MB_ICONEXCLAMATION | MB_YESNO);
			if(res == IDYES)
			{
				t = 0;
				return true;
			}
			return false;
		}
		if(0 ==(t & 1))
		{
			unsigned res = MessageBox(h, L"Ширина медианного фильтра должна быть нечётной", L"Сообщение", MB_ICONEXCLAMATION | MB_YESNO);
			if(res == IDYES)
			{
				++t;
				return true;
			}
			return false;
		}
		if(t < 31) return true;
		MessageBox(h, L"Параметр \"Ширина фильтра\" должен меньше 30", L"Ошибка!!!", MB_ICONEXCLAMATION);
		return false;
	}	
};
PARAM_TITLE(MedianFilter, L"Ширина фильтра")
}



void MedianFiltreDlg::Do(HWND h)
{
	if(TemplDialog<MedianFilterTable>(Singleton<MedianFilterTable>::Instance()).Do(h, L"Медианный фильтр"))
	{
	}
}

