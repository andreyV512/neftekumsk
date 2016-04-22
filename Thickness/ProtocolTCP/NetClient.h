#pragma once
#include "NetFrame.h"

class NetClient: public NetLink
{
	NetClient();
	~NetClient();
public:	
	static bool Connect(wchar_t *addr, int port, struct Frame *);	
};

