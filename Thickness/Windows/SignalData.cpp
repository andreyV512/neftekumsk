#include "stdafx.h"
#include <math.h>
#include "SignalData.h"
//---------------------------------------------------------------------------------------------------------
SignalData::SignalData()
{
	count = 1024;
	double dx = 2 * 3.14 / MAX_ZONES_COUNT * 5;
	for(int i = 0; i < MAX_ZONES_COUNT; ++ i)
	{
		data[i] = sin(i * dx) * 5 + 5;
	}
}
//-------------------------------------------------------------------------------------------------------
#if 0
void SignalData::Clear()
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
void SignalData::Clear()
{
	count = 0;
}
#endif
//----------------------------------------------------------------------------------------------------------
bool SignalData::SetZones(int count_, double *data_)
{
	count = count_;
	if(count > MAX_ZONES_COUNT) count = MAX_ZONES_COUNT;
	for(unsigned i = 0; i < count; ++i)
	{
		data[i] = data_[i];
	}	
	return true;
}
//-------------------------------------------------------------------------------------------
SignalData signalData;
