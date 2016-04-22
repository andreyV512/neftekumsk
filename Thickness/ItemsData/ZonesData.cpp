#include "stdafx.h"
#include <math.h>
#include "ZonesData.h"
#include "AppBase.h"
//---------------------------------------------------------------------------------------------------------
ZonesData::ZonesData()
	//: thresholdClass1(Singleton<ThresholdsTable>::Instance().items.get<NominalThickness>().value)
//	, thresholdMin(Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMin>().value)
	//, thresholdMax(Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value)
{}
//-------------------------------------------------------------------------------------------------------
#if 0
void ThicknessData::Clear()
{
	tresholdClass1  = -1;
	tresholdClass2  = -1;
	countZones = 0;
	for(int i = 0; i < MAX_ZONES_COUNT; ++ i)
	{
		for(int j = 0; j < COUNT_SENSORS; ++j) zones[j][i] = -1;
	}
}
#else
#pragma message("тестовый ThicknessData::Clear()")
void ZonesData::Clear()
{
	countZones = 0;
	ZeroMemory(zones, sizeof(zones));
	ZeroMemory(status, sizeof(status));
}
#endif
//----------------------------------------------------------------------------------------------------------
/*
bool ZonesData::SetZones(int countZones_, double *data)
{
	countZones = countZones_;
	for(int i = 0; i < countZones && i < MAX_ZONES_COUNT; ++i)
	{
		zones[i] = data[i];
	}	
	return true;
}
*/
//-------------------------------------------------------------------------------------------
//ZonesData zonesData;
