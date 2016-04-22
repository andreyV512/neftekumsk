#pragma once
#include "config.h"
#include "Dlg.h"
namespace MainWindowMenu
{
struct MainFile{};
struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

struct MainOptionTypeSize{static void Do(HWND){zprint("");};};
struct MainThresholdOption{};
struct MainCreateTypesize{static void Do(HWND){zprint("");};};
struct MainDeleteTypeSize{static void Do(HWND){zprint("");};};

struct MainOptionUnits{};
struct OptionsCoefficients{};
struct AdditionalParametersTypesize{};
struct InputsOutputs{};
struct MainAbout{static void Do(HWND){zprint("");};};

struct LoadDateFile{static void Do(HWND){zprint("");};};
struct SaveDateFile{static void Do(HWND){zprint("");};};
struct Compute{static void Do(HWND){zprint("");};};
struct MedianFiltreDlg{static void Do(HWND){zprint("");};};
struct DeadAreaDlg{static void Do(HWND){zprint("");};};
struct ACFBorderDlg{static void Do(HWND){zprint("");};};
struct TresholdWindow: TestDlg{};
struct StoredData{static void Do(HWND){zprint("");};};
struct Descriptor1730Dlg{static void Do(HWND){zprint("");};};
struct InputsDlg{static void Do(HWND){zprint("");};};
struct OutputsDlg{static void Do(HWND){zprint("");};};
struct SolenoidParametersTableDlg{static void Do(HWND){zprint("");};};
struct CoordinatesDlg: StoredSizeMainWindow{};
struct L502ParamDlg{static void Do(HWND){zprint("");};};
struct ReferencePointsDlg{static void Do(HWND){zprint("");};};

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

template<>struct SubMenu<AdditionalParametersTypesize>
{
	typedef TL::TypeToTypeLst<
		typename TL::MkTlst<MedianFiltreDlg, DeadAreaDlg, ACFBorderDlg>::Result
		, MenuItem
	>::Result list;
};

template<>struct TopMenu<MainOptionTypeSize>
{
	typedef TL::MkTlst<	
	 MenuItem<TresholdWindow>
		 , Separator<0>
		 , MenuItem<MainCreateTypesize>
		, MenuItem<MainDeleteTypeSize>
		, Separator<1>
		, MenuItem<StoredData>
	>::Result list;
};

template<>struct SubMenu<InputsOutputs>
{
	typedef TL::TypeToTypeLst<
		typename TL::MkTlst<Descriptor1730Dlg, InputsDlg, OutputsDlg>::Result 
		, MenuItem
	>::Result list;
};
struct CommunicationOptions{};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<	
#ifndef DISABLE_TCP
		MenuItem<CommunicationOptions>,
#endif
		 MenuItem<SolenoidParametersTableDlg>
		, Separator<0>
		, MenuItem<CoordinatesDlg>
	>::Result list;
};



template<>struct TopMenu<MainAbout>
{
	typedef TL::MkTlst< 
		 MenuItem<MainAbout>
	>::Result list;
};
typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionTypeSize>
	, TopMenu<MainOptionUnits>
	, TopMenu<MainAbout>
>::Result MainMenu;
//--------------------------------------------------------------------------------------------------------
MENU_ITEM(L"Перерасчёт", Compute)
//--------------------------------------------------------------------------------------------------------

    MENU_TEXT(L"Файл", TopMenu<MainFile>)
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)
	MENU_TEXT(L"О программе", TopMenu<MainAbout>)

	//MENU_ITEM(L"Сохранить данные", MenuItem<SaveDateFile>)
	MENU_ITEM(L"Сохранить данные", SaveDateFile)
	MENU_ITEM(L"Загрузить данные", LoadDateFile)
	MENU_ITEM(L"Выход", MainExit)
	 
  //  MENU_TEXT(L"Дополнительные параметры" , SubMenu<AdditionalParametersTypesize>)

	MENU_ITEM(L"Медианный фильтр" , MedianFiltreDlg)
	MENU_ITEM(L"Границы АЧХ" , ACFBorderDlg)
	MENU_ITEM(L"Мёртвые зоны" , DeadAreaDlg)
	MENU_ITEM(L"Границы достоверности" , MainThresholdOption)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Обнулить счётчик труб", StoredData)
	
	MENU_ITEM(L"Настройка аналоговой платы", L502ParamDlg)	
	MENU_ITEM(L"Настройка сетевого подключения", CommunicationOptions)	
	MENU_ITEM(L"Настройки генератора", SolenoidParametersTableDlg)

	MENU_ITEM(L"Коэффициенты перерасчёта", OptionsCoefficients)	
	MENU_ITEM(L"Пороги опорного сигнала", ReferencePointsDlg)	
	MENU_ITEM(L"Добавить образец", TresholdWindow)
	MENU_ITEM(L"Сохранить координаты окна", CoordinatesDlg)
	
	MENU_ITEM(L"О программе", MainAbout)
}
