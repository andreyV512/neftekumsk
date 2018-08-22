#include "stdafx.h"
#include "ThicknessProtocol.h"
#include "ServerHandler.h"
#include "Frame.h"
#include "NetClient.h"
#include "stdafx.h"
#include "CommonTCP.h"
#include "IPprotocolProcedureList.hpp"
#include "typelist.hpp"
#include "DebugMess.h"
#include "ThicknessData.h"
#include "PrimaryData.h"

namespace{
struct ThicknessD
{
	unsigned size;
	double data[2][60];
	char status[60];
} thicknessD;
}
namespace ThicknessProtocol
{
	struct Stored: public CommonClient
	{
		int to;
		ThicknessD	&item;
		ThicknessData &thickness;		
		Frame *f;
		Stored(Frame *f)
			: to(sizeof(thicknessData))
		    , item(thicknessD)
			, thickness(thicknessData)
			, f(f)
		{
			memmove(&item, &thickness, sizeof(item));
			for(int i = 0; i < 60; ++i)
			{
				char res = 0;
				switch(thickness.status[i])
				{
				case PrimaryData::Defect        : res = 5; break;
				case PrimaryData::Treshold2Class: res = 4; break;
				case PrimaryData::Nominal       : res = 1; break;
				case PrimaryData::DeathZone     : res = 2; break;
				case PrimaryData::Treshold3Class: res = 6; break;
				}
				item.status[i] = res;
			}
			Response *res = (Response *)f->data.buf;
			res->count = (sizeof(f->data.buf) - sizeof(Receive));
			res->Next = 0;
			f->data.id = TL::IndexOf<__ip_protocol_procedure_list__, ThicknessProtocol::Server>::value; 
		}
		
		bool Do()
		{
			Response res = *(Response *)f->data.buf;
			Receive *r = (Receive *)f->data.buf;
			if(res.Next + res.count < to)
			{
				r->from = res.Next;
				r->count = res.count;
				f->proc = Recv<Stored>;
				r->status = eOk;
			}
			else if(res.Next < to)
			{
				r->from = res.Next;
				r->count = to - res.Next; 
				f->proc = Recv<Stored>;
				r->status = eLast;
			}
			else
			{
				return false;
			}

			char *ofs = f->data.buf + sizeof(Receive);
			
			char *d1 = &((char *)&item)[r->from];
			memmove(ofs, d1, r->count);
			f->length = r->count + sizeof(Receive);

			return true;
		}
		static void *operator new(size_t size, void *ptr)
		{
			 return ptr;
		}
		static void operator delete(void *, void *){}
	};

	bool Client::Do(wchar_t *addr, int port)
	{
	    Frame *f = Frame::Get();

		Stored *s = new(Frame::AdditionalData<Stored>(f)) Stored(f);

		if(s->Do())
		{
			dprint("start client \n");
			return NetClient::Connect(addr, port, f);
		}
		else
		{
			Frame::Drop(&f);
		}
		return false;
	}
}

