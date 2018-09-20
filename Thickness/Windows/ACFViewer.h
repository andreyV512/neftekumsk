#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ThicknessData.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "typelist.hpp"
#include "Borders.h"

class ACFViewer
{	
	struct CursorLabel
	{
		ACFViewer &owner;
		double (&data)[1024];
		ColorLabel label;
		Chart &chart;
		CursorLabel(Chart &, ACFViewer &);
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
	class VCursor : public VBorder{public:VCursor(Chart &c):VBorder(c){color = 0xff0000ff;}};
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, LineSeries
		, Grid	
		, LeftBorder
		, RightBorder
	>::Result> chart;
private:
	CursorLabel cursorLabel;
public:
	void (ACFViewer::*bordersProc)(int);
	void LeftBorderProc (int);
	void RightBorderProc(int);
	void RecomputeProc  (int);
public:
	HWND hWnd;
	int /*coefficientA, coefficientB, peak, minEnergy, thickness,*/ acfBorderLeft, acfBorderRight;
	ACFViewer();												   
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &);
};
