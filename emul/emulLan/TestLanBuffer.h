#pragma once
#ifdef DEBUG_ITEMS
struct TestLanBuffer
{
	int countZones;
	int countSensors;	
	double offsetsTime[400];
	char Frame[986 * 6 * 100];
};

class Mapped
{
public:
	TestLanBuffer *buf;
	void Init(){}
	void Destroy();
};

extern Mapped mapped;
#endif