#pragma once
#include "config.h"
#include "Dlg.h"
#include "WindowsPosition.h"
namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"����", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile: LoadData{};
	struct SaveDateFile: StoredData{};
	struct Compute     : Recalculation{};
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

	MENU_ITEM(L"��������� ������", LoadDateFile)
	MENU_ITEM(L"��������� ������", SaveDateFile)
	MENU_ITEM(L"����������", Compute)
	MENU_ITEM(L"�����", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, Separator<0>
			, MenuItem<Compute>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};
	//------------------------------------------------------------------------
	struct MainOptionTypeSize{};
	MENU_TEXT(L"����������", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct ThicknessTreshold : ThicknessDlg{};
	struct DeadZones         : DeadAreaDlg{};
	struct AllowableThickness: BorderCredibilityDlg{};
	struct RotationalSpeed   : RotationalSpeedDlg{};

	struct MainCreateTypesize: SelectTypeSizeNewDlg{};
	struct MainDeleteTypeSize: SelectTypeSizeDeleteDlg{};
	struct WindowPosition: StoredWindowPosition<MainWindow>{};
	struct Axes: AxesTableDlg{};
	
    MENU_ITEM(L"������ ����������", ThicknessTreshold)
	MENU_ITEM(L"̸����� ����", DeadZones)
	MENU_ITEM(L"���������� �������", AllowableThickness)
	MENU_ITEM(L"�������� ��������", RotationalSpeed)
	MENU_ITEM(L"������� ����������", MainCreateTypesize)
	MENU_ITEM(L"������� ����������", MainDeleteTypeSize)
	MENU_ITEM(L"��������� ���������� ����", WindowPosition)
	MENU_ITEM(L"��� �������", Axes)
	MENU_ITEM(L"������� ���", ACFBorderDlg)
	MENU_ITEM(L"������ ���������� �������", MedianFiltreDlg)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
		     MenuItem<ThicknessTreshold>
			, MenuItem<DeadZones>
			, MenuItem<AllowableThickness>
			, MenuItem<ACFBorderDlg>
			, MenuItem<RotationalSpeed>
			, MenuItem<Axes>
			, MenuItem<MedianFiltreDlg>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
			, Separator<1>
			, MenuItem<WindowPosition>
		>::Result list;
	 };
	//-------------------------------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"���������", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate: AnalogPlateDlg{};
	struct DiscretePlate{};
	struct DiscretePlateInputs: DiscretePlateInputsDlg{};
	struct DiscretePlateOutputs: DiscretePlateOutputsDlg{};
	struct ColorItems: ColorItemsDlg{};
	struct Tcp : TcpDlg{};
	struct IOports: IOportsDlg{};
	
	MENU_TEXT(L"���������� �����", SubMenu<DiscretePlate>)
	MENU_ITEM(L"������� �����", DiscretePlateInputs)
	MENU_ITEM(L"������� �����", DiscretePlateOutputs)
	MENU_ITEM(L"���������� �����", DiscretePlateDescriptorDlg)
	MENU_ITEM(L"������������ ���������", CoefficientDlg)
	
	MENU_ITEM(L"���������� �����", AnalogPlate)
	MENU_ITEM(L"��������� �������", SignalDlg)
	
	MENU_ITEM(L"�����", ColorItems)
	MENU_ITEM(L"��������� TCP", Tcp)
	MENU_ITEM(L"�������� ���������� ������", IOports)
	
	template<>struct SubMenu<DiscretePlate>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<DiscretePlateDescriptorDlg, DiscretePlateInputs, DiscretePlateOutputs>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<AnalogPlate>
			, SubMenu<DiscretePlate>
			, MenuItem<Tcp>
			, MenuItem<CoefficientDlg>
			
			, MenuItem<SignalDlg>
			
			, MenuItem<ColorItems>
			, Separator<1>
			, MenuItem<IOports>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout: AboutWindowDlg{};
	MENU_TEXT(L"� ���������", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"� ���������", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	// --------------------------------------------------------------------------------------------------------- 
	/// \brief	�������� ����
	//struct TestMenu{};
	//MENU_TEXT(L"TestMenu", TopMenu<TestMenu>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//struct TestSubMenu0{};
	//struct TestSubMenu1{static void Do(HWND){zprint("");}};
	//struct TestSubMenu2{static void Do(HWND){zprint("");}};
	//struct TestSubMenu3{static void Do(HWND){zprint("");}};
	//struct TestSubMenu4{static void Do(HWND){zprint("");}};
	//
	//MENU_ITEM(L"TestSubMenu0", TestSubMenu0)
	//MENU_ITEM(L"TestSubMenu1", TestSubMenu1)
	//MENU_ITEM(L"TestSubMenu2", TestSubMenu2)
	//MENU_ITEM(L"TestSubMenu3", TestSubMenu3)
	//MENU_ITEM(L"TestSubMenu4", TestSubMenu4)
	//
	//template<>struct TopMenu<TestMenu>
	//{
	//	typedef TL::MkTlst<
	//		MenuItem<TestSubMenu0>
	//		, MenuItem<TestSubMenu1>
	//		, MenuItem<TestSubMenu2>
	//		, MenuItem<TestSubMenu3>
	//		, MenuItem<TestSubMenu4>
	//	>::Result list;
	//};
	//---------------------------------------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<Options>
		, TopMenu<MainAbout>
		//, TopMenu<TestMenu>
	>::Result MainMenu;
	}
