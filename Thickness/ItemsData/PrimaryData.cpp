#include "stdafx.h"
#include "PrimaryData.h"
#include "Lan.h"
#include <math.h>
#include "config.h"
#include "DebugMess.h"
char PrimaryData::buffer[PrimaryData::buffSize];
//----------------------------------------------------------------------------------------------
PrimaryData::PrimaryData()
	: bufSize(Lan::bufSize)
	, frameSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, numberPackets(Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value)
{
}
//---------------------------------------------------------------------------------------------
void PrimaryData::Clear()
{
	current__ = 0;
	startOffset = 0;
	ZeroMemory(status, sizeof(status));
	ZeroMemory(result, sizeof(result));
}
//---------------------------------------------------------------------------------------------
char *PrimaryData::CurrentData()
{
	return &buffer[current__ * bufSize];
}
unsigned &PrimaryData::GetRealOffsetTime()
{
	return realOffsetTime[current__];
}
//---------------------------------------------------------------------------------------------
bool PrimaryData::IncrementOffset()
{
	if(((2 + current__) * bufSize) < buffSize)
	{
		++current__;
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------------------
char *PrimaryData::SensorData(int sensor, int offset)
{
	return &buffer[((offset) * count_sensors + sensor) * frameSize];
}
//-------------------------------------------------------------------------------------
int PrimaryData::GetCurrentOffset() const
{
	return current__ * numberPackets;
}
//-------------------------------------------------------------------------------------
void PrimaryData::SetCurrentOffset(int d)
{
	int bufSize = numberPackets 
	   * Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value * count_sensors;
	current__ = d / bufSize;
}
//--------------------------------------------------------------------------------------
int PrimaryData::Filling()
{
  return current__ * numberPackets
	   * Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value * count_sensors;
}
//----------------------------------------------------------------------------------------
