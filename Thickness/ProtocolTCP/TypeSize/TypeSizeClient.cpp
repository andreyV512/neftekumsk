#include "stdafx.h"
#include "..\..\TCPServer\ServerHandler.h"
#include "..\ProtocolTCP\Frame.h"
#include "..\ProtocolTCP\NetClient.h"
#include "typelist.hpp"
#include "..\ProtocolTCP\IPprotocolProcedureList.hpp"
#include "DebugMess.h"

namespace 
{
	struct XXData
	{
		bool ok;
		void Test(wchar_t *s)
		{
			ok = 0 == wcscmp(L"Ok", s);
			dprint("result %d %S\n", ok, s);
		}
	} xxData;
}

namespace TypeSizeProtocol
{
	//namespace
	//{
		struct Stored: public CommonClient
		{
			XXData &item;
			Frame *f;
			Stored(XXData &item, Frame *f, wchar_t *data)
				: item(item)
				, f(f)
			{
				item.ok = false;
				f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, TypeSizeProtocol::Server>::value;
				int len = 1 + wcslen(data);
				wcscpy_s((wchar_t *)f->data.buf, len, data);
				f->length = 2 * len;
				f->proc = Recv<Stored>;
			}

			bool Do()
			{
				item.Test((wchar_t *)f->data.buf);
				f->proc = Stop;
				return true;
			}

			static void *operator new(size_t size, void *ptr)
			{
				return ptr;
			}
			static void operator delete(void *, void *){}
		};		
	//}
	void Client::Do(wchar_t *addr, int port, wchar_t *data)
	{
		Frame *f = Frame::Get();

		new(Frame::AdditionalData<Stored>(f)) Stored(xxData, f, data);
		NetClient::Connect(addr, port, f);
	}

}