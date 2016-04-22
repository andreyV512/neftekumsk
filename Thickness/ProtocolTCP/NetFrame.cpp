#include "StdAfx.h"
#include "NetFrame.h"
#include "Frame.h"

DWORD WINAPI NetLink_Send(LPVOID f_)
{
	Frame *f = (Frame *)f_;
	WSABUF buf;
	buf.buf = (char *)&f->data;
	buf.len = f->length + sizeof(f->data.id);
	DWORD dwBytes, dwFlags = 0;
	memset(f, 0, sizeof(OVERLAPPED));
	WSASend(f->socket, &buf, 1, &dwBytes, dwFlags, f, f->proc);
	f->proc = NULL;
	return 0;
}

void NetLink::Send(Frame *f)
{
	if(NULL != f->proc)
	{		
		QueueUserWorkItem(NetLink_Send, f, WT_EXECUTEINIOTHREAD);
	}
	else
	{
		closesocket(f->socket);
		Frame::Drop(f);
	}
}

DWORD WINAPI NetLink_Recv(LPVOID f_)
{
	Frame *f = (Frame *)f_;
	WSABUF buf;
	buf.buf = (char *)&f->data;
	buf.len = sizeof(f->data);
	DWORD dwBytes, dwFlags = 0;
	memset(f, 0, sizeof(OVERLAPPED));
	WSARecv(f->socket, &buf, 1, &dwBytes, &dwFlags, f, f->proc);
	f->proc = NULL;
	return 0;
}

void NetLink::Recv(Frame *f)
{
	if(NULL != f->proc)
	{
		QueueUserWorkItem(NetLink_Recv, (LPVOID)f, WT_EXECUTEINIOTHREAD);
	}
	else
	{
		closesocket(f->socket);
		Frame::Drop(f);
	}
}


