#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ThicknessData.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "typelist.hpp"
#include "Borders.h"

class CorrelationViewer
{	
	struct CursorLabel
	{
		CorrelationViewer &owner;
		double (&data)[1024];
		ColorLabel label;
		Chart &chart;
		CursorLabel(Chart &, CorrelationViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
	};
	bool mouseMove;
	TMouseMove storedMouseMove;
	Gdiplus::Bitmap *backScreen;
public:
	double data[1024];
private:
	Cursor cursor;
public:
	class LeftBorder : public VBorder{public:LeftBorder(Chart &c):VBorder(c){}};
	class RightBorder : public VBorder{public:RightBorder(Chart &c):VBorder(c){}};
	class PeakBorder : public HBorder{public:PeakBorder(Chart &c):HBorder(c){}};
	class MinEnergyBorder : public HBorder{public:MinEnergyBorder(Chart &c):HBorder(c){color = 0xffff0000;}};
	class BottomBorder : public HBorder{public:BottomBorder(Chart &c):HBorder(c){color = 0xff0000ff;}};
	class VCursor : public VBorder{public:VCursor(Chart &c):VBorder(c){color = 0xff0000ff;}};
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, LineSeries
		, Grid	
		, LeftBorder
		, RightBorder
		, PeakBorder
		, MinEnergyBorder
		, BottomBorder
	>::Result> chart;
private:
	CursorLabel cursorLabel;
public:
	HWND hWnd;
	double coefficientA, coefficientB, peak, bottomBorder, minEnergy, thickness;
	char status;
	CorrelationViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &);
};
