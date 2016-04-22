#include "StdAfx.h"
#include "AsyncServer.h"
#include <winsock2.h>
#include "Frame.h"
#include "NetFrame.h"
#include "ServerHandler.h"
#include "DebugMess.h"

namespace
{
	struct NetServer
	{
		SOCKET listenSocket;

		bool terminated;
		NetServer() : terminated(false)
		{}

		~NetServer()
		{
			terminated = true;
		}

		void Init(int port)
		{
			listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (listenSocket == INVALID_SOCKET)
			{
				dprint("socket function failed with error: %u\n", WSAGetLastError());
			}

			sockaddr_in service;
			service.sin_family = AF_INET;
			service.sin_addr.s_addr = htonl(INADDR_ANY);
			service.sin_port = htons(port);

			int res = bind(listenSocket, (SOCKADDR *)&service, sizeof(service));
			if (res == SOCKET_ERROR) {
				dprint("bind failed with error %u\n", WSAGetLastError());
				closesocket(listenSocket);
			}

			if (listen(listenSocket, 5) != SOCKET_ERROR)
			{
				QueueUserWorkItem(ListenDo, this, WT_EXECUTELONGFUNCTION);
			}
			else
			{
				dprint("listen function failed with error: %d", WSAGetLastError());
			}
		}

		static DWORD WINAPI ListenDo(LPVOID self_)
		{
			NetServer *self = (NetServer *)self_;
			DWORD dwFlags = 0;
			while (!self->terminated)
			{
				SOCKET socket = accept(self->listenSocket, NULL, NULL);

				if (INVALID_SOCKET == socket) break;
				Frame *f = Frame::Get();
				f->socket = socket;
				f->proc = RecvAcceptComplete;
				NetLink::Recv(f);
			}
			closesocket(self->listenSocket);
			return 0;
		}

		static void CALLBACK RecvAcceptComplete(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
		{
			Frame *f = (Frame *)lpOverlapped;
			if (dwError != 0 || dwTransferred == 0)
			{
				closesocket(f->socket);
				Frame::Drop(f);
				return;
			}

			Server::Proc proc = Server::Handler(f->data.id);
			if (NULL == proc)
			{
				closesocket(f->socket);
				Frame::Drop(f);
			}
			else
			{
				f->length = dwTransferred;
				proc(dwError, dwTransferred, lpOverlapped, dwFlags);
			}
		}
	};
}

void AsyncServer::Create(int port)
{
	static NetServer x;
	x.Init(port);
}

void Destroy(){}

