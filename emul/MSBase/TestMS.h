#pragma once
#include "TablesDefine.h"
#include "templates.hpp"
#include "tables.hpp"

DEFINE_PARAM(IDTube, int, 1)

     DEFINE_PARAM(TubeNum         , int, 1)
	 DEFINE_PARAM(DateTime, COleDateTime, 0.0)
     STR_PARAM(TypeSize		  , 10, L"")
     STR_PARAM(Result		  , 10, L"")
     STR_PARAM(SolidGroup	  , 10, L"")
     DEFINE_PARAM(ThickBrackBorder, float, 1)
     DEFINE_PARAM(NumOfZones	  , int, 1)
     DEFINE_PARAM(Cut1			  , int, 1)
     DEFINE_PARAM(Cut2			  , int, 1)

struct TubesStat
{
	typedef TL::MkTlst<
		TubeNum
		, DateTime
		, TypeSize
		, Result
		, SolidGroup
		, ThickBrackBorder
		, NumOfZones
		, Cut1
		, Cut2
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"TubesStat";}
};

void TestMS();
