#include "stdafx.h"
#include "ThicknessProtocol.h"
#include "Frame.h"
#include "NetFrame.h"
#include "typelist.hpp"
#include "DebugMess.h"

namespace 
{
	struct Thick
	{
		int countZones;
		double data[360];
		char   status[360];
		void Print(int count)
		{
			for(int i = 0; i < count; ++i)
			{
				dprint("%f ", data[i]);
			}
			dprint("\ncount items %d\n", count);
		}
	} thick;
}

namespace ThicknessProtocol
{	
	bool Server::Do(Frame *f)
	{
		Receive *recvData = (Receive *)&f->data.buf;

		int offs = recvData->from;
		int count = recvData->count;

		double *dta = (double *)((char *)recvData + sizeof(Receive));
		char *st = (char *)(dta + count);

		if(offs + count <= dimention_of(thick.status))
		{
			memmove(&thick.data[offs], dta, count * sizeof(double));
			memmove(&thick.status[offs], st, count * sizeof(char));
		}

		if(eLast == recvData->status)
		{
			thick.Print(offs + count);
			return false;
		}
		else
		{
			Response *r = (Response *)&f->data.buf;
			r->status = eOk;
			r->Next = offs + count;
			f->length = sizeof(Response);
			dprint("server %d\n", r->Next);
		}
		return true;
	}
}