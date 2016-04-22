#include "stdafx.h"
#ifdef DEBUG_ITEMS
#include "TestLanBuffer.h"
#include "DebugMess.h"

namespace{
	HANDLE hMapFile;
	wchar_t *name = L"buffer for data exchange";
}

void Mapped::Init()
{
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security 
		PAGE_READWRITE,          // read/write access
		0,                       // max. object size 
		sizeof(TestLanBuffer),                // buffer size  
		name);                 // name of mapping object
    int size = sizeof(TestLanBuffer);
	zprint(" size %d", size);
	buf = (TestLanBuffer *) MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,                   
		0,
		sizeof(TestLanBuffer));           
}

void Mapped::Destroy(void)
{
	UnmapViewOfFile(buf);
	CloseHandle(hMapFile);
}
#endif