#pragma once
#include "tables.hpp"
#include "TablesDefine.h"
#include "LanParameters.h"
#include "InOutBits.h"
#include "AppConst.h"
//-------------------------------------------------------------------------------------------
extern const double &result_undefined;
//-----------------------------------------------------------------------------------
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
STR_PARAM(NameParam, 128, L"NONAME")

DEFINE_PARAM(Border2Class, double, 5.8)
DEFINE_PARAM(BorderDefect, double, 4.5)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		Border2Class
		, BorderDefect
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThresholdsTable";}
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
DEFINE_PARAM(LessMinimumEnergy, int, 0xff555555)
	DEFINE_PARAM(GreaterMaximumEnergy, int, 0xff555555)
	DEFINE_PARAM(ValueLessThreshold, int, 0xff555555)
	DEFINE_PARAM(ThicknessLessThreshold, int, 0xff555555)
	DEFINE_PARAM(Undefined, int, 0xff555555)

	DEFINE_PARAM(Nominal, int, 0xff00ff00)
	DEFINE_PARAM(Treshold2Class, int, 0xffffff00)
	DEFINE_PARAM(Defect, int, 0xffff0000)
 

	DEFINE_PARAM(DeathZone, int, 0xff333333)   

struct ColorTable
{
	typedef TL::MkTlst<	
		LessMinimumEnergy
		, GreaterMaximumEnergy
		, ValueLessThreshold
		, ThicknessLessThreshold
		, Nominal
		, Defect
		, Treshold2Class
		, Undefined
		, DeathZone
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ColorTable";}
};
//----------------------------------------------------------------------------------- 
DEFINE_PARAM(MinimumThicknessPipeWall, double, 2.5)
DEFINE_PARAM(MaximumThicknessPipeWall, double, 8.0)
struct BorderCredibilityTable
{
	typedef TL::MkTlst<
		MinimumThicknessPipeWall
		, MaximumThicknessPipeWall
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"BorderCredibilityTable";}
};
//-----------------------------------------------------------------------------------------
DEFINE_PARAM(MinAxes, double, 2.5)
DEFINE_PARAM(MaxAxes, double, 8.0)
struct AxesTable
{
	typedef TL::MkTlst<
		MinAxes
		, MaxAxes
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AxesTable";}
};
//-------------------------------------------------------------------------------------- 
/*
DEFINE_PARAM(SupplySensorDelay, int, 150)
DEFINE_PARAM(RemoveSensorDelay, int,  150)
DEFINE_PARAM(FrequencyFrames, int,  100)

DEFINE_PARAM(ReferenceOffset1, unsigned, 1750)
DEFINE_PARAM(ReferenceOffset2, unsigned, 4600)

DEFINE_PARAM(ReferenceOffset3, unsigned, 1260)
DEFINE_PARAM(ReferenceOffset4, unsigned, 3030)
struct AdditionalSettingsTable
{
	typedef TL::MkTlst<	
		ReferenceOffset1
		, ReferenceOffset2
		, ReferenceOffset3
		, ReferenceOffset4
		, SupplySensorDelay
		, RemoveSensorDelay
		, FrequencyFrames
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AdditionalSettingsTable";}
};
*/
//--------------------------------------------------------------------------------------
template<int>struct ACFBorderLeft ;
template<int>struct ACFBorderRight;
DEFINE_PARAM_NUM(ACFBorderLeft , 0, int, 45)
DEFINE_PARAM_NUM(ACFBorderRight, 0, int, 80)
DEFINE_PARAM_NUM(ACFBorderLeft , 1, int, 45)
DEFINE_PARAM_NUM(ACFBorderRight, 1, int, 80)
DEFINE_PARAM_NUM(ACFBorderLeft , 2, int, 45)
DEFINE_PARAM_NUM(ACFBorderRight, 2, int, 80)
struct ACFBorderTable
{
	typedef TL::MkTlst<
		  ACFBorderLeft<0>
		, ACFBorderRight<0>
		, ACFBorderLeft<1>
		, ACFBorderRight<1>
		, ACFBorderLeft<2>
		, ACFBorderRight<2>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ACFBorderTable";}
};
//-----------------------------------------------------------------------------------------------------
struct InputBitsTable
{
	typedef input_bits_list items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBitsTable";}
};
struct OutputBitsTable
{
	typedef output_bits_list items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBitsTable";}
};
//-----------------------------------------------------------------------------------------------------
 DEFINE_PARAM(DeadAreaMM0, int, 200)
 DEFINE_PARAM(DeadAreaMM1, int, 200)
 struct DeadAreaTable
 {
	typedef TL::MkTlst<
		DeadAreaMM0
		, DeadAreaMM1
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeadAreaTable";}
 };
//----------------------------------------------------------------------------------------------------
 DEFINE_PARAM(MedianFilter, int, 5)
 struct MedianFilterTable
 {
	 typedef TL::MkTlst<
		 MedianFilter
	 >::Result items_list;
	 typedef NullType unique_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"MedianFilterTable";}
 };
//------------------------------------------------------------------------------------------------------ 
 template<int>struct MinEnergy;
 template<int>struct MaxEnergy;
 template<int>struct Peak;

#define PEAK(n)\
	DEFINE_PARAM_NUM(MinEnergy, n, double, 0.1)\
	DEFINE_PARAM_NUM(MaxEnergy, n, double, 1000)\
	DEFINE_PARAM_NUM(Peak, n, double, 0.3)

     PEAK(0)
	 PEAK(1)
	 PEAK(2)
	 PEAK(3)
	 PEAK(4)
	 PEAK(5)

#undef PEAK

#define PEAK(n) MinEnergy<n>, MaxEnergy<n>, Peak<n>
 struct SignalParametersTable
 {
	 typedef TL::MkTlst<		
		 PEAK(0), PEAK(1), PEAK(2)//, PEAK(3), PEAK(4), PEAK(5)
	 >::Result items_list;
	 typedef NullType unique_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"SignalParametersTable";}
 };
#undef PEAK
//-------------------------------------------------------------------------------------------------------
 template<int NUM>struct CoefficientA;
 template<int NUM>struct CoefficientB;
 DEFINE_PARAM_NUM(CoefficientA, 1, double, 0.066)
 DEFINE_PARAM_NUM(CoefficientB, 1, double, -0.069)
 DEFINE_PARAM_NUM(CoefficientA, 2, double, 0.066)
 DEFINE_PARAM_NUM(CoefficientB, 2, double, -0.069)
 DEFINE_PARAM_NUM(CoefficientA, 3, double, 0.066)
 DEFINE_PARAM_NUM(CoefficientB, 3, double, -0.069)
 //DEFINE_PARAM_NUM(CoefficientA, 4, double, 0.066)
 //DEFINE_PARAM_NUM(CoefficientB, 4, double, -0.069)
 //DEFINE_PARAM_NUM(CoefficientA, 5, double, 0.066)
 //DEFINE_PARAM_NUM(CoefficientB, 5, double,- 0.069)
 //DEFINE_PARAM_NUM(CoefficientA, 6, double, 0.066)
 //DEFINE_PARAM_NUM(CoefficientB, 6, double, -0.069)

 struct CoefficientParametersTable
 {
	 typedef TL::MkTlst<		
		 CoefficientA<1>
		 , CoefficientB<1>
		 , CoefficientA<2>
		 , CoefficientB<2>
		 , CoefficientA<3>
		 , CoefficientB<3>
		 //, CoefficientA<4>
		 //, CoefficientB<4>
		 //, CoefficientA<5>
		 //, CoefficientB<5>
		 //, CoefficientA<6>
		 //, CoefficientB<6>
	 >::Result items_list;
	 typedef NullType unique_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"CoefficientParametersTable";}
 };
 //---------------------------------------------------------------------------------------------------------
 DEFINE_PARAM(NamePlate1730, int, 0)
 struct NamePlate1730ParametersTable
 {
	 typedef TL::MkTlst<
		 NamePlate1730
	 >::Result items_list;
	 typedef NullType unique_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"NamePlate1730ParametersTable";}
 };
//-------------------------------------------------------------------------------------------------------
// rotational speed sensors
template<class T>struct TestBit;
DEFINE_PARAM_WAPPER(TestBit, SpeedRLBitOut, bool, false)
DEFINE_PARAM_WAPPER(TestBit, SpeedRMBitOut, bool, true)
DEFINE_PARAM_WAPPER(TestBit, SpeedRHBitOut, bool, false)
struct RotationalSpeedSensorsTable
{
	typedef TL::MkTlst<
		 TestBit<SpeedRLBitOut>
		, TestBit<SpeedRMBitOut>
		, TestBit<SpeedRHBitOut>
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"RotationalSpeedSensorsTable";}
};
//--------------------------------------------------------------------------------------------------------
struct AxesTable;
DEFINE_PARAM_ID(ThresholdsTable            , int, 1)
DEFINE_PARAM_ID(DeadAreaTable			   , int, 1)
DEFINE_PARAM_ID(BorderCredibilityTable	   , int, 1)
DEFINE_PARAM_ID(RotationalSpeedSensorsTable, int, 1)
DEFINE_PARAM_ID(AxesTable, int, 1)
DEFINE_PARAM_ID(ACFBorderTable          , int, 1)
DEFINE_PARAM_ID(ColorTable, int, 1)
 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable			   	>
		, ID<BorderCredibilityTable	   	>
		, ID<RotationalSpeedSensorsTable>
		, ID<AxesTable>
		, ID<ACFBorderTable>
		, ID<ColorTable>
		, NameParam
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };
//----------------------------------------------------------------------------
 DEFINE_PARAM(IPPort, int, 8888)
//#define ip_address() ((192 << 24) | (168 << 16) | (1 << 8) | (8 << 0))
//DEFINE_PARAM(IPAddr, int, ip_address())
//#undef ip_address
STR_PARAM(IPName, 64, L"A3")
struct IPAddressTable
{
	typedef TL::MkTlst<	
		IPPort
		, IPName
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"IPAddressTable";}
};
//--------------------------------------------------------------------------------------------------------
 struct ParametersBase
 {
	 typedef TL::MkTlst<
		 SignalParametersTable		   
		 , CoefficientParametersTable	 
		 , NamePlate1730ParametersTable	  		
		 , PointsOptionsTable			
		 , GraphicSignalOptionsTable	   
		 , LanParametersTable		   
		// , AdditionalSettingsTable
		 , InputBitsTable
		 , OutputBitsTable 
		 
		 , MedianFilterTable
		
		 , IPAddressTable
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		 CurrentParametersTable		 
		 , ParametersTable			   
		 , ThresholdsTable			  
		 , DeadAreaTable				  
		 , BorderCredibilityTable	 
		 , RotationalSpeedSensorsTable
		// , DiameterTubeTable
		, AxesTable
		, ACFBorderTable
		, ColorTable
	 >::Result password_typesize_multy_row_table_list;

	 typedef TL::MkTlst<
		  password_typesize_multy_row_table_list
		 , one_row_table_list
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
	static void InitTypeSizeTables(CBase &);
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





