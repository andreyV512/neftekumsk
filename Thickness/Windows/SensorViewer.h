#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "ThicknessData.h"
#include "Chart.hpp"
#include "BarSeries.h"
#include "typelist.hpp"
#include "TopLabelViewer.h"

class SensorViewer
{	
	struct CursorLabel
	{
		int mouse_x;		
		ColorLabel label;
		Chart &chart;
		int num;
		TopLabelViewer &topLabel;
		CursorLabel(Chart &, int, TopLabelViewer &);
		bool Draw(TMouseMove &l, VGraphics &g);
		bool GetColorBar(int zone, double &, unsigned &, double &, unsigned &);
	};
public:
	bool mouseMove;
	TMouseMove storedMouseMove;
public:
	Gdiplus::Bitmap *backScreen;
public:
	Cursor cursor;
private:
public:
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, BarSeriesDouble
		, Grid		
	>::Result> chart;
	CursorLabel cursorLabel;
public:
	const unsigned num;
	HWND hWnd;
	SensorViewer(int, TopLabelViewer &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &);
	void operator()(TLButtonDown &);
	void Repaint();
};
