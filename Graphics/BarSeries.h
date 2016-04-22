#pragma once
#include "Chart.h"

class BarSeries
{
	class TObj{};
	typedef bool(TObj::*Tptr)(int, double &, unsigned &);
	bool GetColorBarDefault(int zone, double &data, unsigned &color);	
	TObj *obj;
	Tptr SetColorBar;
public:
	Chart &chart;
	BarSeries(Chart &chart);
	void Draw();
	void DrawSingleBar(int , int);
	template<class T>void SetColorBarHandler(T *t, bool(T::*ptr)(int, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};

class BarSeriesDouble
{
	class TObj{};
	typedef bool(TObj::*Tptr)(int, double &, unsigned &, double &, unsigned &);
	bool GetColorBarDefault(int zone, double &, unsigned &, double &, unsigned &);	
	TObj *obj;
	Tptr SetColorBar;
public:
	Chart &chart;
	BarSeriesDouble(Chart &chart);
	void Draw();
	void DrawSingleBar(int , int);
	template<class T>void SetColorBarHandler(T *t, bool(T::*ptr)(int, double &, unsigned &, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};