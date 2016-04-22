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
	HWND hWnd;
	HWND hToolBar;
	//int &frequencyFrames;
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
	void MouseMoveHandler(unsigned);
public:
	ZonesViewer zonesViewer;
	SignalViewer signalViewer;
	ACFViewer acfViewer;
	CorrelationViewer correlationViewer;
	static void Open(unsigned sensor, unsigned zone);
	static ZonesWindow &Instance();
};

