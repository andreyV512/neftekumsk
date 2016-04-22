// TEST_1730.cpp : Defines the entry point for the console application.
//D:\new\Thickness9000\1730\Device1730.cpp
//D:\new\Thickness9000\TEST_1730\TEST_1730.cpp

#include "stdafx.h"
#include "Device1730.h"
#include <conio.h>


int _tmain(int argc, _TCHAR* argv[])
{
	Device1730 d;
	d.Init(0);
	unsigned last = 0x0BedC0de;
	if(d.IsOpen())
	{
		printf("Init Ok!!!!\n");
		while(true)
		{
			if(_kbhit() && 27 == _getch()) 
			{
				d.Destroy();
				return  0;
			}
			unsigned x = d.Read();
			if(x != last)
			{
				printf("%8x   %8x\n", x, last);
				last = x;
			}
			Sleep(100);
		}
	}
	else
	{
		printf("error\n");
	}
	getchar();
	return 0;
}

