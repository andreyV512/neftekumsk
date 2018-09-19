#pragma once
#include "config.h"
#include "Dlg.h"
#include "WindowsPosition.h"
namespace MainWindowMenu
{
struct MainFile{};
struct MainOptionUnits{};
struct SaveCoordinateWindow: StoredWindowPosition<MessWindow>{}; 

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		 MenuItem<SaveDateFile>
		, Separator<0>		
		 , MenuItem<MainExit>
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<	
		MenuItem<SaveCoordinateWindow>
	>::Result list;
};




typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;
//--------------------------------------------------------------------------------------------------------
MENU_TEXT(L"����", TopMenu<MainFile>)	
MENU_TEXT(L"���������", TopMenu<MainOptionUnits>)

MENU_ITEM(L"�����", MainExit)
MENU_ITEM(L"��������� ���������� ����", SaveCoordinateWindow)
MENU_ITEM(L"��������� ���������", SaveDateFile)
}
