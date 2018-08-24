// TestClientTCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "InitTCP.h"
#include "CommunicationWapper.h"
#include <stdio.h>
//#include "typelist.hpp"
#include "AppBase.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Singleton<IPAddressTable>::Instance().items.get<IPName>().value = L"ANDREW";
	Singleton<IPAddressTable>::Instance().items.get<IPPort>().value = 5555;
	InitTcp initTCP;
	if(SendThickhess())
	{
		printf("Ok");
	}
	else
	{
		printf("Err");
	}
	getchar();
	return 0;
}

