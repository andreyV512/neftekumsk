#pragma once
#include <windows.h>
#include "message.h"
#include "ZonesViewer.h"
#include "CorrelationViewer.h"
#include "SignalViewer.h"
#include "ACFViewer.h"
#include "FFT.h"

class ZonesWindow
{
	int currentScreen;
public:
	HWND hLabelAllFrames, hLabelCalculatedFrames;
public:
	HWND hWnd;
	HWND hToolBar;
	unsigned currentOffsetInZone;
	ZonesWindow();
	void operator()(TCommand &);
	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	unsigned operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TMouseWell &);
	void operator()(TSysKeyDown &);
private:
	CFFT fft;
public:
	unsigned sensor, zone, offset;
private:
	int &signalLength;
	void Open_(unsigned sensor, unsigned zone);
public:
	void MouseMoveHandler(unsigned);
public:
	ZonesViewer zonesViewer;
	SignalViewer signalViewer;
	ACFViewer acfViewer;
	CorrelationViewer correlationViewer;
	static void Open(unsigned sensor, unsigned zone);
	static ZonesWindow &Instance();
};

