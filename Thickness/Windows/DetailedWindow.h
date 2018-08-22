#pragma once
#include <windows.h>
#include "message.h"
#include "ThicknessData.h"
#include "SensorViewer.h"
#include "typelist.hpp"
#include "TopLabelViewer.h"

class DetailedWindow
{
	template<int N>struct SensorViewerItem : SensorViewer
	{
		static const int Num = N;
		SensorViewerItem(TopLabelViewer &l) : SensorViewer(Num, l) {} 
	};
public:
	typedef TL::CreateNumList<SensorViewerItem, 1, count_sensors>::Result sensors_list;

	TL::Factory<sensors_list> sensorsWindow;
	TopLabelViewer topLabelViewer;
public:
	HWND hWnd;
	DetailedWindow();
	void operator()(TSize &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TCommand &);
	void operator()(TKeyDown &);
	void operator()(TMouseWell &);
public:
	static DetailedWindow &Instance();
	static void Open();
	void Update();	
};

