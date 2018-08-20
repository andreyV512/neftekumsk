#include "stdafx.h"
#include "CommunicationWapper.h"
#include "AppBase.h"
#include "..\Windows\SelectTypeSize.h"
#include "DebugMess.h"
#include "App.h"
#include "Automat.h"
#include "GetHostByName.h"

#include "ProtocolTCP\Thickness\ThicknessProtocol.h"

//struct IntToIPAddr
//{
//	wchar_t buf[16];
//	wchar_t *operator()(int addr)
//	{
//		wsprintf(buf, L"%d.%d.%d.%d"
//		, ((unsigned char *)&addr)[3]
//		, ((unsigned char *)&addr)[2]
//		, ((unsigned char *)&addr)[1]
//		, ((unsigned char *)&addr)[0]
//		);
//		return buf;
//	}
//};

bool TcpServerTypeSize(wchar_t *receiveData)
{	
	bool result = app.ChangeTypeSize(receiveData);
	if(result)
	{
		zprint("");
		automat.AutomatMode();
	}
	return result;
}

bool SendThickhess()
{
	ThicknessProtocol::Client().Do(
		GetHostByName()(Singleton<IPAddressTable>::Instance().items.get<IPName>().value)
		, Singleton<IPAddressTable>::Instance().items.get<IPPort>().value
		);
	return true;
}