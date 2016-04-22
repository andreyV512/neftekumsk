#pragma once
#include "message.h"
#include "Chart.h"
#include "Chart.hpp"
#include "GridChart.h"
#include "ColorLabel.h"
class CrossViewerData;
class CrossViewer
{
public:
	HWND hWnd;
private:
	Gdiplus::Bitmap *backScreen;
	ChartDraw<Chart, TL::MkTlst<
		LeftAxesGrid
		, BottomAxesGrid
		, GridSeries
		, GridGrid		
	>::Result> chart;
	bool mouseMove;
	TMouseMove storedMouseMove;
	ColorLabel label;
	Cursor cursor;
	CrossViewerData &crossViewerData;
	bool CursorHandler(TMouseMove &, VGraphics &);
public:
	CrossViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void Update();
};