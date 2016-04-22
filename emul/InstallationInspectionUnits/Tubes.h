#pragma once
#include "Chart.h"

class Tube
{
	static const int color = 0xffffffff;
	 Chart &chart;
	 int y;
	 POINT point;
public:
	double length;
	double offsX;
	Tube(Chart &);
	void Draw();
	bool InRect(int );
//	void Restart();
};

class Sensor
{
	Chart &chart;
public:
	wchar_t *name;
	double x;
	int y;	
	unsigned *port;
	unsigned bit;
	Tube *tube;	
	Sensor(Chart &);
	void Draw();
};

class OutPut
{
	Chart &chart;
public:
	wchar_t *name;
	double x;
	int y;	
	unsigned *port;
	unsigned bit;
//	Tube *tube;	
	OutPut(Chart &);
	void Draw();
};

class Arrow
{
	Chart &chart;
	double x;
	int y;	
	int blink;
public:
	bool *forward, *backward;
	Arrow(Chart &);
	void Draw();
};

