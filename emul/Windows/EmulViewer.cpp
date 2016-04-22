#include "stdafx.h"
#include "EmulViewer.h"
#include <MMSystem.h>
#include <typeinfo>
#include "ArchiveEvents.h"
#include "EmulLan.h"

#include "DebugMess.h"
using namespace Gdiplus;
//------------------------------------------------------------------------
namespace{
template<class O, class P> struct __init_sq__
{
	void operator()(O *, P *)
	{
	}
};
template<int N, class P> struct __init_sq__<SQ<N>, P>
{
	typedef SQ<N> O;
	void operator()(O *o, P *p)
	{
		o->port = &p->inputPort;
		o->tube = &p->chart.items.get<Tube>();
	}
};
template<int N, class P> struct __init_sq__<Y<N>, P>
{
	typedef Y<N> O;
	void operator()(O *o, P *p)
	{
		o->port = &p->outputPort;
	//	o->tube = &p->chart.items.get<Tube>();
	}
};
}
EmulViewer::EmulViewer()
	: backScreen(NULL)	
	, chart(backScreen)
	, start(false)
	, speed(0.5)
	, offsTube(0)
	, forward(true)
	, backward(false)
	, inputPort(archiveEvents.map->input)
	, lastInputPort(0)
	, outputPort(archiveEvents.map->output)
	, lastOutputPort(0)
	, startFrame(false)
{
	chart.minAxesX = 0;
	chart.maxAxesX = 20;
	chart.offsetAxesLeft = 10;
	Arrow &a = chart.items.get<Arrow>();
	a.forward = &forward;
	a.backward = &backward;
	TL::foreach<type_items_list, __init_sq__>()(&chart.items, this);
}
//-----------------------------------------------------------------------------
void EmulViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
}
//--------------------------------------------------------------------------
void EmulViewer::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < m.Width || backScreen->GetHeight() < m.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(m.Width, m.Height);
		}
	}
	else if(m.Width > 0 && m.Height > 0)
	{
		backScreen = new Bitmap(m.Width, m.Height);
	}
	else
	{
		return;
	}
	Graphics g(backScreen);
	chart.rect.right = m.Width;
	chart.rect.bottom = m.Height;
	chart.Draw(g);
}
//-------------------------------------------------------------------------
void EmulViewer::operator()(TTimer &m)
{
	switch(m.ID)
	{
	case ID_TIMER1:
		{
			int t = timeGetTime();
			double xspeed = 0 != (inputPort &(1 << inverterRun))
				? speed
				: 0;
			double d = 0.001 * xspeed * (t - lastTime); 
			if(forward && !backward) offsTube += d; else if(!forward && backward) offsTube -= d;
			Tube &tube = chart.items.get<Tube>();
			tube.offsX = offsTube;
			lastTime = t;
			RECT r;
			GetClientRect(m.hwnd, &r);
			TSize s = {m.hwnd, WM_SIZE, 0, (WORD)r.right, (WORD)r.bottom};
			SendMessage(MESSAGE(s));
			InvalidateRect(m.hwnd, NULL, TRUE);
			Do();
		}
		break;
	}
}
//-------------------------------------------------------------------------
void EmulViewer::Start()
{
	if(!start)
	{
		start = true;
		lastTime = timeGetTime();
        SetTimer(hWnd, ID_TIMER1, 10, NULL);
	}
}
void EmulViewer::Stop()
{
	if(start)
	{
		start = false;
		KillTimer(hWnd, ID_TIMER1);
	}
}
namespace{
	template<class List, template<int>class W>struct __select_sg_list__;
	template<class Head, class Tail, template<int>class W>struct __select_sg_list__<Tlst<Head, Tail>, W>
	{
		typedef typename __select_sg_list__<Tail, W>::Result Result;
	};
	template<int N, class Tail, template<int>class W>struct __select_sg_list__<Tlst<W<N>, Tail>, W>
	{
		typedef Tlst<W<N>, typename __select_sg_list__<Tail, W>::Result> Result;
	};
    template<template<int>class W>struct __select_sg_list__<NullType, W>
	{
		typedef NullType Result;
	};

	struct __test_bits_data__
	{
		unsigned bits, xorBits;
		bool start;
	};

	struct __test_input_output_bits_data__
	{
		unsigned &input_bits, output_bits, xorBits;
		__test_input_output_bits_data__(unsigned &inputPort, unsigned outputPort, unsigned xorBits)
			: input_bits  (inputPort)
			, output_bits (outputPort)
			, xorBits	  (xorBits)
		{}
	};

	template<class O, class P>struct __test_bits__
	{
		void operator()(O *o, P *p)
		{
			if(o->bit & p->xorBits)
			{
				bool b = 0 != (o->bit & p->bits);
				dprint("%s %d", typeid(O).name(), b);
			}
		}
	};

	template<class P>struct __test_bits__<SQ<1>, P>
	{
		typedef SQ<1> O;
		void operator()(O *o, P *p)
		{
			if(o->bit & p->xorBits)
			{
				bool b = 0 != (o->bit & p->bits);
				dprint("%s %d", typeid(O).name(), b);
				p->start |= b;
			}
		}
	};
	template<class P>struct __test_bits__<SQ<2>, P>
	{
		typedef SQ<2> O;
		void operator()(O *o, P *p)
		{
			if(o->bit & p->xorBits)
			{
				bool b = 0 != (o->bit & p->bits);
				dprint("%s %d", typeid(O).name(), b);
				p->start |= b;
			}
		}
	};
	
	template<class O, class P>struct __out_bits__
	{
		void operator()(O *o, P *p)
		{
			if(o->bit & p->xorBits)
			{
				bool b = 0 != (o->bit & p->output_bits);
				dprint("%s %d", typeid(O).name(), b);
			}
		}
	};

    struct RunInverter
	{
		HANDLE h;
		static const unsigned bit = 1 << 4;
		unsigned *inputs;
		RunInverter()
		    : h(INVALID_HANDLE_VALUE)
		{}
		static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
		{
			RunInverter *o = (RunInverter *)lpParameter;
			*o->inputs |= bit;
			DeleteTimerQueueTimer(NULL, o->h, NULL);
			o->h = INVALID_HANDLE_VALUE;
		}
	} runInverter;
	template<class P>struct __out_bits__<Y<sTF>, P>
	{
		typedef Y<sTF> O;
		void operator()(O *o, P *p)
		{
			if(o->bit & p->xorBits)
			{
				bool b = 0 != (o->bit & p->output_bits);
				if(b)
				{
					 runInverter.inputs = &p->input_bits;					
					 CreateTimerQueueTimer(&runInverter.h, NULL, RunInverter::WaitOrTimerCallback, &runInverter, 5000, 0, WT_EXECUTEONLYONCE);					
				}
			}
		}
	};
}

namespace{
	HANDLE hTimer;
	static VOID CALLBACK WaitOrTimerCallbackX(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		EmulViewer *x = (EmulViewer *)lpParameter;
		DeleteTimerQueueTimer(NULL, hTimer, NULL);
		//x->chart.items.get<Tube>().Restart();
		x->offsTube = 0;
	}
}

void EmulViewer::Do()
{
	if(0 == (outputPort & chart.items.get<Y<sTF> >().bit))
	{
		inputPort &=~(1 << inverterRun);
	}
	if(inputPort != lastInputPort)
	{
		typedef __select_sg_list__<type_items_list, SQ>::Result list;
		unsigned xorBits = inputPort ^ lastInputPort;
		__test_bits_data__ data = {inputPort, xorBits, false};
		TL::foreach<list, __test_bits__>()(&chart.items, &data);
		if(data.start != startFrame)
		{
			startFrame = data.start;
			if(startFrame)
			{
				emulLan.Start(chart.items.get<Tube>().length);
				zprint(" length %f", chart.items.get<Tube>().length);
			}
			else
			{
				emulLan.Stop();
				CreateTimerQueueTimer(&hTimer, NULL, WaitOrTimerCallbackX, this, 7000, 0, WT_EXECUTEONLYONCE);
			}
		}
		lastInputPort = inputPort;
	}
	//dprint("%x", outputPort);
	if(outputPort != lastOutputPort)
	{
		typedef __select_sg_list__<type_items_list, Y>::Result list;
		unsigned xorBits = outputPort ^ lastOutputPort;
		__test_input_output_bits_data__ data(inputPort, outputPort, xorBits);
		TL::foreach<list, __out_bits__>()(&chart.items, &data);
		lastOutputPort = outputPort;
	}
//	 /*
	
	//*/
}

