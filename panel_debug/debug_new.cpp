// debug_new.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <clocale>
#include "DebugMess.h"


int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_CTYPE, "rus");
	ViewerDebugMess m;

	printf("отладочная панель\n");
	while(true)
	{
		char *c = m.get();
		if(NULL != c)printf(c);
		Sleep(10);
	}

	return 0;
}

