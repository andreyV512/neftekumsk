#include "stdafx.h"
#include "Compute.h"
#include <Mmsystem.h>
#include <math.h>
#include "AppBase.h"
#include "ThicknessData.h"
#include "ThicknessViewer.h"
//#include "StoredData.h"
#include "mainwindow.h"
#include "DetailedWindow.h"
#include "SQEvents.h"
#include "PrimaryData.h"
#include "Communication.h"
#include "Strobes.h"

#include "Lan.h"

//#include "Debug1730.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif
//--------------------------------------------------------------------
Compute::Compute(PrimaryData &pd)
	: primaryData(pd)
 //   , referenceOffset1(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset1>().value)
//	, referenceOffset2(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset2>().value)
 //   , referenceOffset3(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset3>().value)
//	, referenceOffset4(Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset4>().value)
	, referenceOffsetSQ3(300)
	, frameSize(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
	, treshold2Class(Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value)
	, treshold3Class(Singleton<ThresholdsTable>::Instance().items.get<Border3Class>().value)
	, tresholdDefect(Singleton<ThresholdsTable>::Instance().items.get<BorderDefect>().value)
	, deadAreaMM0 (Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0>().value)
	, deadAreaMM1(Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1>().value)
//	, lengthTube    (Singleton<ThresholdsTable>::Instance().items.get<LengthTube>().value)




//	, nominalThickness(Singleton<ThresholdsTable>::Instance().items.get<NominalThickness>().value)
//	, nominalPercentMin(Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMin>().value)
	, thicknessData(::thicknessData)
	//, nominalPercentMax(Singleton<ThresholdsTable>::Instance().items.get<NominalPercentMax>().value)
	//, defectDifferentWallPercent(Singleton<ThresholdsTable>::Instance().items.get<DefectDifferentWallPercent>().value)
//	, frequencyFrames(Singleton<AdditionalSettingsTable>::Instance().items.get<FrequencyFrames>().value)
//	, deadAreaMM0(Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM0>().value)
//	, deadAreaMM1(Singleton<DeadAreaTable>::Instance().items.get<DeadAreaMM1>().value)	
	, medianFiltreLength(Singleton<MedianFilterTable>::Instance().items.get<MedianFilter>().value)
	//, acfBorderLeft(Singleton<ACFBorderTable>::Instance().items.get<ACFBorderLeft>().value)
	//, acfBorderRight(Singleton<ACFBorderTable>::Instance().items.get<ACFBorderRight>().value)
{
	InitializeCriticalSection(&cs0);
	InitializeCriticalSection(&cs1);
	hEvent = CreateEvent(NULL, FALSE, FALSE, LAN_DEVICE_FRAME_READY);
	
	hThread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, CREATE_SUSPENDED, NULL);
	hThread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, this, CREATE_SUSPENDED, NULL);	
}
//--------------------------------------------------------------------
void Compute::MeshuringBaseStart()
{
	deltaTimeZoneStart = timeGetTime();
	lastOffset = 0;
	lastZone = 0;
	lastFrame = 0;
	primaryData.Clear();
	thicknessData.Clear();
}
//--------------------------------------------------------------------
typedef TL::CreateNumList<TL::IntToType, count_sensors, 1 + count_sensors>::Result num_list;

template<class O, class P>struct Compute::__init_offset__
{
	void operator()(O *, P *p)
	{
		static const int i = O::value - 1;
		p->coefficientA[i] = Singleton<CoefficientParametersTable>::Instance().items.get<CoefficientA<O::value> >().value;
		p->coefficientB[i] = Singleton<CoefficientParametersTable>::Instance().items.get<CoefficientB<O::value> >().value;

		p->minOffset[i] = unsigned((Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value
		   - p->coefficientB[i]) / p->coefficientA[i]
		);
		
		//double nominalThickness = Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value;
		p->maxOffset[i] = unsigned((Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value
			- p->coefficientB[i]) / p->coefficientA[i]
		);
	}
};
template<int N, class P>struct Compute::__init_energy__<MinEnergy<N>, P>
{
	void operator()(MinEnergy<N> *o, P *p)
	{
		p->minEnergy[N] = o->value;
	}
};
template<int N, class P>struct Compute::__init_energy__<MaxEnergy<N>, P>
{
	void operator()(MaxEnergy<N> *o, P *p)
	{
		p->maxEnergy[N] = o->value;
	}
};
template<int N, class P>struct Compute::__init_energy__<Peak<N>, P>
{
	void operator()(Peak<N> *o, P *p)
	{
		p->peak[N] = o->value;
	}
};

template<int N, class P>struct Compute::__init_left_acf__<ACFBorderLeft<N>, P>
{
	void operator()(ACFBorderLeft<N> *o, P *p)
	{
		p->acfBorderLeft[N] = o->value;
			//Singleton<ACFBorderTable>::Instance().items.get<ACFBorderLeft<N>>().value;
	}
};
template<int N, class P>struct Compute::__init_left_acf__<ACFBorderRight<N>, P>
   {void operator()(ACFBorderRight<N> *, P *){}};
template<int N, class P>struct Compute::__init_right_acf__<ACFBorderRight<N>, P>
	{
	void operator()(ACFBorderRight<N> *o, P *p)
	{
		p->acfBorderRight[N] = o->value;
			//&Singleton<ACFBorderTable>::Instance().items.get<ACFBorderRight<N>>().value;
	}
};
template<int N, class P>struct Compute::__init_right_acf__<ACFBorderLeft<N>, P>
   {void operator()(ACFBorderLeft<N> *, P *){}};

struct Lock
{
	CRITICAL_SECTION &critical;
	Lock(CRITICAL_SECTION &critical)
		: critical(critical)
	{
		EnterCriticalSection(&critical);
	}
    ~Lock()
	{
		LeaveCriticalSection(&critical);
	}
};

void Compute::ThreadProc(Compute *d)
{
	while(true)
	{
		switch(WaitForSingleObjectEx(d->hEvent, 1300, TRUE))
		{
		case WAIT_OBJECT_0:
			d->Calculation();
			break;
		case WAIT_TIMEOUT:
			{
				SuspendThread(GetCurrentThread());
			}
			break;
		case WAIT_IO_COMPLETION: 
			return;
		}
	}
}

void Compute::InitParam()
{
	fft.Init(frameSize);

	TL::foreach<num_list, Compute::__init_offset__>()((TL::Factory<num_list> *)0, this);

	TL::foreach<SignalParametersTable::items_list, Compute::__init_energy__>()(
		&Singleton<SignalParametersTable>::Instance().items
		, this
		);
	TL::foreach<ACFBorderTable::items_list, Compute::__init_left_acf__>()(
		&Singleton<ACFBorderTable>::Instance().items
		, this
		);
	TL::foreach<ACFBorderTable::items_list, Compute::__init_right_acf__>()(
		&Singleton<ACFBorderTable>::Instance().items
		, this
		);
	for(int i = 0; i < dimention_of(filtre); ++i)
	{
		filtre[i].SetLength(medianFiltreLength);
		filtre[i].Clear();
	}
	//defectBorderMin = tresholdDefect;
	//defectBorderMax = treshold2Class;
	//defectBorder3 = treshold3Class;
	
}
//-----------------------------------------------------------------------------------------
void Compute::EndAdjustmentsTube()//коррекция результата с учётом мёртвой зоны в конце трубы
{
	int len = primaryData.countZones + 1;

	if(len > dimention_of(primaryData.offsetOfTime)) len = dimention_of(primaryData.offsetOfTime) - 1;

	SubRecalculation(0, len);
	DeathZoneFront(0);
	DeathZoneBack(len);
	thicknessViewer.Update();
}
//---------------------------------------------------------------------------------------------
void Compute::StopMeshuring()
{
	stopMeshuring = timeGetTime();
}
//---------------------------------------------------------------------------------------
struct LockTry
{
	CRITICAL_SECTION &cs;
	LockTry(CRITICAL_SECTION &cs) : cs(cs) {}
	~LockTry(){LeaveCriticalSection(&cs);}
};
//----------------------------------------------------------------------------------
void SetDataStatus(double &sensorsMin, double &sensorsMax, char &sensorsStatus, unsigned char stat, double data_i)
{

	for(unsigned char i = PrimaryData::DeathZone; i < PrimaryData::Undefined; i <<= 1)
	{
		if(stat & i)
		{

			if(sensorsStatus < PrimaryData::Undefined)
			{
				if(0 != data_i && sensorsMin > data_i) sensorsMin = data_i;
				if(sensorsMax < data_i) sensorsMax = data_i;

			}
			if(sensorsStatus > i)
			{
				if(PrimaryData::Undefined == sensorsStatus)
				{
					sensorsMin = sensorsMax = data_i;
				}
				sensorsStatus = i;
			}
			return;
		}
	}

	if(stat == PrimaryData::Undefined)
	{
		sensorsStatus = PrimaryData::Undefined;
		sensorsMin = sensorsMax = data_i;
	}

}

void Compute::CalculationFftFrame()
{
	unsigned startOffset = 0;
	unsigned stopOffset = 0;
	{
		Lock lock(cs0);
		startOffset = lastOffset;
		stopOffset = lastOffset = primaryData.GetCurrentOffset();
	}
	for(unsigned i = startOffset; i < stopOffset; ++i)
	{
		for(int sens = 0; sens < count_sensors; ++sens)
		{
			CalculationOneFrame(//вычисление кадра первичных данных(474, 986)
				sens
				, primaryData.SensorData(sens, i)
				, primaryData.result[sens][i]
		    	, primaryData.status[sens][i]    
			);
		}
	}
}

void Compute::Calculation(int startOffs, int stopOffs)
{
#ifdef DEBUG_ITEMS
	Communication::GetOffset();//получены временные смещения
#endif
	int countFrames = primaryData.GetCurrentOffset();
	int start = startOffs ;  //смещение начало
	primaryData.startOffset = start;
	int len = primaryData.countZones + 1;
	if(len > dimention_of(primaryData.offsetOfTime)) len = dimention_of(primaryData.offsetOfTime) - 1;
	SubRecalculation(0, len);
	DeathZoneFront(0);
	DeathZoneBack(len);
	zprint("countFrames %d start offs  %d stop offs %d", countFrames, start, stop);
	thicknessViewer.Update();
}

inline double __abs__(double x){return x >= 0 ? x : -x;}
void Compute::Calculation()
{
	unsigned stopOffset; 
	unsigned startOffset;
	for(;;)
	{		
		{
			Lock lock(cs0);
			stopOffset = primaryData.GetCurrentOffset();
			startOffset = lastOffset;
			if(stopOffset - startOffset > 300) stopOffset = startOffset + 300;
			lastOffset = stopOffset;
		}
		for(unsigned i = startOffset; i < stopOffset; ++i)
		{
			for(int sens = 0; sens < count_sensors; ++sens)
			{
				CalculationOneFrame(//вычисление кадра первичных данных(474, 986)
					sens
					, primaryData.SensorData(sens, i)
					, primaryData.result[sens][i]
				    , primaryData.status[sens][i]    
				);
			}
		}
		bool calculateFrame = startOffset < stopOffset;
		if(calculateFrame)
		{
			Lock lock(cs0);
			Point p = {startOffset, stopOffset};
			list.Add(p);
		}
		if(TryEnterCriticalSection(&cs1))
		{
			LockTry lock(cs1);
			for(;;)
			{
				unsigned start, stop;
				bool calculateZones = false;
				{
					Lock lock(cs0);					
					List<Point>::Item *i = list.items;
					while(NULL != i)
					{
						if(lastFrame == i->data.start)
						{
							start = i->data.start;
							stop = lastFrame = i->data.stop;
							calculateZones = true;
							list.Drop(i);
							break;
						}
						i = i->next;
					}
				}
				if(!calculateZones) return;
				const unsigned offs = filtre[0].length;
				for(unsigned lz = start - offs; lz < stop - offs; ++lz)
				{
					if(primaryData.countZones < dimention_of(primaryData.offsetOfTime) && lz > unsigned(primaryData.offsetOfTime[primaryData.countZones] ))
					{			
						++primaryData.countZones;	
						offsetOfZonesOffset += deltaTime;
						primaryData.offsetOfTime[primaryData.countZones] = offsetOfZonesOffset;//смещение зоны во времени				
						for(int i = 0; i < count_sensors; ++i)
						{
							sensorsData[i].zonesMin[primaryData.countZones] = result_undefined;
							sensorsData[i].zonesMax[primaryData.countZones] = result_undefined;
							sensorsData[i].status[primaryData.countZones] = PrimaryData::Undefined;
							sensorsData[i].countZones = primaryData.countZones;
						}
						thicknessData.zonesMin[primaryData.countZones] = result_undefined;
						thicknessData.zonesMax[primaryData.countZones] = result_undefined;
						thicknessData.status[primaryData.countZones] = PrimaryData::Undefined;
						thicknessData.countZones = primaryData.countZones;
						thicknessViewer.DrawSomeBar();
					}
					//if(deadAreaTime < lz)//изменено 18 11 14 если данные в мёртвой зоне вычисления не производятся
					{
						double &thicknessMin = thicknessData.zonesMin[primaryData.countZones];
						double &thicknessMax = thicknessData.zonesMax[primaryData.countZones];
						char   &thicknessStatus = thicknessData.status[primaryData.countZones];
						for(int i = 0; i < count_sensors; ++i)
						{
							double *data = &primaryData.result[i][lz];
							char *status = &primaryData.status[i][lz];
							int index = (filtre[i])(data);

							SetDataStatus(
								sensorsData[i].zonesMin[primaryData.countZones]
							, sensorsData[i].zonesMax[primaryData.countZones]
							, sensorsData[i].status[primaryData.countZones]
							, status[index]
							, data[index]
							);
							
							SetDataStatus(
								thicknessMin
								, thicknessMax
								, thicknessStatus
								, status[index]
							, data[index]
							);
						}
						/*
						if(thicknessMax - thicknessMin > defectDifferentWall) 
						{
							if(PrimaryData::Nominal == thicknessStatus || PrimaryData::DeathZone == thicknessStatus)thicknessStatus = PrimaryData::DefectDifferentWall;
							else thicknessStatus |= PrimaryData::DefectDifferentWall;
						}
						*/
					}
				}
			}
		}
		else
		{
			if(!calculateFrame)return;
		}
	}
}
//----------------------------------------------------------------------------------
struct DebugStatus
{
	double &result;
	char &status;
	DebugStatus(double &r, char &s)
		: result(r)
		, status(s)
	{}
	~DebugStatus()
	{
		dprint("result %f status %d", result, status);
		if(0 == status) Sleep(1000);
	}
};

struct  min_max
{
	double valMin, valMax;
	int offs;
};

void Compute::CalculationOneFrame(int sensorIndex, char *sensorData, double &result, char &status)
{
	result = result_undefined;
	double data[1024] = {};
	double t = 0;
	int j = 0;
	for(; j < frameSize; ++j)
	{
		data[j] = sensorData[j];
	}
///------------------------------------------
	fft.Direct(data);
	fft.Spectrum(data);	
//--------------------------------------отсечение в частотной области
	ZeroMemory(data, sizeof(double) * acfBorderLeft[sensorIndex]);
	ZeroMemory(&data[acfBorderRight[sensorIndex]], sizeof(double) * (fft.bufferSize - acfBorderRight[sensorIndex]));
//---------------------------------------------------------------------------------------
	fft.Direct(data);
	fft.Spectrum(data);
//------------------------------------------------------------------------------
	if(data[0] < minEnergy[sensorIndex]) 
	{
		status = PrimaryData::LessMinimumEnergy;//отбраковка минимальная энергия
		return;
	}

//	/* не удалять
	if(data[0] > maxEnergy[sensorIndex]) 
	{
		status = PrimaryData::GreaterMaximumEnergy;//отбраковка максимальная энергия
		return;
	}
	//*/
//-----------------------------------поиск пика	
	int maxOffs = maxOffset[sensorIndex];
	int z = minOffset[sensorIndex];
	if(maxOffs > frameSize) maxOffs = frameSize;
	if(z > frameSize / 2) z = 0;
	//double minVal = data[z];
	//double maxVal = minVal;
	double val = 0;
	int offsMin = z;
	int offsMax = z;
	int offsVal = z;

	double minVal = data[0];
	for(int i = 1; i < z; ++i)
	{
		if(minVal > data[i]) minVal = data[i];
	}
	double maxVal = minVal;

	double tresh = data[0] * peak[sensorIndex];

	for(; z < maxOffs; ++z)
	{
		if(minVal > data[z]) 
		{
			minVal = data[z];
			offsMin = z;
			if(offsMin > offsMax) 
			{
				minVal = maxVal = data[z];
				continue;
			}
		}
		if(maxVal < data[z])
		{
			maxVal = data[z];
			offsMax = z;
		}
		double t = maxVal - minVal;
		if(val < t) 
		{
			val = t;
			offsVal = z;
		}
	}

	if(val < data[0] * peak[sensorIndex])
	{
		 status = PrimaryData::ValueLessThreshold;//меньше допустимого порога
		 return;
	}
	if((int)minOffset[sensorIndex] > offsVal) offsVal = minOffset[sensorIndex];

	result = offsVal;
	result *= coefficientA[sensorIndex];
	result += coefficientB[sensorIndex];

	char s = PrimaryData::Nominal;
	
	if(result < tresholdDefect)
	{
		s = PrimaryData::Defect;
	}
	else if(result < treshold3Class) 
	{
		s = PrimaryData::Treshold3Class;
	}
	else if(result < treshold2Class) 
	{
		s = PrimaryData::Treshold2Class;
	}

	status = s;
}
//-----------------------------------------------------------------------
VOID CALLBACK APCProc(_In_  ULONG_PTR dwParam){}

void Compute::Stop()
{
  // EnterCriticalSection(&cs0);
  // stop = dimention_of(hThread);
   //LeaveCriticalSection(&cs0);
}
//---------------------------------------------------------------------------
void Compute::Destroy()
{
	TerminateThread(hThread[0], 0);
	TerminateThread(hThread[1], 0);
}
//-------------------------------------------------------------------------------
struct __Recalculation__
{
	Compute &owner;
	int start, stop;
	__Recalculation__(Compute &o, int start, int stop)
		: owner(o)
	    , start(start)
		, stop(stop)
	{}
	void Do()
	{
		for(int i = start; i < stop; ++i)
		{
			for(int sens = 0; sens < count_sensors; ++sens)
			{
				owner.CalculationOneFrame(//вычисление кадра первичных данных(486, 984)
					sens
					, owner.primaryData.SensorData(sens, i)
					, owner.primaryData.result[sens][i]
				, owner.primaryData.status[sens][i]    
				);
			//	owner.primaryData.result[sens][i - owner.primaryData.trimOffset] = owner.primaryData.testThickness;
			}
			//owner.primaryData.testThickness += 0.1;
			//if(owner.primaryData.testThickness > 10) owner.primaryData.testThickness = 0;
		}
	}
	static void Proc(__Recalculation__ *d){d->Do();}
};

void Compute::Recalculation()
{
	compute.InitParam();
	unsigned startTime = GetTickCount();
	//defectBorderMin = tresholdDefect;
	//defectBorderMax = treshold2Class;
	//defectBorder3 =  treshold3Class;

	ZeroMemory(&thicknessData, sizeof(thicknessData));
	ZeroMemory(&sensorsData, sizeof(sensorsData));
	ZeroMemory(primaryData.result, sizeof(primaryData.result));
	ZeroMemory(primaryData.status, sizeof(primaryData.status));
	int count = primaryData.GetCurrentOffset();
	if(count > 0)
	{
		count /= 2;
		int o = 0;
		__Recalculation__ _0(*this, o, o + count);
		o += count;
		__Recalculation__ _1(*this, o, o + count);
		
		HANDLE h[] = {
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__Recalculation__::Proc, &_0, 0,NULL)
			,  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)__Recalculation__::Proc, &_1, 0,NULL)
		};
		WaitForMultipleObjects(dimention_of(h), h, TRUE, INFINITE);

		for(int i = 0; i < dimention_of(h); ++i) CloseHandle(h[i]);

		int len = primaryData.countZones;
		if(len > dimention_of(primaryData.offsetOfTime)) len = dimention_of(primaryData.offsetOfTime);
		if(len > 0)
		{
			Strobes();

			SubRecalculation(0, len);
			DeathZoneFront(0);
			DeathZoneBack(len);

			thicknessData.countZones = primaryData.countZones = len;
			thicknessData.status[len - 1] = 0;
			for(int i = 0; i < count_sensors; ++i)
			{
				sensorsData[i].countZones = primaryData.countZones;
				sensorsData[i].status[len - 1] = 0;

			}
			unsigned stopTime = GetTickCount() - startTime;
			dprint(__FUNCTION__" duration computing %d\n", stopTime);
		}
	}
	thicknessViewer.Update();
}

void Compute::CalculationZones()
{
	int len = primaryData.countZones;
	if(len > dimention_of(primaryData.offsetOfTime)) len = dimention_of(primaryData.offsetOfTime);
	SubRecalculation(0, len);
	DeathZoneFront(0);
	DeathZoneBack(len);
	
	thicknessData.countZones = primaryData.countZones;
	for(int i = 0; i < count_sensors; ++i)
	{
		sensorsData[i].countZones = primaryData.countZones;
	}	
	thicknessViewer.Update();
}

void Compute::DeathZoneFront(int x)
{
	int offs = 0;
	double countZones = (double)deadAreaMM0 / zone_length;
	int full = (int)floor(countZones) + x;
	for(int i = x; i < full; ++i)
	{
		int last = unsigned((primaryData.offsetOfTime[i + 1]));		
		for(int j = offs; j < last; ++j)
		{
			for(int k = 0; k < count_sensors; ++k)
			{
				primaryData.status[k][j] = PrimaryData::DeathZone;
			}			
		}
		offs = last;
	}
	int  fractional = (int)ceill(countZones);
	
	double xlast = (primaryData.offsetOfTime[full + 1] - primaryData.offsetOfTime[full]) * (countZones - full);
	int last = offs + int( xlast);
	for(int j = offs; j <= last; ++j)
	{
		for(int k = 0; k < count_sensors; ++k)
		{
			primaryData.status[k][j] = PrimaryData::DeathZone;
		}
	}
	SubRecalculation(0, fractional);
}
void Compute::DeathZoneBack(int endZone)
{
	--endZone;
	double countZones = (double)deadAreaMM1 / zone_length;
	int full = (int)floor(countZones);
	int offs = int((primaryData.offsetOfTime[endZone - full]) );	
	int intact = offs;
	for(int i = endZone - full; i < endZone; ++i)
	{
		int last = int((primaryData.offsetOfTime[i + 1]) );		
		for(int j = offs; j <= last; ++j)
		{
			for(int k = 0; k < count_sensors; ++k)
			{
				primaryData.status[k][j] = PrimaryData::DeathZone;
			}
		}
		offs = last;
	}
	
	int fractional = (int)ceill(countZones);
	
	double xfirst = (countZones - full) * (primaryData.offsetOfTime[endZone - full] - primaryData.offsetOfTime[endZone - fractional]);
	int countFractionalZone = int(xfirst);
	for(int j = intact - countFractionalZone; j <= intact; ++j)
	{
		for(int k = 0; k < count_sensors; ++k)
		{
			primaryData.status[k][j] = PrimaryData::DeathZone;
		}
	}
	SubRecalculation(endZone - fractional, endZone);
}
void Compute::SubRecalculation(int start, int stop)
{
	unsigned offs = unsigned((primaryData.offsetOfTime[start]));
	for(int z = start; z < stop; ++z)
	{
		thicknessData.countZones = z;
		thicknessData.zonesMin[z] = result_undefined;
		thicknessData.zonesMax[z] = result_undefined;
		thicknessData.status[z] = PrimaryData::Undefined;
		for(int i = 0; i < count_sensors; ++i)
		{
			sensorsData[i].zonesMin[z] = result_undefined;
			sensorsData[i].zonesMax[z] = result_undefined;
			sensorsData[i].status[z] = PrimaryData::Undefined;
		}
		unsigned last = unsigned((primaryData.offsetOfTime[1 + z]));
		double &thicknessMin = thicknessData.zonesMin[z];
		double &thicknessMax = thicknessData.zonesMax[z];
		char   &thicknessStatus = thicknessData.status[z];
		for(unsigned j = offs; j < last; ++j)
		{
			for(int i = 0; i < count_sensors; ++i)
			{
				double *data = &primaryData.result[i][j];
				char *status = &primaryData.status[i][j];
				unsigned index = (filtre[i])(data);
                if(index > (unsigned)filtre[i].length) break;
				SetDataStatus(
					sensorsData[i].zonesMin[z]
				, sensorsData[i].zonesMax[z]
				, sensorsData[i].status[z]
				, status[index]
				, data[index]////////////
				);

				SetDataStatus(
					thicknessMin
					, thicknessMax
					, thicknessStatus
					, status[index]
				, data[index]
				);				
			}			
		}		
		offs = last;
	}
}
//------------------------------------------------------------------------------------

