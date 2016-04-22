#pragma once
#include <time.h>
#include "Messages.h"
#include <Windows.h>
#include "typelist.hpp"
#include <CommCtrl.h>
#include <Mmsystem.h>

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif		   

class ArchiveEvents
{
public:
	struct Data
	{
		unsigned time;
		int event;
		double data;
	};
	struct TMapData
	{
		unsigned input, output;
		unsigned head, tail;
		Data data[1024];
	};
	HANDLE hMapFile;	
	TMapData *map;
	ArchiveEvents();
	~ArchiveEvents();
	template<class Event, class D>void Set(D d)
	{		
		Data t = {timeGetTime(), TL::IndexOf<message_list, Event>::value, (double)d};
		unsigned i = InterlockedIncrement(&map->head);
		i %= dimention_of(map->data);
		map->data[i] = t;
	}
};

class ClientEvents
{
	wchar_t buf[512];
	template<class O, class P>struct ClientEvents_Init;
	wchar_t *(*TextArray[TL::Length<message_list>::value])();
	wchar_t *(*DataArray[TL::Length<message_list>::value])(double );
public:
	HANDLE hMapFile;	
	ArchiveEvents::TMapData *map;
	ClientEvents();
	~ClientEvents();
	wchar_t *GetText(unsigned );
	wchar_t *GetTime(unsigned );
	wchar_t *NumberMessage(unsigned );
	wchar_t *GetData(unsigned);
	struct DataColor
	{
        NMLVCUSTOMDRAW *d;
		unsigned row;
		DataColor(NMLVCUSTOMDRAW *d, unsigned row) : d(d), row(row) {}
	};
	void GetDataColor(DataColor &);
	void GetTextColor(DataColor &);
};

extern ArchiveEvents archiveEvents;
