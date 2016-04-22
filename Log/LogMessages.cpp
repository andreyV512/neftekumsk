#include "stdafx.h"
#include "LogMessages.h"
#include <Windows.h>
#include "typelist.hpp"
#include <stdio.h>


#pragma warning(disable : 4996)


namespace LogMess
{
	struct Loc
	{
		unsigned backColor, textColor;
		void(*proc)(char *, double);
	};
	template<class T, class C>struct GenFunc;
	template<class C>struct GenFunc<void, C>
	{
		static void Func(char *buf, double)
		{
			sprintf(buf, C::mess());
		}
	};
	
	template<class C>struct GenFunc<int, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), (int)val);			
		}
	};
	template<class C>struct GenFunc<double, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), val);			
		}
	};
	template<class C>struct GenFunc<bool, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), 0.0 != val ? "¬ À" : "Œ“ À");			
		}
	};
	template<class C>struct GenFunc<const char *, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), (char *)*(unsigned *)&val);			
		}
	};
	
	template<int i = 0, int stop = max_row>struct Item: Item<i + 1, max_row>
	{
		Item()
		{
			arr[i].backColor = Row<i>::backColor;
			arr[i].textColor = Row<i>::textColor;
			arr[i].proc = GenFunc<Row<i>::type, Row<i> >::Func;
		}
	};
	template<>struct Item<max_row, max_row>
	{
		Loc arr[max_row];
	};
	struct FactoryMessages::Inner
	{
		unsigned start;
		Item<> items;
		static Inner &Instance(){static Inner x; return x;}
	};
	FactoryMessages::FactoryMessages()
		: inner(Inner::Instance())
	{
	}
	void FactoryMessages::StartTime()
	{
		inner.start = GetTickCount();
	}
	bool FactoryMessages::Color(int i, unsigned &backColor, unsigned &textColor)
	{
		if(i < max_row) 
		{
			backColor = inner.items.arr[i].backColor;
			textColor = inner.items.arr[i].textColor;
			return true;
		}
		return false;
	}
	bool FactoryMessages::Text(int i, char *buf, double val)
	{
		if(i < max_row) 
		{
			inner.items.arr[i].proc(buf, val);
			return true;
		}
		return false;
	}
	FactoryMessages &FactoryMessages::Instance()
	{
		static FactoryMessages x; return x;
	}
}
