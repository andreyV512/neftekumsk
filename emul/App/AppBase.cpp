#include "stdafx.h"
#include "AppBase.h"

#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif	
//-------------------------------------------------------------------
namespace{
template<class O, class P>struct app_base_init
{
	void operator()(O *, P *p)
	{
	   Select<O>(*p).ID(1).Execute(Singleton<O>::Instance());
	}
};

template<class O, class P>struct multiply_base_init
{
	void operator()(O *, P *)
	{
	}
};

struct multiply_base_init_data
{
	CBase &base;
	ParametersTable &param;
	multiply_base_init_data(CBase &base) 
		: base(base)
		, param(Singleton<ParametersTable>::Instance())
	{
		CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
		Select<CurrentParametersTable>(base).ID(1).Execute(current);
		
		Select<ParametersTable>(base).ID(current.items.get<CurrentID>().value).Execute(param);
	}
};
template<class X, class P>struct multiply_base_init<ID<X>, P>
{
	typedef ID<X> O;
	void operator()(O *, P *p)
	{
		 Select<X>(p->base).ID(p->param.items.get<O>().value).Execute(Singleton<X>::Instance());
		 X &x = Singleton<X>::Instance();
	}
};
}
//-----------------------------------------------------------------------
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
	  TL::foreach<ParametersTable::items_list, multiply_base_init>()((TL::Factory<ParametersTable::items_list> *)0, &multiply_base_init_data(base));
	  TL::foreach<ParametersBase::one_row_table_list, app_base_init>()((TL::Factory<ParametersBase::type_list> *)0, &base);
  }
}
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\ParametersBase");
	return path;
}
//-------------------------------------------------------------------------	





