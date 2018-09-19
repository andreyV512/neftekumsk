#pragma once
#include "WindowsPosition.h"
#include "Dlg.h"
#include "..\DlgTemplates\ParamDlg.hpp"
namespace IOportsWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"����", TopMenu<MainFile>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

	MENU_ITEM(L"�����", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<MainExit>
		>::Result list;
	};
//-----------------------------------------------------------------------------
	struct MainOptionUnits{};
	MENU_TEXT(L"���������", TopMenu<MainOptionUnits>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition : WindowPositionDlg<IOportsWindow>{};

	MENU_ITEM(L"��������� ���������� ����", WindowPosition)

	template<>struct TopMenu<MainOptionUnits>
	{
		typedef TL::MkTlst<
			MenuItem<WindowPosition>
		>::Result list;
	};

	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionUnits>
	>::Result MainMenu;
//..++++++++++++++++++
	struct MainOptionUnits1{};
	MENU_TEXT(L"���������", TopMenu<MainOptionUnits1>)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

