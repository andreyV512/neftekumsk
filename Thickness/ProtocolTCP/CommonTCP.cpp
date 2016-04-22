#include "stdafx.h"
#include "CommonTCP.h"
#include "Frame.h"
#include "NetFrame.h"


void CALLBACK CommonClient::Stop(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
{
	Frame *f = (Frame *)lpOverlapped;
	closesocket(f->socket);
	Frame::Drop(f);
}
