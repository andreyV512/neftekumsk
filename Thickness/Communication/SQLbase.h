#pragma once
#include "TablesDefine.h"
#include "Base.hpp"
#include "tables.hpp"
//---------------------------------------------------------
CHAR_PARAM  (Result      , 2, "")
DEFINE_PARAM(TypeSize    , int, 1)
DEFINE_PARAM(Date    , COleDateTime, 0.0)
DEFINE_PARAM(MinThickness, float, 4.3f)
DEFINE_PARAM(Length      , float, 0)

struct InfoTube
{
	typedef TL::MkTlst<
		Result      
		, TypeSize    
		, Date    
		, MinThickness
		, Length      
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InfoTube";}
};

DEFINE_PARAM(Zone, int, 0)
DEFINE_PARAM(ID_Tube, int, 0)

struct ThickDef
{
	typedef TL::MkTlst<
		Zone      
		, MinThickness    
		, ID_Tube    
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThickDef";}
};
//--------------------------------------------------------------------------------------------------------
struct CommunicationBase
{
	typedef TL::MkTlst<
		InfoTube	
		, ThickDef
	>::Result type_list;

	typedef TL::Factory<type_list> TTables;
	TTables tables;
};