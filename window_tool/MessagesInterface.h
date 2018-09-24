#pragma once
#include "message.h"
class TEvent
{
public:
	virtual void Do(TCommand &){}
	virtual void Do(TNotify &){}
};