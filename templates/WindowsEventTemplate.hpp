#pragma once
#include <windows.h>
#include "message.h"
#include "typelist.hpp"
namespace{
template<class T>struct TypeToEvent
{
	/*
	необходимо для типа Т сопоставить сообщение windows
	*/
	typedef typename T::_must_match_the_class_of_message_windows nonexist;
};

template<>struct TypeToEvent<TMouseMove>{static const int value = WM_MOUSEMOVE;};//<-------см. ниже комментарий
template<>struct TypeToEvent<TSize>{static const int value = WM_SIZE;};
template<>struct TypeToEvent<TPaint>{static const int value = WM_PAINT;};
template<>struct TypeToEvent<TActivate>{static const int value = WM_ACTIVATE;};
template<>struct TypeToEvent<TUser>{static const int value = WM_USER;};
template<>struct TypeToEvent<TDestroy>{static const int value = WM_DESTROY;};
template<>struct TypeToEvent<TLButtonDown>{static const int value = WM_LBUTTONDOWN;};
template<>struct TypeToEvent<TRButtonDown>{static const int value = WM_RBUTTONDOWN;};
template<>struct TypeToEvent<TLButtonUp>{static const int value = WM_LBUTTONUP;};
template<>struct TypeToEvent<TCommand>{static const int value = WM_COMMAND;};
template<>struct TypeToEvent<TNotify>{static const int value = WM_NOTIFY;};
template<>struct TypeToEvent<TGetMinMaxInfo>{static const int value = WM_GETMINMAXINFO;};
template<>struct TypeToEvent<TLButtonDbClk>{static const int value = WM_LBUTTONDBLCLK;};
template<>struct TypeToEvent<TKeyDown>{static const int value = WM_KEYDOWN;};
template<>struct TypeToEvent<TMouseWell>{static const int value =  WM_MOUSEWHEEL;};
template<>struct TypeToEvent<TMoving>{static const int value =  WM_MOVING;};
template<>struct TypeToEvent<TSizing>{static const int value =  WM_SIZING;};
template<>struct TypeToEvent<TMessage>{static const int value =  WM_MESSAGE;};
template<>struct TypeToEvent<TTimer>{static const int value =  WM_TIMER;};

template<class O, class P>class IsFuncExist
{
	template<class T, T>struct Helper{};
	template<class Z>static double Is(O *, Helper<unsigned(O::*)(Z &), &O::operator()> * = NULL);
	template<class Z>static double Is(O *, Helper<void(O::*)(Z &), &O::operator()> * = NULL);
	template<class Z>static char Is(...);
public:
	static const bool value = sizeof(Is<P>((O *)0)) == sizeof(double);
	typedef typename TL::_if<value, P, NullType>::Result Result;
};

template<class List, class T, class tmp = NullType>struct AddTypeEvent;
template<class Head, class Tail, class T, class tmp>struct AddTypeEvent<Tlst<Head, Tail>, T, tmp>
{
	typedef typename AddTypeEvent<Tail, T, typename TL::Append<tmp, typename IsFuncExist<T, Head>::Result>::Result>::Result Result;
};
template<class Head, class T, class tmp>struct AddTypeEvent<Tlst<Head, NullType>, T, tmp>
{
	typedef typename TL::Append<tmp, typename IsFuncExist<T, Head>::Result>::Result Result;
};

typedef TL::MkTlst<
	TMouseMove, TSize, TPaint, TActivate, TNotify
	, TLButtonDown, TLButtonUp, TLButtonDbClk, TKeyDown 
	, TUser, TCommand, TDestroy
	, TGetMinMaxInfo, TMouseWell, TRButtonDown, TMoving, TSizing
	, TMessage, TTimer
>::Result type_events_all_list; //<-------см. ниже комментарий

template<bool >struct Wapper
{
	template<class O, class P>unsigned operator()(O &o, P *p)
	{
		p->obj(o);
		return -1;
	}
};

template<>struct Wapper<false>
{
	template<class O, class P>unsigned operator()(O &o, P *p)
	{
		return p->obj(o);
	}
};

template<class T>struct EventHandler
{
	template<class O, class P>struct loc
	{
		template<class T, T>struct Helper{};
		template<class Z>static double Is(T *, Helper<void(T::*)(Z &), &T::operator()> * = NULL);
	    template<class Z>static char Is(...);
		unsigned operator()(O *, P *p)
		{
			if(TypeToEvent<O>::value != p->mess.uMsg) return 0;
			return Wapper<sizeof(Is<O>((T *)0)) == sizeof(double)>()((O &)p->mess, p);
		}
	};
	template<class List, class T>struct TestNotNullType
	{
		typedef List Result;
	};
	template<class T>struct TestNotNullType<NullType, T>
	{
		/*
		класс должен иметь обработчик в виде- unsigned T::operator()(XXX &) или void T::operator()(XXX &);
		где: XXX - TMouseMove, TSize, TPaint, TActivate(добавить при необходимости в список type_events_all_list, см. выше)
		*/
		typedef typename T::_class_does_not_have_any_handler nonexist;
	};
	TMessage &mess;
	T &obj;
public:
	EventHandler(TMessage &mess, T &obj)
		: mess(mess)
		, obj(obj)
	{}
	unsigned operator()()
	{
		typedef typename TestNotNullType<typename AddTypeEvent<type_events_all_list, T>::Result, T>::Result list;
		return TL::find_ret<list, loc>()((TL::Factory<list> *)0, this);
	}
};
}
//------------------------------------------------------------------------------------------------
template<class T>class Viewer
{
	template<bool>struct Create
	{
		unsigned operator()(TCreate &l)
		{
			T *o = (T *)l.create->lpCreateParams;
			o->hWnd = l.hwnd;
			SetWindowLongPtr(l.hwnd, GWLP_USERDATA, (LONG)o);
			return 0;
		}
	};
	template<>struct Create<true>
	{
		unsigned operator()(TCreate &l)
		{
			T *o = (T *)l.create->lpCreateParams;
			o->hWnd = l.hwnd;
			SetWindowLongPtr(l.hwnd, GWLP_USERDATA, (LONG)o);
			return (*o)(l);
		}
	};
public:
	static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		T *o = (T *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if(NULL != o)
		{
			unsigned result = EventHandler<T>((TMessage &)hWnd, *o)();
			if(0 != result) 
			{
				if(-1 == result) return 0;
				return result;
			}
		}	
		else if(message == WM_CREATE)
		{
			return Create<IsFuncExist<T, TCreate>::value>()((TCreate &)hWnd);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
};
//--------------------------------------------------------------------------------------------------------------