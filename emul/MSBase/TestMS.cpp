#include "stdafx.h"
#include "TestMS.h"
#include "MSBase.hpp"
#include "DebugMess.h"

namespace{
	struct __data__
	{
		int num;
		__data__() : num(0){}
	};
	template<class O, class P>struct __sub__
	{
		void operator()(O *o, P *p)
		{
//			dprint("%S", o->name());
		}
	};
	template<class O, class P>struct __proc__
	{
		void operator()(O &o, P &p)
		{
//			dprint("%d", ++p.num);			
			TL::foreach<typename O::items_list, __sub__>()(&o.items, &p);
		}
	};
}

void TestMS()
{
	MSBase msBase(L"defectoscope.udl");
	if(msBase.IsOpen())
	{
		zprint("  Ok");
		__data__ d;
		Select<TubesStat>((CBase &)msBase).ExecuteLoop<__proc__, __data__>(d);
	}
}