#pragma once
//D:\work\NefteKumsk\Buran9000\Common\ProtocolTCP\TypeSize\TypeSizeProtocol.h
//D:\work\NefteKumsk\Buran9000\Common\ProtocolTCP\CommonTCP.h
#include "../ProtocolTCP/CommonTCP.h"

namespace TypeSizeProtocol
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