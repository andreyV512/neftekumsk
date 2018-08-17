#include "stdafx.h"
#include "DebugMess.h"
#ifdef XDEBUG
#include <stdio.h>
#define d_mess debug.print
namespace {
	wchar_t name[] = L"Parallel hatch in space";
}
DebugMess::DebugMess()
{
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(TMapData),                // buffer size  
		name);                 // name of mapping object
	/*
	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
	{ 
	d_mess("Could not create file mapping object (%d).\n", 
	GetLastError());
	return;
	}
	*/
	int res = GetLastError();
	map = (TMapData *) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,                   
		sizeof(TMapData));           

	if (map == NULL) 
	{ 
		d_mess("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
	if(res == 0)
	{
		map->head = 0;
		map->tail = 0;
	}
}

void DebugMess::Destroy(void)
{
	UnmapViewOfFile(map);
	CloseHandle(hMapFile);
}
#pragma warning(disable : 4996)
void DebugMess::print(char *c, ...)
{
	if(NULL != map)
	{
		LONG i = InterlockedIncrement(&map->head);
		--i;
		i &= 0xff;
		char *b = map->data[i];
		vsprintf(b, c, (char *)&c + 4);
		size_t len = strlen(b);
		b[len] = '\n';
		b[1 + len] = '\0';
	}
}
//---------------------------------------------------------------------------------------
ViewerDebugMess::ViewerDebugMess() : map(NULL)
{	
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(TMapData),                // buffer size  
		name);                 // name of mapping object
	if (hMapFile == NULL) 
	{ 
		d_mess("Could not open file mapping object (%d).\n", 
			GetLastError());
		return;
	} 

	map = (TMapData *)MapViewOfFile(hMapFile,    // handle to mapping object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,                    
		0,                    
		sizeof(TMapData));                   

	if (map == NULL) 
	{ 
		d_mess("Could not map view of file (%d).\n", 
			GetLastError()); 
		return;
	}
}
//----------------------------------------------------------------------------
ViewerDebugMess::~ViewerDebugMess()
{
	UnmapViewOfFile(map);
	CloseHandle(hMapFile);
}
//----------------------------------------------------------------------------
char *ViewerDebugMess::get()
{
	if(map && map->tail != map->head)
	{
		static char b[512];		
		CharToOemA(map->data[map->tail & 0xff], b);
		++map->tail;
		return b;
	}
	return NULL;
}
//-----------------------------------------------------------------------------

class DebugMess::Initialization
{
	DebugMess debug;
public:
	static DebugMess &Instance(){static Initialization x; return x.debug;}
};
DebugMess &debug = DebugMess::Initialization::Instance();
#endif
