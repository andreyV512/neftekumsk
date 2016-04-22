#pragma once
#include "AppBase.h"
class ThicknessData
{
public:
	ThicknessData();
	static const int MAX_ZONES_COUNT = 60;
	int countZones;
	double zonesMin[MAX_ZONES_COUNT];
	double zonesMax[MAX_ZONES_COUNT];
	char   status[MAX_ZONES_COUNT];
	void Clear();
};

extern ThicknessData thicknessData;
