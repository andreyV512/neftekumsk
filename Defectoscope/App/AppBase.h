#pragma once
#include "tables.hpp"
#include "TablesDefine.h"
//-------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
DEFINE_PARAM(OffsetWindowX, int, 0)
DEFINE_PARAM(OffsetWindowY, int, 0)
DEFINE_PARAM(WindowWidth, int, 750)
DEFINE_PARAM(WindowHeight, int, 450)
struct ThicknessWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThicknessWindowTable";}
};
struct ZonesWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ZonesWindowTable";}
};

struct TresholdPanelViewerTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TresholdPanelViewer";}
};

	struct TresholdWindowTable
{
	typedef TL::MkTlst<	
		OffsetWindowX
		, OffsetWindowY
		, WindowWidth
		, WindowHeight
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TresholdWindowTable";}
};
//--------------------------------------------------------------------------------------


DEFINE_PARAM(PortTCP, int, 2030)
STR_PARAM(AddresTCP, 17, L"192.168.1.100")
struct TcpCommunications
{
	typedef TL::MkTlst<	
		PortTCP
		, AddresTCP
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TcpCommunications";}
};
//*/
//-----------------------------------------------------------------------------------
DEFINE_PARAM(SignalPoint, double, 10)
DEFINE_PARAM(ReferencePoint, double, 10)

struct L502ParametersTable
{
	typedef TL::MkTlst<	
		SignalPoint
		, ReferencePoint
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502ParametersTable";}
};
//-----------------------------------------------------------------------------------
DEFINE_PARAM(CurrentID, int, 1)

struct CurrentParametersTable
{
	typedef TL::MkTlst<
		CurrentID
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentParametersTable";}
};

struct NameParam
{
	typedef Holder<128> type_value;
	type_value value;
	type_value default_value;
	const wchar_t *name(){return L"NameParam";}
	NameParam()
	{
		value = L"NONAME";
		default_value = L"NONAME";
	}
};
template<int >struct XParam;
template<int >struct EParam;

DEFINE_PARAM_NUM(EParam, 1, int, 36)
DEFINE_PARAM_NUM(EParam, 2, int, 40)
DEFINE_PARAM_NUM(EParam, 3, int, 45)
DEFINE_PARAM_NUM(EParam, 4, int, 50)
DEFINE_PARAM_NUM(EParam, 5, int, 59)
DEFINE_PARAM_NUM(EParam, 6, int, 62)
DEFINE_PARAM_NUM(EParam, 7, int, 65)
DEFINE_PARAM_NUM(EParam, 8, int, 70)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		  EParam<1>
		, EParam<2>
		, EParam<3>
		, EParam<4>
		, EParam<5>
		, EParam<6>
		, EParam<7>
		, EParam<8>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThresholdsTable";}
};
DEFINE_PARAM_ID(ThresholdsTable, int, 1)
struct ParametersTable
{
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, NameParam
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
};
//----------------------------------------------------------------------------------------
DEFINE_PARAM(CommunicationRemoveUnit, int, 0)
DEFINE_PARAM(CounterTubesStored, int, 0)

struct DifferentOptionsTable
{
	typedef TL::MkTlst<
		CommunicationRemoveUnit
		, CounterTubesStored
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DifferentOptionsTable";}
};
DEFINE_PARAM(OffsetAxesX, int, 100)
DEFINE_PARAM(FrameWidth, int, 400)
DEFINE_PARAM(MaxAxesY, double, 8191)
DEFINE_PARAM(MinAxesY, double, -8192)
struct GraphicSignalOptionsTable
{
	typedef TL::MkTlst<
		OffsetAxesX
		, FrameWidth
		, MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"GraphicSignalOptionsTable";}
};
struct PointsOptionsTable
{
	typedef TL::MkTlst<
		MaxAxesY
		, MinAxesY
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"PointsOptionsTable";}
};
//----------------------------------------------------------------------------------
DEFINE_PARAM(Frequency502, int, 4000)
DEFINE_PARAM(FrequencyGenerator, int, 6)
DEFINE_PARAM(InputRangeSignal, int, 0)
DEFINE_PARAM(RangeReferenceSignal, int, 0)
struct SolenoidParametersTable
{
	typedef TL::MkTlst<
		Frequency502
		, FrequencyGenerator
		, InputRangeSignal
		, RangeReferenceSignal
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"SolenoidParametersTable";}
};
//----------------------------------------------------------------------------------
struct ParametersBase
{
	typedef TL::MkTlst<
		TcpCommunications	
		//	, ThicknessWindowTable
		//	, ZonesWindowTable
		//	, TresholdWindowTable
		//	, DifferentOptionsTable
		, SolenoidParametersTable
		, PointsOptionsTable
		, GraphicSignalOptionsTable
	>::Result one_row_table_list;
	typedef TL::MkTlst<
		CurrentParametersTable
		, ThresholdsTable
		, ParametersTable
	>::Result multy_row_table_list;	
	typedef TL::MkTlst<
		one_row_table_list
		, multy_row_table_list
	>::Result multy_type_list; 

	typedef TL::MultyListToList<multy_type_list>::Result type_list;
	typedef TL::Factory<type_list> TTables;
	TTables tables;
	wchar_t path[512];
	const wchar_t *name();
};

struct AppBase
{
	void Init();
};

template<class T>int CurrentId(CBase &base)
{
	  CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
	  Select<CurrentParametersTable>(base).ID(1).Execute(current);
	  ParametersTable param;
	  Select<ParametersTable>(base).ID(current.items.get<CurrentID>().value).Execute(param);
	  return param.items.get<T>().value;
}

template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}

template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   Update<ParametersTable>(base).set<T>(num).Where().ID(current.items.get<CurrentID>().value).Execute();
}

template<class T>struct Singleton
{
	static T& Instance(){static T x; return x;}
};





