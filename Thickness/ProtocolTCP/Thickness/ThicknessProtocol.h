#pragma once
#include "CommonTCP.h"

namespace ThicknessProtocol
{
	enum Status { eOk, eNew, eNext, eLast, eErr };
	struct Receive
	{
		int from, count;
		Status status;
	};
	struct Response
	{
		int from, count;
		Status status;
		int Next;
	};
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	struct Client
	{
		static bool Do(wchar_t *addr, int port);
	};
}