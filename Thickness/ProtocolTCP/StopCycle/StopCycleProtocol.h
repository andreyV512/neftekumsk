#pragma once
#include "../ProtocolTCP/CommonTCP.h"

namespace StopCycleProtocol
{
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	class Client: public CommonClient
	{
	public:
		static void Do(wchar_t *addr, int port, wchar_t *);
	};
}