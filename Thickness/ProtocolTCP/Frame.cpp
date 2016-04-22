#include "stdafx.h"
#include "Frame.h"
#include "DebugMess.h"

CRITICAL_SECTION cs;
Frame *head = NULL;

namespace
{
	struct Init
	{
		Init()
		{
			InitializeCriticalSection(&cs);
		}
	};
	volatile Init init;
}

Frame::Frame()
{}
static int counter = 0;
static int drop = 0;
static int nex = 0;
Frame *Frame::_Get()
{
	Frame *result = NULL;
	EnterCriticalSection(&cs);
	result = head;
	if (NULL != result) 
	{
			head = head->next;
			dprint(" next Frames %d \n", ++nex);
	}
	else
	{
		result = new Frame;
		result->next = NULL;
		dprint(" Count Frames %d \n", ++counter);
	}
	LeaveCriticalSection(&cs);
	return result;
}

void Frame::Drop(void *data)
{
	EnterCriticalSection(&cs);
	((Frame *)data)->next = head;
	head = (Frame *)data;
	dprint(" Drop Frames %d %d\n", counter, nex);
	--nex;
	LeaveCriticalSection(&cs);
}