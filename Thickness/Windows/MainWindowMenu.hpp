#pragma once
#include "config.h"
#include "Dlg.h"
#include "WindowsPosition.h"
namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile: LoadData{};
	struct SaveDateFile: StoredData{};
	struct Compute     : Recalculation{};
	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

	MENU_ITEM(L"Загрузить данные", LoadDateFile)
	MENU_ITEM(L"Сохранить данные", SaveDateFile)
	MENU_ITEM(L"Перерасчёт", Compute)
	MENU_ITEM(L"Выход", MainExit)

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
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct ThicknessTreshold : ThicknessDlg{};
	struct DeadZones         : DeadAreaDlg{};
	struct AllowableThickness: BorderCredibilityDlg{};
	struct RotationalSpeed   : RotationalSpeedDlg{};

	struct MainCreateTypesize: SelectTypeSizeNewDlg{};
	struct MainDeleteTypeSize: SelectTypeSizeDeleteDlg{};
	struct WindowPosition: StoredWindowPosition<MainWindow>{};
	struct Axes: AxesTableDlg{};
	
    MENU_ITEM(L"Пороги отбраковки", ThicknessTreshold)
	MENU_ITEM(L"Мёртвые зоны", DeadZones)
	MENU_ITEM(L"Допустимая толщина", AllowableThickness)
	MENU_ITEM(L"Скорость вращения", RotationalSpeed)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Сохранить координаты окна", WindowPosition)
	MENU_ITEM(L"Оси толщины", Axes)
	MENU_ITEM(L"Границы АЧХ", ACFBorderDlg)
	//MENU_ITEM(L"Диаметр трубы", DiameterTubeDlg)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
		//	 MenuItem<DiameterTubeDlg>
		     MenuItem<ThicknessTreshold>
			, MenuItem<DeadZones>
			, MenuItem<AllowableThickness>
			, MenuItem<ACFBorderDlg>
			, MenuItem<RotationalSpeed>
			, MenuItem<Axes>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
			, Separator<1>
			, MenuItem<WindowPosition>
		>::Result list;
	 };
	//-------------------------------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"Установка", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate: AnalogPlateDlg{};
	struct DiscretePlate{};
	struct DiscretePlateInputs: DiscretePlateInputsDlg{};
	struct DiscretePlateOutputs: DiscretePlateOutputsDlg{};
	struct ColorItems: ColorItemsDlg{};
	struct Tcp : TcpDlg{};//{static void Do(HWND h){zprint("");}}; 
	
	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные порты", DiscretePlateInputs)
	MENU_ITEM(L"Выодные порты", DiscretePlateOutputs)
	MENU_ITEM(L"Дискриптор платы", DiscretePlateDescriptorDlg)
	MENU_ITEM(L"Коэффициенты пересчёта", CoefficientDlg)
	MENU_ITEM(L"Ширина медианного фильтра", MedianFiltreDlg)
	MENU_ITEM(L"Аналоговая плата", AnalogPlate)
	MENU_ITEM(L"Параметры сигнала", SignalDlg)
	
	MENU_ITEM(L"Цвета", ColorItems)
	MENU_ITEM(L"Настройка TCP", Tcp)
	
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
			, MenuItem<MedianFiltreDlg>
			, MenuItem<SignalDlg>
			
			, MenuItem<ColorItems>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout: AboutWindowDlg{};//{static void Do(HWND){zprint("");}};
	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"О программе", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	// --------------------------------------------------------------------------------------------------------- 
	/// \brief	Добавить меню
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
