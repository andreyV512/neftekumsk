#include "stdafx.h"
#include "TypeSizeProtocol.h"
#include "../Frame.h"
#include "../NetFrame.h"
#include "CommunicationWapper.h"
#include "DebugMess.h"
#include "AppBase.h"

namespace TypeSizeProtocol
{
	struct Data
	{
		wchar_t Ok[4];
		double tresholdKlass1;
		double tresholdKlass2;
		double tresholdKlass3;
	};
	bool Server::Do(Frame *f)
	{
		wchar_t *s = (wchar_t *)f->data.buf;
		Data *d = (Data *)f->data.buf;
		if(TcpServerTypeSize(s))
		{
			ThresholdsTable::TItems &i = Singleton<ThresholdsTable>::Instance().items;
			wcsncpy_s(d->Ok , L"Ok", 3);
			d->tresholdKlass1 = i.get<Border2Class>().value;
			d->tresholdKlass2 = i.get<Border3Class>().value;
			d->tresholdKlass3 = i.get<BorderDefect>().value;
		}
		else
		{
			wcsncpy_s(d->Ok , L"Err", 4);
		}

		f->length = sizeof(Data);
		f->proc = Stop;
		return true;
	}
}