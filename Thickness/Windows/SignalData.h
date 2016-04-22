#pragma once
class SignalData
{
public:
	static const int MAX_ZONES_COUNT = 1024;
	unsigned count;
	double data[MAX_ZONES_COUNT];
	SignalData();
	void Clear();
	bool SetZones(int count_, double *data);
};

//extern SignalData signalData;
