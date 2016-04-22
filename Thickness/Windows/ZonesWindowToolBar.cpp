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