#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ThicknessData.h"
#include "Chart.hpp"
#include "BarSeries.h"

struct CursorLabel
{
	ColorLabel label;
	Chart &chart;
	CursorLabel(Chart &chart);
	bool Draw(TMouseMove &l, VGraphics &g);
	bool GetColorBar(int zone, double &data_, unsigned &color);
};

class ThicknessViewer
{		
	struct CursorLabel
	{
		ColorLabel label;
		Chart &chart;
		CursorLabel(Chart &chart);
		bool Draw(TMouseMove &l, VGraphics &g);
		bool GetColorBar(int zone, double &, unsigned &);
	};
	bool mouseMove;
	Gdiplus::Bitmap *backScreen;
	Cursor cursor;
public:
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxesMeters
		, BarSeries
		, Grid		
	>::Result> chart;
	CursorLabel cursorLabel;	
	bool painting;
public:
	HWND hWnd;
	TMouseMove storedMouseMove;
	bool openDetailedWindow;
	ThicknessViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &l);
	void operator()(TLButtonDown &l);
	void Update();
	void DrawSomeBar();
};
extern ThicknessViewer thicknessViewer;

