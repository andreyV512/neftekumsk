#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
#include "Tubes.h"

template<int N>struct SQ;
template<int N>struct Y;

static const unsigned powerInverter = 0;
static const unsigned speedRL = 1;
static const unsigned speedRM = 2;
static const unsigned speedRH = 3;
static const unsigned sTF     = 4;
static const unsigned powerScanner = 5;
static const unsigned ready        = 6;
static const unsigned control      = 7;
static const unsigned result       = 8;

static const unsigned inverterRun = 4;


template<>struct SQ<1>: Sensor
{
	SQ(Chart &chart)
		: Sensor(chart)
	{
		x = 1; y = 5;
		bit = 1 << 0;
		name = L"SQ1";
	}
};

template<>struct SQ<2>: Sensor
{
	SQ(Chart &chart)
		: Sensor(chart)
	{
		x = 1.7; y = 5;
		bit = 1 << 1;
		name = L"SQ2";
	}
};

template<>struct SQ<inverterRun>: Sensor
{
	SQ(Chart &chart)
		: Sensor(chart)
	{
		x = 5; y = 40;
		bit = 1 << inverterRun;
		name = L"inverterRun";
	}
};

#define STRUCT_Y(bIt, oX, oY, nAme)template<>struct Y<bIt>: OutPut \
{  \
	Y(Chart &chart): OutPut(chart)\
	{	 \
		x = oX; y = oY;	\
		bit = 1 << bIt;	\
		name = nAme;  \
	}  \
};

STRUCT_Y(powerInverter, 5, 80, L"powerInverter")
STRUCT_Y(speedRL, 5, 100, L"speedRL")
STRUCT_Y(speedRM, 5, 120, L"speedRM")
STRUCT_Y(speedRH, 5, 140, L"speedRH")
STRUCT_Y(sTF    , 5, 160, L"sTF    ")
STRUCT_Y(powerScanner, 5, 180, L"powerScanner")
STRUCT_Y(ready       , 5, 200, L"ready       ")
STRUCT_Y(control     , 5, 220, L"control     ")
STRUCT_Y(result      , 5, 240, L"result      ")

class EmulViewer
{
	static const int ID_TIMER1 = 567;
	Gdiplus::Bitmap *backScreen;	
public:
	typedef TL::MkTlst<
		BottomAxesGrid
		, SQ<1>, SQ<2>
	//, SQ<inverterRun>
		, Y<powerInverter>
		, Y<speedRL>
		, Y<speedRM>
		, Y<speedRH>
		, Y<sTF    >
		, Y<powerScanner>
		, Y<ready       >
		, Y<control     >
		, Y<result      >
		, Arrow
		, Tube
	>::Result type_items_list;
	typedef ChartDraw<Chart, type_items_list> TChart;
	TChart chart;
	bool start;
	bool startFrame;
public:
	HWND hWnd;
	unsigned &inputPort;
	unsigned lastInputPort;
	unsigned &outputPort;
	unsigned lastOutputPort;
	double speed;
	double offsTube;
	unsigned lastTime;
	bool forward, backward;
	EmulViewer();
	void operator()(TPaint &);
	void operator()(TSize &);
	void operator()(TTimer &);
	void Start();
	void Stop();
	void Do();
};
