#pragma once
class ZonesData
{
public:
	static const int MAX_ZONES_COUNT = 2048;
	int countZones;
	double zones[MAX_ZONES_COUNT];
	char status[MAX_ZONES_COUNT];
	int offs[MAX_ZONES_COUNT];
	ZonesData();
	void Clear();
	bool SetZones(int countZones, double *data);
};

//extern ZonesData zonesData;
