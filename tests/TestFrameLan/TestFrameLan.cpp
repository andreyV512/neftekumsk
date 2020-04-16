// TestFrameLan.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "templates\typelist.hpp"

class ThicknessData
{
public:
	static const int MAX_ZONES_COUNT = 60;
	int countZones;
	double zonesMin[MAX_ZONES_COUNT];
	double zonesMax[MAX_ZONES_COUNT];
	char   status[MAX_ZONES_COUNT];
};


struct HeadStoredData
{
	int countZones;
	int countSensors;
	int trimOffset;
	unsigned frameStartTime;
};

struct close_file
{
	FILE *f;
	close_file(FILE *f) : f(f){}
	~close_file(){if(NULL != f)fclose(f);}
};

class PrimaryData
{
public:
	int current__;
	
public:

	template<int NUM>struct Mess{static const int value = NUM;};	

	static const unsigned DeathZone              = 1 << 0;
	static const unsigned Defect                 = 1 << 1;
	static const unsigned Treshold3Class         = 1 << 2;
	static const unsigned Treshold2Class         = 1 << 3;
	static const unsigned Nominal                = 1 << 4;


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

	static const int count_sensors = 3;

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
};

//PrimaryData primaryData;

int countZones;
unsigned indexOffsetZone[ThicknessData::MAX_ZONES_COUNT] = {};
unsigned realOffsetTime[1000] = {};
unsigned frameStartTime;

static const int buffSize = 512 * 1024 * 1024;
static char buffer[buffSize];

void Load(wchar_t *name)
{
//	OpenData o(h);
	//if(o())
	{
		FILE *f = _wfopen(name, L"rb");
		close_file c_f(f);
		if(NULL != f)
		{
			HeadStoredData head;
			if(1 == fread(&head, sizeof(head), 1, f))
			{
				countZones = head.countZones;
				fread(realOffsetTime, sizeof(realOffsetTime), 1, f);
				int count = fread(indexOffsetZone, sizeof(indexOffsetZone), 1, f);
				frameStartTime = head.frameStartTime;
				char buf[1024 * 16];
				int k = 0;
				for(;;)
				{
					int count = fread(buf, 1, sizeof(buf), f);
					if(0 == count) break;
					{
						for(int i = 0; i < count; ++i)
						{
							buffer[k++] = buf[i];
						}
					}
				}
				
			}
		}
		for(int i = 1; i < 1000; ++i)
		{
			double t = ((unsigned)realOffsetTime[i] - (unsigned)realOffsetTime[i - 1]);
			printf("%d %f %d\n", i, t, realOffsetTime[i] );
		}
	   getchar();
	}	
}

int _tmain(int argc, _TCHAR* argv[])
{
	//G:\Projects\Work\neftekumsk\data\ЛДФ ---
	Load(L"G:\\Projects\\Work\\neftekumsk\\data\\ЛДФ ---\\etalon.dat");
	getchar();
	return 0;
}

