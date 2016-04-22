#pragma once
#include "message.h"
class NotifyEvent
{
	virtual LRESULT Notify(TNotify &) = 0;
	static LRESULT Do(TNotify &m)
	{
		NotifyEvent *x = (NotifyEvent *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
		if(0 == IsBadReadPtr(x, sizeof(x)))return x->Notify(m);
		return 0;
	}
};