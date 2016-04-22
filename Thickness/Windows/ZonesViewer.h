#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ThicknessData.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "typelist.hpp"
#include "ZonesData.h"
class ZonesViewer
{	
	struct CursorLabel
	{
		ZonesViewer &owner;
		ZonesData &data;
		ColorLabel label;
		Chart &chart;
		unsigned offset;
		CursorLabel(Chart &, ZonesViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
		bool GetColorBar(int zone, double &data_, unsigned &color);
	};
public:
	TMouseMove storedMouseMove;
private:
	Gdiplus::Bitmap *backScreen;
public:
	ZonesData data;
private:
	struct TObj{};
	TObj *obj;
	void(TObj::*ptr)(unsigned);
	Cursor cursor;
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesInt 
		, BarSeries
		, Grid	
	>::Result> chart;
	CursorLabel cursorLabel;
	bool mouseMove;
public:
	HWND hWnd;
	unsigned &offset;
	unsigned zonesOffset;
	ZonesViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TDestroy &);
	void operator()(TLButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &);
	template<class T>void SetMouseMoveHandler(T *t, void(T::*ptr_)(unsigned))
	{
		obj = (TObj *)t;
		ptr = (void(TObj::*)(unsigned))ptr_;
	}
};
