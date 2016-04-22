#include "stdafx.h"
#include "InitTcp.h"
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

InitTcp::InitTcp()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

InitTcp::~InitTcp()
{
	 WSACleanup();
}
