#include "stdafx.h"
#include "AppBase.h"
#include <typeinfo>
#include "DebugMess.h"
//-------------------------------------------------------------------
namespace{
template<class O, class P>struct row_table
{
	void operator()(O *, P *p)
	{
		O &o = Singleton<O>::Instance();
		Select<O>(*p).ID(1).Execute(o);
	}
};

template<int N>void DPrint(Holder<N> &d){dprint("%S", (wchar_t *)d);}
	template<class O, class P>struct __default_param__
	{
		void operator()(O *o, P *){}
	};	
	template<class X, class P>struct __default_param__<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			X &x = Singleton<X>::Instance();
		    Select<X>(*base).ID(o->value).Execute(x);
			TL::foreach<typename X::items_list, __default_param__XXX>()(&x.items, base);
		}
	};
	template<class O, class P>struct __default_param__XXX
	{
		void operator()(O *o, P *){}
	};	
	template<class X, class P>struct __default_param__XXX<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			X &x = Singleton<X>::Instance();
		    Select<X>(*base).ID(o->value).Execute(x);
			TL::foreach<typename X::items_list, __default_param__>()(&x.items, base);
		}
	};
}

void AppBase::InitTypeSizeTables(CBase &base)
{
	CurrentParametersTable x;
	Select<CurrentParametersTable>(base).ID(1).Execute(x);
	ParametersTable	&p = Singleton<ParametersTable>::Instance();
	Select<ParametersTable>(base).ID(x.items.get<CurrentID>().value).Execute(p);
	TL::foreach<typename ParametersTable::items_list, __default_param__>()(&p.items, &base);
}

void AppBase::Init()
{
	ParametersBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		TL::foreach<ParametersBase::one_row_table_list, row_table>()((TL::Factory<ParametersBase::one_row_table_list> *)0, &base);

		InitTypeSizeTables(base);
	}
}
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = wcslen(path);
	path[len - 4] = 0;
	return path;
}
//-------------------------------------------------------------------------	
const double &result_undefined = Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value;





