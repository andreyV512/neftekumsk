#include "stdafx.h"
#include "Strobes.h"
#include "PrimaryData.h"
#include "Compute.h"

void Strobes()
{
	unsigned t = primaryData.indexOffsetZone[0];
	for(int i = 0; i < dimention_of(primaryData.indexOffsetZone); ++i)
	{
		primaryData.indexOffsetZone[i] -= t;
	}
	unsigned startTime = primaryData.frameStartTime;
	unsigned numberPackets = Singleton<LanParametersTable>::Instance().items.get<NumberPackets>().value;
	unsigned startSamples = 0;
	int k = 0;
	for(; k < dimention_of(primaryData.realOffsetTime); ++k)
	{
		if(startTime < primaryData.realOffsetTime[k]) break; 
	}

	double (&offsetOfSamples)[ThicknessData::MAX_ZONES_COUNT] = primaryData.offsetOfTime;
	unsigned (&realOffsetTime)[1000] = primaryData.realOffsetTime;

	offsetOfSamples[0] = (double)numberPackets*(k - 1) + 
		(double)numberPackets*(startTime - realOffsetTime[k - 1])/(realOffsetTime[k] - realOffsetTime[k - 1]);

	unsigned baseTime = startTime;
	startTime = baseTime + primaryData.indexOffsetZone[1];
	int index = 1;
	for(; k < dimention_of(primaryData.realOffsetTime); ++k)
	{
		if(startTime < primaryData.realOffsetTime[k])
		{
			int t = startTime - realOffsetTime[k - 1];
			if(t < 0)
			{
				dprint("err\n");
			}
			offsetOfSamples[index] = (double)numberPackets*(k - 1) + 
				(double)numberPackets* t /(realOffsetTime[k] - realOffsetTime[k - 1]);

			++index;
			if(index >= primaryData.countZones) break;
			
			startTime = baseTime + primaryData.indexOffsetZone[index];
			while(int(startTime - realOffsetTime[k]) < 0)
			{
				++index;
				if(index >= primaryData.countZones) break;

				startTime = baseTime + primaryData.indexOffsetZone[index];
			}
		}
	}
}