#include  "stdafx.h"
#include "EmulLan.h"
#ifdef DEBUG_ITEMS
#include <Windows.h>
#include <stdio.h>
#include "typelist.hpp"
#include "TestLanBuffer.h"

#include "DebugMess.h"

namespace{
wchar_t *path[] = {
	L"D:/Projects/project/chelabinsk/tubes/etalon.dat"
	, L"D:/Projects/project/chelabinsk/tubes/ÑÎÏ 114_11 141022 1200.dat"
	, L"D:/Projects/project/chelabinsk/tubes/ÑÎÏ 114_11 141023 1340.dat"
	, L"D:/Projects/project/chelabinsk/tubes/òðóáà 114_10 141022 11 09.dat"
};

unsigned currentPath = 0;

const wchar_t *setFrame = L"setFrame";
const wchar_t *getFrame = L"getFrame";

HANDLE hgetFrame;
HANDLE hsetFrame;
HANDLE hThread;

FILE *f;
wchar_t *s;
int offset = 0;

//double offsetOfTime[400];
#pragma warning(disable : 4996)
DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
	while(true)
	{
		WaitForSingleObject(hgetFrame, INFINITE);
		 FILE *f = _wfopen(s, L"rb");
		 fseek(f, offset ,SEEK_SET);
		int size = fread(mapped.buf->Frame, 1, sizeof(mapped.buf->Frame), f);
		offset += size;
		if(size < sizeof(mapped.buf->Frame))
		{
			fseek(f, sizeof(mapped.buf->offsetsTime) + 2 * sizeof(int) ,SEEK_SET);
			offset = fread(mapped.buf->Frame, 1, sizeof(mapped.buf->Frame), f);
		}
		fclose(f);
		Sleep(100);
		SetEvent(hsetFrame);
	}
	return 0;
}

}

void EmulLan::Init()
{
	hgetFrame = CreateEvent(NULL, FALSE, FALSE, getFrame);
	hsetFrame = CreateEvent(NULL, FALSE, FALSE, setFrame);
	hThread = CreateThread(NULL, 0, ThreadProc, NULL, CREATE_SUSPENDED, NULL);
}

void EmulLan::Destroy()
{
	CloseHandle(hgetFrame );
	CloseHandle(hsetFrame );
	CloseHandle(hThread );
}

void EmulLan::Start(double &length)
{
	s = path[currentPath % dimention_of(path) ];
	++currentPath;
	 FILE *f = _wfopen(s, L"rb");

	 if(1 == fread(mapped.buf, 2 * sizeof(int), 1, f))
	 {
		 offset = fread(mapped.buf->offsetsTime, 1, sizeof(mapped.buf->offsetsTime), f);
		 zprint(" bufferFrame");
		 length = 0.05 * mapped.buf->countZones;
		 while(0 != ResumeThread(hThread));
		 SetEvent(hgetFrame);
	 }
	 fclose(f);
}
void EmulLan::Stop()
{
	SuspendThread(hThread);
//	fclose(f);
	zprint(" bufferFrame");
}

bool EmulLan::Ready()
{
	return WAIT_OBJECT_0 == WaitForSingleObject(hsetFrame, 3000);
}
void EmulLan::GetBuf(char *frame)
{
	memmove(frame, mapped.buf->Frame, sizeof(mapped.buf->Frame));
	SetEvent(hgetFrame);
}
#endif
