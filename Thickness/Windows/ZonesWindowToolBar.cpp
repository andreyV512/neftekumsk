#include "stdafx.h"
#include "ZonesWindowToolBar.h"
#include "ZonesWindow.h"
#include "DetailedWindow.h"
#include "EmptyWindow.h"
//#include "Automat.h"
#include "DebugMess.h"
#include "PrimaryData.h"
#if 0
void LeftArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor;
	int zone = z.zone - 1;
	if(zone < 0) zone = primaryData.countZones;
	ZonesWindow::Open(sensor, zone);
}
//----------------------------------------------------------------------------
void RightArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor;
	int zone = z.zone + 1;
	if(zone > primaryData.countZones) zone = 0;
	ZonesWindow::Open(sensor, zone);
}
#else
void LeftArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor;
	int zone = z.zone - 2;
	if(zone < 0) zone = primaryData.countZones - 1;
	ZonesWindow::Open(sensor, zone);
}
//----------------------------------------------------------------------------
void RightArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor;
	int zone = z.zone;
	if(0 == thicknessData.status[zone]) zone = 0;
	ZonesWindow::Open(sensor, zone);
}
#endif
//----------------------------------------------------------------------------
void UpArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor - 1;
	int zone = z.zone - 1;
	if(sensor < 0) sensor = count_sensors - 1;
	ZonesWindow::Open(sensor, zone);
}
//----------------------------------------------------------------------------
void DownArrowHandler(HWND h)
{
	ZonesWindow &z = ZonesWindow::Instance();
	int sensor = z.sensor + 1;
	int zone = z.zone - 1;
	if(sensor >= count_sensors) sensor = 0;
	ZonesWindow::Open(sensor, zone);
}
//----------------------------------------------------------------------------
namespace tl
{
	template<class T>struct __tool_btn_width__
	{
		static const int value = 10;
	};
	template<int idb, void(*ptr)(HWND), class z, template<int, void(*)(HWND), class>class W>struct __tool_btn_width__<W<idb, ptr, z> > 
	{
		static const int value = 24;
	};
	template<class List>struct __tool_bar_width__;
	template<class Head, class Tail>struct __tool_bar_width__<Tlst<Head, Tail> >
	{
		static const int value = __tool_btn_width__<Head>::value + __tool_bar_width__<Tail>::value;
	};
	template<>struct __tool_bar_width__<NullType>
	{
		static const int value = 0;
	};	
}
const int DetailiedToolButtonWidth()
{
	return tl::__tool_bar_width__<detailied_tool_button_list>::value;
}