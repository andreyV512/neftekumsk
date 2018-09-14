#include "stdafx.h"
#include "Automat.h"
#include "SQEvents.h"
#include "DebugMess.h"
#include "App.h"
#include "LogBuffer.h"
#include "LogMessages.h"
#include <Mmsystem.h>
#include "Lan.h"
#include "Compute.h"
#include "DataExchange.h"
#include "Config.h"
#include "CommunicationWapper\CommunicationWapper.h"
#include "Windows\MainWindowToolBar.h"

#pragma warning(disable : 4995)

DWORD WINAPI Automat::Proc(PVOID x)
{
	((Automat *)x)->Do();
	return 0;
}
//------------------------------------------------------------------------------
namespace
{
#define xEvent2(name) HANDLE &h##On##name = sQEvents.items.get<SQEvents::Event<SQEvents::On<name> > >().hEvent;\
	HANDLE &h##Off##name = sQEvents.items.get<SQEvents::Event<SQEvents::Off<name> > >().hEvent;

#define xEvent1(name) HANDLE &h##name =	sQEvents.items.get<SQEvents::Event<SQEvents::name> >().hEvent;

	xEvent2(SQ1BitIn           )
		xEvent2(SQ3BitIn           )
		xEvent2(StateBBitIn        )
		xEvent2(StateABitIn        )
		xEvent2(RunBitIn           )
		xEvent2(ControlCircuitBitIn)
		xEvent2(WorkBitIn          )
		xEvent2(SycleBitIn         )
		xEvent2(BaseBitIn          )

		xEvent1(AlarmExit)
		xEvent1(SingleMode)
		xEvent1(AutomaticMode) 		
		xEvent1(UserStop)

#undef xEvent1
#undef xEvent2
	struct __RotationalSpeedSensorsStart_Data__
	{
		unsigned output;
		OutputBitsTable::TItems &items;
		__RotationalSpeedSensorsStart_Data__()
			: output(0)
			, items(Singleton<OutputBitsTable>::Instance().items)
		{}
	};
	template<class O, class P>struct __RotationalSpeedSensorsStart__
	{
		void operator()(O *o, P *p)
		{
			if(o->value) p->output |= p->items.get<typename TL::Inner<O>::Result>().value;
		}
	};
	
	void ErrStop()
	{
		//	CLR_BITS(PowerScannerBitOut, ReadyBitOut, ControlBitOut, ResultBitOut, SpeedRLBitOut, SpeedRMBitOut, SpeedRHBitOut, STFBitOut);
		ResetEvent(hAutomaticMode);
		SetEvent(hAlarmExit);
	}

	void ClearOutBits()
	{
		unsigned t = (unsigned)-1;	
		t &= ~Singleton<OutputBitsTable>::Instance().items.get<PowerInverterBitOut>().value;
		t &= ~Singleton<OutputBitsTable>::Instance().items.get<ReadyBitOut>().value;
		t &= ~Singleton<OutputBitsTable>::Instance().items.get<ResultBitOut>().value;
		sQEvents.ClrBits(t);
	}

	void ClearAllBits()
	{
		ResetEvent(hAutomaticMode);		
		Log::Mess<LogMess::ErrStop>(0);
		zprint(" ErrStop stop");
		ClearOutBits();
	}

	struct TestSycle
	{
		typedef TL::MkTlst<RunBitIn, ControlCircuitBitIn, WorkBitIn, SycleBitIn>::Result error_list;
		template<class O, class P>struct __maska_bits__
		{
			void operator()(O *o, P *p)
			{
				*p |= o->value;
			}
		};
		template<class O, class P>struct __search_text_message__
		{
			bool operator()(O *o, P *p)
			{
				if(0 == (o->value & *p))
				{
					Log::Mess<Text<O>::value>(0);
					SetEvent(hAlarmExit);
					return false;
				}
				return true;
			}
		};
		template<class T>struct Text;
#define T(name)template<>struct Text<name>{static const int value = LogMess::AlarmExit##name;};
		T(RunBitIn			 )
			T(ControlCircuitBitIn)
			T(WorkBitIn			 )
			T(SycleBitIn		 )
#undef T
			HANDLE hTimer;
		unsigned bits;
		unsigned countCycles;
		bool testBits;
		TestSycle()
			: hTimer(INVALID_HANDLE_VALUE)
			, bits(0)
			, testBits(false)
		{ }		
		void Create()
		{
			TL::foreach<error_list, __maska_bits__>
				()(&Singleton<InputBitsTable>::Instance().items, &bits);
			CreateTimerQueueTimer(&hTimer, NULL, WaitOrTimerCallback, this, 3000, 1000, WT_EXECUTEINTIMERTHREAD);
		}
		void Destroy()
		{
			if(INVALID_HANDLE_VALUE != hTimer)DeleteTimerQueueTimer(NULL, hTimer, NULL);
		}
		void Start()
		{
			countCycles = 0;
			testBits = true;
			zprint(" start");
		}
		void Stop()
		{
			testBits = false;
			zprint(" stop");
			Automat::RotationalSpeedSensorsStop();
			CLR_BITS(ControlBitOut);
            //ClearOutBits();
		}
		void Do()
		{
			if(testBits)
			{
				if(bits == (bits & sQEvents.input))
				{
					countCycles = 0;
				}
				else if(++countCycles > 3)
				{
					TL::find<error_list, __search_text_message__>()(&Singleton<InputBitsTable>::Instance().items, &sQEvents.input);
					zprint(" err");					
					Stop();
				}
				if(!(sQEvents.input & Singleton<InputBitsTable>::Instance().items.get<WorkBitIn>().value))
				{
					Stop();

					Log::Mess<LogMess::InfoOffPowerScannerBitOut>(0);
					CLR_BITS(PowerScannerBitOut);//10 1
					

					//Sleep(1500);

					Lan::Stop();
					Automat::RotationalSpeedSensorsStop();
					sQEvents.ClrBits((unsigned)-1);
				}
			}

			Log::TData *d;
			if(Log::LastMessage(d))
			{
				LogMess::FactoryMessages &f = LogMess::FactoryMessages::Instance();
				wchar_t mess[128];
				unsigned textColor, backColor;
				f.Color(d->id, backColor, textColor);
				char c[128];
				f.Text(d->id, c, d->value);
				backColor = ((backColor << 16) & 0xff0000) | ((backColor >> 16) & 0xff) | (backColor & 0xff00);
				wsprintf(mess, L"<%6x>%S", backColor, c);
				app.MainWindowTopLabel(mess);
			}
		}
		static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
		{
			((TestSycle *)lpParameter)->Do();
		}
	};
}
// -----------------------------------------------------------------------------
static const int __5_minutes__ = 60 * 5 * 1000;
static const int __1_minutes__ = 60 * 1 * 1000;
static const int __20_sek__ = 20 * 1000;

#define StartWaitEvent(...){\
	HANDLE h[] = {__VA_ARGS__};\
	state = WaitForMultipleObjects(dimention_of(h), h, FALSE, INFINITE);\
}

#define WaitEvent(tme, e){\
	HANDLE h[] = {App::ProgrammExitEvent, hAlarmExit, hUserStop, e};\
	switch(WaitForMultipleObjects(dimention_of(h), h, FALSE, tme))	\
{\
	case WAIT_TIMEOUT:\
	Log::Mess<LogMess::TimeoutPipe>(0);\
	ClearAllBits();\
	continue;\
	case 0 + WAIT_OBJECT_0:\
	Log::Mess<LogMess::ProgramClosed>(0);\
	goto EXT;\
	case 1 + WAIT_OBJECT_0:\
	ClearAllBits();\
	zprint("  hAlarmExit");\
	continue;\
	case 2 + WAIT_OBJECT_0:\
	Log::Mess<LogMess::InfoUserStop>(0);\
	ClearAllBits();\
	continue;\
	case 3 + WAIT_OBJECT_0:\
	;\
	break;\
}\
}

TestSycle testSycle;

void Automat::RotationalSpeedSensorsStart()
	{
		__RotationalSpeedSensorsStart_Data__ data;
		TL::foreach<RotationalSpeedSensorsTable::items_list, __RotationalSpeedSensorsStart__>()(&Singleton<RotationalSpeedSensorsTable>::Instance().items, &data);
		sQEvents.SetBits(data.output);
		SET_BITS(STFBitOut);
	}

	void Automat::RotationalSpeedSensorsStop()
	{
		CLR_BITS(SpeedRLBitOut, SpeedRMBitOut, SpeedRHBitOut, STFBitOut);
	}

Automat::Automat()
	: testMode(false), workMode(false), exitTubeMode(false) 
{
	testSycle.Create();
}

void Automat::Do()
{
	//DataExchange().DeleteTimeOffset(); //удалить данные в таблице Strobes
	unsigned state = 0;
	SET_BITS(PowerInverterBitOut);
	while(true)
	{
#ifdef DEBUG_ITEMS
		switch(WaitForSingleObject(App::hEventStrobes, 5 * 60000))
		{
		case WAIT_OBJECT_0:
			{
				compute.Recalculation();
				SendThickhess();
			}
			break;
		}
		continue;
#endif
		workMode = false;
		bool automatMode = false;
		bool singleMode = false;
		ClearOutBits();
		Sleep(1111);
		Lan::Stop();
		MainWindowToolBar::NoCycle();
		StartWaitEvent(hAutomaticMode, hSingleMode, App::ProgrammExitEvent);
		switch(state)
		{
		case 0 + WAIT_OBJECT_0:
			automatMode = true;
			Log::Mess<LogMess::AutomatMode>(0);
			break;
		case 1 + WAIT_OBJECT_0:
			singleMode = true;
			Log::Mess<LogMess::SingleMode>(0);
			break;
		case 2 + WAIT_OBJECT_0:
			Log::Mess<LogMess::ProgramClosed>(0);
			goto EXT;
		}

		if(testMode || exitTubeMode)
		{
			continue;
		}
		dprint("start sycle");
		workMode = true;
		
		if(ANY_BITS(SQ1BitIn, SQ3BitIn))
		{
			Log::Mess<LogMess::AlarmExitTubeInUnit>(0);//"Труба в модуле, выполните выгон");
			ErrStop();
			continue;
		}

		if(!ANY_BITS(ControlCircuitBitIn))	  //2
		{
			Log::Mess<LogMess::AlarmExitDirectionChainOn>(0);//"Включите цепи управления");
			ErrStop();
			continue;
		}

		if(!ANY_BITS(StateABitIn))	  //4
		{
			Log::Mess<LogMess::AlarmExitInverterFaulty>(0);//"Частотный преобразователь не исправен");
			ErrStop();
			continue;
		}
		MainWindowToolBar::Cycle();
		Log::Mess<LogMess::InfoOnWorkBitIn>(0);//"Ожидание сигнала \"Работа\"");	//5
		WaitEvent(__5_minutes__, hOnWorkBitIn)

			Log::Mess<LogMess::InfoReadyBitOut>(0);//"Выставлен сигнал \"Готовность\"");	//6
		SET_BITS(ReadyBitOut);


		//Log::Mess<LogMess::InfoOnSycleBitIn>(0);//"Ожидание сигнала \"Цикл\""); //7
		//WaitEvent(__5_minutes__, hOnSycleBitIn)
		SET_BITS(PowerInverterBitOut);
		Log::Mess<LogMess::InfoRotationalSpeedSensorsStart>(0);//"Включение вращения модуля датчиков сканирования"); //8
		RotationalSpeedSensorsStart();

		//__20_sek__
		Log::Mess<LogMess::InfoOnRunBitIn>(0);//"Ожидание раскрутки модуля датчиков сканирования"); //8 1
		WaitEvent(__20_sek__, hOnRunBitIn)

			Log::Mess<LogMess::InfoControlBitOut>(0);//"Выставлен сигнал \"Контроль\"");	//9
		SET_BITS(ControlBitOut);

		CLR_BITS(ResultBitOut);
		testSycle.Start();//периодическая проверка "Цепи управления" "Работа" "Цикл"

		Log::Mess<LogMess::InfoOnSQ1BitIn>(0);//"Ожидание трубы на датчике SQ1");  //10
		WaitEvent(__5_minutes__, hOnSQ1BitIn)
			compute.MeshuringBaseStart();

		Lan::Start();   //сбор данных LAN10

	//	testSycle.Start();//периодическая проверка "Цепи управления" "Работа" "Цикл"

		Log::Mess<LogMess::InfoPowerScannerBitOut>(0);//"Включение сканирующего устройства");
		SET_BITS(PowerScannerBitOut);//10 1

		unsigned timeSQ1 = timeGetTime();
		primaryData.scanerStart = timeSQ1;
		Log::Mess<LogMess::InfoOnSQ3BitIn>(0);//"Ожидание трубы на датчике SQ3");  //10	2
		WaitEvent(__20_sek__, hOnSQ3BitIn)

			unsigned timeSQ3 = timeGetTime();
		primaryData.scanerStop = timeSQ3;

		primaryData.frameStartTime = timeSQ1 + (timeSQ3 - timeSQ1) / 2;//время в середине сканирующего устройства
		//-----------------------------------------------------------------------------------------------------
		Log::Mess<LogMess::InfoOffSQ1BitIn>(0);//"Ожидание съезда трубы с датчика SQ1");  //10
		WaitEvent(__1_minutes__, hOffSQ1BitIn)

			timeSQ1 = timeGetTime();

		Log::Mess<LogMess::InfoOffSQ3BitIn>(0);//"Ожидание съезда трубы с датчика SQ3");  //10	2
		WaitEvent(__20_sek__, hOffSQ3BitIn)
			timeSQ3 = timeGetTime();

		testSycle.Stop();

		Log::Mess<LogMess::InfoOffPowerScannerBitOut>(0);
		CLR_BITS(PowerScannerBitOut);//10 1

		Sleep(1500);

		Lan::Stop();

		int stopTimeTube = (timeSQ3 - timeSQ1) / 2;//время в середине сканирующего устройства
		ClearOutBits();

		Log::Mess<LogMess::WaitStrobes>(0);
		WaitForSingleObject(App::hEventStrobes, INFINITE);		
		compute.Recalculation();
		SendThickhess();
		ResetEvent(App::hEventStrobes);

		Log::Mess<LogMess::InfoDataCollectionComplete>(0);
		dprint("stop sycleXXX");
	}
EXT:
	testSycle.Destroy();
	sQEvents.ClrBits((unsigned)-1);
}
void Automat::Init()
{
	CloseHandle(CreateThread(NULL, 0, Proc, this, 0, NULL));
}
void Automat::Destroy()
{

}

void Automat::AutomatMode()
{
	SetEvent(hAutomaticMode);
}

void Automat::SingleMode()
{
	SetEvent(hSingleMode);
}

void Automat::StopMode()
{
	SetEvent(hUserStop);
}

DWORD WINAPI Automat::ProcTest(PVOID x)
{
	((Automat *)x)->DoTest();
	return 0;
}

bool Automat::TestMode()
{
	if(workMode)return false;
	testMode = true;
	thicknessData.Clear();
	primaryData.Clear();
	QueueUserWorkItem(ProcTest, this, WT_EXECUTELONGFUNCTION);
	return true;
}

namespace{
	bool Delay(unsigned d)
	{
		return WAIT_TIMEOUT == WaitForSingleObject(App::ProgrammExitEvent, d);
	}		
}
void Automat::DoTest()
{	
	Log::Mess<LogMess::InfoPowerScannerBitOut>(0);//"Включение сканирующего устройства");
	SET_BITS(PowerScannerBitOut);//10 1
	Lan::Start();   //сбор данных LAN10
	int dly = 5000;
	if(Delay(dly))
	{
		CLR_BITS(PowerScannerBitOut);

		Log::Mess<LogMess::InfoOffPowerScannerBitOut>(0);
		CLR_BITS(PowerScannerBitOut);//10 1

		int count = 50;

		double delta = primaryData.GetCurrentOffset() / count;

		primaryData.frameStartTime = primaryData.realOffsetTime[1];

		for(int i = 0; i < count + 1; ++i)
		{
			primaryData.indexOffsetZone[i] = unsigned(i * delta);
		}
		primaryData.countZones = count;
		compute.Recalculation();

		testMode = false;
		Delay(1000);
	}
	Lan::Stop();
	MainWindowToolBar::NoCycle();
}

DWORD WINAPI Automat::ProcExitTube(PVOID x)
{
	((Automat *)x)->ExitTubeDo();
	return 0;
}

void Automat::ExitTube()
{
	if(!workMode)
	{
		exitTubeMode = true;
		QueueUserWorkItem(ProcExitTube, this, WT_EXECUTELONGFUNCTION);
	}
}

void Automat::ExitTubeDo()
{	
	SET_BITS(PowerInverterBitOut);
	bool exit = false;
	while(!exit)
	{
		exit = true;

		Log::Mess<LogMess::InfoRotationalSpeedSensorsStart>(0);//"Включение вращения модуля датчиков сканирования"); //8
		RotationalSpeedSensorsStart();

		//__20_sek__
		Log::Mess<LogMess::InfoOnRunBitIn>(0);//"Ожидание раскрутки модуля датчиков сканирования"); //8 1
		WaitEvent(__20_sek__, hOnRunBitIn)

			Log::Mess<LogMess::InfoControlBitOut>(0);//"Выставлен сигнал \"Контроль\"");	//9
		SET_BITS(ControlBitOut);

		Log::Mess<LogMess::InfoOnSQ1BitIn>(0);//"Ожидание трубы на датчике SQ1");  //10
		WaitEvent(__5_minutes__, hOnSQ1BitIn)

			Log::Mess<LogMess::InfoOffSQ1BitIn>(0);//"Ожидание съезда трубы с датчика SQ1");  //10
		WaitEvent(__1_minutes__, hOffSQ1BitIn)

	}
	SET_BITS(PowerInverterBitOut);
	exitTubeMode = false;
	return;
EXT:														
	sQEvents.ClrBits((unsigned)-1);
}

#undef StartWaitEvent
#undef WaitEvent