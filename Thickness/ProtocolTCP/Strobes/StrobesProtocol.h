#pragma once
#include "CommonTCP.h"

namespace StrobesProtocol
{
	class Server: public CommonServer<Server>
	{
	public:
		static bool Do(Frame *);
	};
	//Слиент не используется
	class Client: public CommonClient
	{
	public:
		static void Do(wchar_t *addr, int port, wchar_t *);
	};
}