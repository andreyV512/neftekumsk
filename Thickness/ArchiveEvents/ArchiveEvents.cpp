#include "stdafx.h"
#include "ArchiveEvents.h"
#include "typelist.hpp"
#include "Messages.h"
#include "InOutBits.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	

const wchar_t *nameArch = L"ArchiveEvents";
//------------------------------------------------------------------------------
ArchiveEvents::ArchiveEvents()
{
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(TMapData),                // buffer size  
		nameArch);                 // name of mapping object

	bool exist = GetLastError() == ERROR_ALREADY_EXISTS;

	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
	{ 
		dprint("Could not create file mapping object (%d).\n", 
			GetLastError());
		return;
	}
	map = (TMapData *) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		sizeof(TMapData));           

	if (map == NULL) 
	{ 
		dprint("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
	if(!exist)
	{
		map->head = 0;
		map->tail = 0;
	}
}
//-----------------------------------------------------------------------------
ArchiveEvents::~ArchiveEvents()
{
	UnmapViewOfFile(map);
	CloseHandle(hMapFile);
}
//------------------------------------------------------------------------------
//ArchiveEvents archiveEvents;
template<class O, class P>struct ClientEvents::ClientEvents_Init
{
	void operator()(O *, P *p)
	{
		p->TextArray[TL::IndexOf<message_list, O>::value] = &(MessageField<O>::Text);
		p->DataArray[TL::IndexOf<message_list, O>::value] = &(DataField<O>::Text);
	}
};
//--------------------------------------------------------------------------------
ClientEvents::ClientEvents()
{
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(ArchiveEvents::TMapData),                // buffer size  
		nameArch);                 // name of mapping object
	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
	{ 
		dprint("Could not open file mapping object (%d).\n", 
			GetLastError());
		return;
	} 

	map = (ArchiveEvents::TMapData *)MapViewOfFile(hMapFile,    // handle to mapping object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,                    
		0,                    
		sizeof(ArchiveEvents::TMapData));                   

	if (map == NULL) 
	{ 
		dprint("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
	
	TL::foreach<message_list, ClientEvents_Init>()((TL::Factory<message_list> *)0, this);//TextArray);
}
//--------------------------------------------------------------------------------
ClientEvents::~ClientEvents()
{
	UnmapViewOfFile(map);
	CloseHandle(hMapFile);
}
//--------------------------------------------------------------------------------
#pragma warning(disable : 4996)
wchar_t *ClientEvents::GetText(unsigned index)
{		
	if(index > map->tail && (index - map->tail) < dimention_of(map->data))
	{
		unsigned i = index % dimention_of(map->data);
		unsigned e = map->data[i].event;
		if(0 == e) return L"";
		if(e < dimention_of(TextArray))return TextArray[e]();
	}
	return L"";
}
//--------------------------------------------------------------------------------
wchar_t *ClientEvents::GetTime(unsigned index)
{		
	if(index > map->tail && (index - map->tail) < dimention_of(map->data))
	{
		unsigned i = index % dimention_of(map->data);
		if(0 ==  map->data[i].event) return L"";
		unsigned z = map->data[(i - 1)% dimention_of(map->data)].time;
		if(z == 0) z = map->data[i].time;
		unsigned t = map->data[i].time - z;
		wsprintf(buf, L"%3d.%.3d", t / 1000, t % 1000);
		return buf;
	}
	return L"";
}
//--------------------------------------------------------------------------------
wchar_t *ClientEvents::NumberMessage(unsigned index)
{
	if(index > map->tail && (index - map->tail) < dimention_of(map->data))
	{
		unsigned i = index % dimention_of(map->data);
		if(0 ==  map->data[i].event) return L"";
		wsprintf(buf, L"%4d", index);
		return buf;
	}
	return L"";
}
//------------------------------------------------------------------------------------------------------------------
wchar_t *ClientEvents::GetData(unsigned index)
{
	if(index > map->tail && (index - map->tail) < dimention_of(map->data))
	{
		unsigned i = index % dimention_of(map->data);
		unsigned e = map->data[i].event;
		if(0 == e) return L"";
		if(e < dimention_of(DataArray))return DataArray[e](map->data[i].data);
	}
	return L"";
}
//----------------------------------------------------------------------------------------------------------------
void ClientEvents::GetDataColor(DataColor &d)
{
	if(d.row > map->tail && (d.row - map->tail) < dimention_of(map->data))
	{
		unsigned i = d.row % dimention_of(map->data);
		unsigned e = map->data[i].event;
		if(e > 0)
		{
			if(e < dimention_of(DataArray))
			{
				if(map->data[d.row].data > 0)
				{
					d.d->clrText = -1;
					d.d->clrTextBk = 0xddddff;
					return;
				}
			}
		}
	}
	d.d->clrText = 0;;
	d.d->clrTextBk = -1;
}
//---------------------------------------------------------------------------------------------------------
//message_list
template<class T>struct __text_color__
{
	void operator()(NMLVCUSTOMDRAW *d)
	{
		d->clrText = 0;
	    d->clrTextBk = -1;
	}
};

#define TEXT_COLOR_TYPE(n, clr_text, clr_textBk)template<>struct __text_color__<n>\
{\
	void operator()(NMLVCUSTOMDRAW *d)\
	{\
		d->clrText = clr_text;\
	    d->clrTextBk = clr_textBk;\
	}\
};

TEXT_COLOR_TYPE(SQ1BitIn           , -1, 0xffdddd)
//TEXT_COLOR_TYPE(PipeInletSQ3BitIn, -1, 0xddffdd)

#undef TEXT_COLOR

typedef TL::ListToWapperList<message_list, __text_color__>::Result __text_color_list__;
TL::Factory<__text_color_list__> __text_color_rows__;
TL::ArrayFunc<__text_color_list__, NMLVCUSTOMDRAW *, void> __text_color_funcs__(__text_color_rows__);

void ClientEvents::GetTextColor(DataColor &d)
{
	if(d.row > map->tail && (d.row - map->tail) < dimention_of(map->data))
	{
		__text_color_funcs__[map->data[d.row % dimention_of(map->data)].event](d.d);
		return;
	}
	d.d->clrText = 0;
	d.d->clrTextBk = -1;
}
