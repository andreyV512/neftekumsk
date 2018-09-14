#include "stdafx.h"
#include "SQEvents.h"
#include "ArchiveEvents.h"
#include "Device1730.h"

#include "DebugMess.h"

namespace 
{
	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			o->hEvent = CreateEvent(NULL, false, false, o->Name());
			//zprint("FALSE, FALSE");
		}
	};
	template<class P>struct __init__<SQEvents::Event<SQEvents::AutomaticMode>, P>
	{
		typedef SQEvents::Event<SQEvents::AutomaticMode> O;
		void operator()(O *o, P *p)
		{
			o->hEvent = CreateEvent(NULL, TRUE, FALSE, o->Name());
			//zprint("TRUE, FALSE");
		}
	};
	template<class T, class P>struct __init__<SQEvents::Event<SQEvents::On<T> >, P>
	{
		typedef SQEvents::Event<SQEvents::On<T> > O;
		void operator()(O *o, P *p)
		{
			o->hEvent = CreateEvent(NULL, TRUE, FALSE, o->Name());
			//zprint("TRUE, FALSE");
		}
	};
	template<class T, class P>struct __init__<SQEvents::Event<SQEvents::Off<T> >, P>
	{
		typedef SQEvents::Event<SQEvents::Off<T> > O;
		void operator()(O *o, P *p)
		{
			o->hEvent = CreateEvent(NULL, TRUE, FALSE, o->Name());
			//zprint("TRUE, FALSE");
		}
	};
//------------------------------------------------------------------------------------
	template<class List>struct __on_list__;
	template<class Head, class Tail>struct __on_list__<Tlst<SQEvents::Event<SQEvents::On<Head> >, Tail> >
	{
		typedef Tlst<SQEvents::Event<SQEvents::On<Head> >, typename __on_list__<Tail>::Result> Result;
	};	
	template<class Head, class Tail, template<class >class Wapper>struct __on_list__<Tlst<SQEvents::Event<Wapper<Head> >, Tail> >
	{
		typedef typename __on_list__<Tail>::Result Result;
	};
	template<class Head, class Tail>struct __on_list__<Tlst<Head, Tail> >
	{
		typedef typename __on_list__<Tail>::Result Result;
	};
	template<>struct __on_list__<NullType>
	{
		typedef NullType Result;
	};
#if 0
	template<class O, class P>struct __do__
	{
		void operator()(O *o, P *p)
		{
			if(o->bit & p->msk)
			{
				if(o->bit & p->input) 
				{
					SetEvent(o->hEvent);
					p->last |= o->bit;
				}
				else
				{
					typedef SQEvents::Event<SQEvents::Off<typename O::inner_type> > T;
					SetEvent(p->items.get<T>().hEvent);
					p->last &= ~o->bit;
				}
			}
		}
	};
#else
	template<class O, class P>struct __do__
	{
		bool operator()(O *o, P *p)
		{
			if(o->bit & p->msk)
			{
				if(o->bit & p->input) 
				{
					o->Do();
					p->last |= o->bit;
					//dprint("on %s %d", typeid(O).name(), o->bit);
				}
				else
				{
					typedef SQEvents::Event<SQEvents::Off<typename O::inner_type> > T;
					p->items.get<T>().Do();
					p->last &= ~o->bit;
				//	dprint("off %s %d", typeid(O).name(), o->bit);
				}
				p->msk &= ~o->bit;
			}
			return 0 != p->msk;
		}
	};
#endif
}
//------------------------------------------------------------------------------------
#pragma warning(disable : 4355)
SQEvents::SQEvents(): items(*this) 
{}	   
//----------------------------------------------------------------------------------
void SQEvents::Init()
{
	TL::foreach<type_items, __init__>()(&items, (int *)0);
	CloseHandle(CreateThread(NULL, 0, Proc, this, 0, NULL));
}
//----------------------------------------------------------------------------------
void SQEvents::Destroy()
{
}
//----------------------------------------------------------------------------------
void SQEvents::Do()
{
	//инверсия бит
	unsigned inverseBits = items.get<Event<On<RunBitIn> > >().bit;
	while(true)
	{
		switch(WaitForSingleObject(App::ProgrammExitEvent, 10))
		{
		case WAIT_TIMEOUT:
			input = device1730.Read();
			input ^= inverseBits;
			msk = input ^ last;
			last = input;
			if(0 != msk)
			{
				TL::find<__on_list__<type_items>::Result, __do__>()(&items, this);
				//dprint("input %8x", input);
			}
			break;
		case WAIT_OBJECT_0: return;
		}
	}

}
 DWORD WINAPI SQEvents::Proc(PVOID x)
 {
	((SQEvents *)x)->Do();
	return 0;
 }
//----------------------------------------------------------------------------------
void SQEvents::Reset()
{
	last = 0;
}
//------------------------------------------------------------------------------------
namespace
{
	CRITICAL_SECTION cs;
	struct InitCriticalSection
	{
		InitCriticalSection(CRITICAL_SECTION &cs)
		{
			InitializeCriticalSection(&cs);
		}
	};
	volatile InitCriticalSection initCriticalSection(cs) ;
}
void SQEvents::SetBits(unsigned d)
{
	 EnterCriticalSection(&cs);
	 unsigned t = device1730.ReadOutput();
	 t |= d;
	 device1730.Write(t);
	 LeaveCriticalSection(&cs);
}
void SQEvents::ClrBits(unsigned d)
{
	EnterCriticalSection(&cs);
	unsigned t = device1730.ReadOutput();
	t &= ~d;
	device1730.Write(t);
	LeaveCriticalSection(&cs);
}
