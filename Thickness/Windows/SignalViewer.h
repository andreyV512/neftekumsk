#pragma once
#include "message.h"
#include "ColorLabel.h"
#include "Chart.h"
#include "SignalData.h"
#include "Chart.hpp"
#include "typelist.hpp"
#include "Borders.h"

class SignalViewer
{	
	struct CursorLabel
	{
		double (&data)[1024];
		ColorLabel label;
		SignalViewer &owner;
		CursorLabel(SignalViewer &, double (&)[1024]);
		bool Draw(TMouseMove &l, VGraphics &g);
	};
	bool mouseMove;
	TMouseMove storedMouseMove;
	Gdiplus::Bitmap *backScreen;
public:
	double data[1024];
	double coefficientA, coefficientB;
private:
	Cursor cursor;
	class VCursor1 : public VBorder
	{
	public:
		VCursor1(Chart &c):VBorder(c){color = 0xffffff00;}
	};
	class VCursor2 : public VBorder
	{
	public:
		VCursor2(Chart &c):VBorder(c){color = 0xff00ffff;}
	};
	ChartDraw<Chart, TL::MkTlst<
		LeftAxes
		, BottomAxes 
		, LineSeries
		, Grid	
		, VCursor1
		, VCursor2
	>::Result> chart;
	CursorLabel cursorLabel;
	const int &signalLength;
public:
	HWND hWnd;
	SignalViewer();
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDown &);
	void operator()(TRButtonDown &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TKeyDown &);
};
