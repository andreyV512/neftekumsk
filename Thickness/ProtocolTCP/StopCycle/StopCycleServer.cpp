#include "stdafx.h"
#include "StopCycleProtocol.h"
#include "../Frame.h"
#include "../NetFrame.h"
#include "DebugMess.h"
#include "Automat.h"

namespace StopCycleProtocol
{
	bool Server::Do(Frame *f)
	{
		wchar_t *s = (wchar_t *)f->data.buf;
		
		wchar_t *ret =  true//0 == wcscmp(L"1234567890", s)
			? L"Ok"
			: L"Err"
			;

		 automat.StopMode();

		dprint("receive %S\n");
		
		int len = 1 + wcslen(ret);
		wcscpy_s((wchar_t *)f->data.buf, len, ret);		
		f->length = 2 * len;
		f->proc = Stop;
		return true;
	}
}