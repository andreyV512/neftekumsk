#pragma once
#include "AppBase.h"
#include "ThicknessData.h"
class PrimaryData
{
public:
	int current__;
	
public:
	template<int NUM>struct Mess{static const int value = NUM;};	

	static const unsigned DeathZone              = 1 << 0;
	static const unsigned Defect                 = 1 << 1;
	static const unsigned Treshold2Class         = 1 << 2;
	static const unsigned Nominal                = 1 << 3;
	static const unsigned Treshold3Class         = 1 << 4;

	static const unsigned Undefined              = 1 << 5;
	static const unsigned LessMinimumEnergy      = 2 << 5;
	static const unsigned ValueLessThreshold     = 3 << 5;
	static const unsigned ThicknessLessThreshold = 4 << 5;	
	static const unsigned GreaterMaximumEnergy   = 5 << 5;

	


	
	typedef TL::MkTlst<
	    Mess<Defect                >
		, Mess<Treshold2Class        >
		, Mess<Treshold3Class        >
		, Mess<LessMinimumEnergy     >
		, Mess<GreaterMaximumEnergy     >
		, Mess<ValueLessThreshold    >
		, Mess<ThicknessLessThreshold>
		, Mess<Nominal               >
		, Mess<Undefined             >
		, Mess<DeathZone             >
	>::Result message_list;


	static const int buffSize = 512 * 1024 * 1024;
	static const int sensorBuffSize = buffSize / 474 / count_sensors;
	const size_t &bufSize;
	const int &frameSize;
	static char buffer[buffSize];
	double result[count_sensors][sensorBuffSize];//результат вычислений
	char status[count_sensors][sensorBuffSize];//тег вычислений
	int countZones;
	int startOffset;
	const int &numberPackets;

	double offsetOfTime[ThicknessData::MAX_ZONES_COUNT];//временные метки
	unsigned indexOffsetZone[ThicknessData::MAX_ZONES_COUNT];
	unsigned realOffsetTime[1000];
	unsigned frameStartTime;
	unsigned scanerStart, scanerStop;
	PrimaryData();
	short *operator[](int );
	void Clear();
	char *CurrentData();
	unsigned &GetRealOffsetTime();
	bool IncrementOffset();
	char *SensorData(int sensor, int offset);
	int GetCurrentOffset() const;
	void SetCurrentOffset(int);
	int Filling();//заполнение буфера в short
};

extern PrimaryData primaryData;

