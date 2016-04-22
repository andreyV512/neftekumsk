#include "StdAfx.h"
#include "NetClient.h"
#include "NetFrame.h"
#include "Frame.h"
#include "DebugMess.h"
#include <stdlib.h>

#pragma warning(disable : 4996)
bool NetClient::Connect(wchar_t *addr, int port, Frame *f)
{
	f->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
    if (f->socket == INVALID_SOCKET) 
	{
        dprint("socket function failed with error: %u\n", WSAGetLastError());
		Frame::Drop(f);
		return false;
    }

	sockaddr_in service;
	service.sin_family = AF_INET;
	char _addr[20];
	wcstombs(_addr, addr, 20);
    service.sin_addr.s_addr = inet_addr(_addr);
    service.sin_port = htons(port);

	if(INVALID_SOCKET == connect(f->socket, (const sockaddr *)&service, sizeof(sockaddr_in)))
	{
		closesocket(f->socket);
		Frame::Drop(f);
		return false;
	}
	NetLink::Send(f);
	return true;
}
