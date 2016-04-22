#include "stdafx.h"
#include "DataExchange.h"
#include "SQLbase.h"
#include "ThicknessData.h"
#include "PrimaryData.h"
#include "DebugMess.h"
#include "Config.h"
#include "TablesDefine.h"
#include "Tables.hpp"

#ifndef NO_EXPRESS
DataExchange::DataExchange()
{
	base.Open(L"./connect.udl");
}

unsigned DataExchange::Get_ID_TubeLast()
{
	ADODB::_CommandPtr cmd;
	cmd.CreateInstance(__uuidof(ADODB::Command));
	cmd->ActiveConnection = base.conn;
	cmd->CommandType = ADODB::adCmdText;
	cmd->CommandText = L"SELECT max(ID_Tube)as X FROM InfoTube";
	_variant_t rowsAffected;
	ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
	return rec->Fields->GetItem(L"X")->GetValue();
}

bool DataExchange::GetTimeOffset()
{
	if(base.IsOpen()) 
	{
		dprint("base open-----------------------------------------------");
		ADODB::_CommandPtr cmd;
		_variant_t rowsAffected;
		try
		{
			cmd.CreateInstance(__uuidof(ADODB::Command));
			cmd->ActiveConnection = base.conn;
			cmd->CommandType = ADODB::adCmdText;
			cmd->CommandText = L"SELECT * FROM Strobes";
			ADODB::_RecordsetPtr rec = cmd->Execute( &rowsAffected, 0, ADODB::adCmdText);
			int count = rec->Fields->GetItem(L"Count")->GetValue();
			double (&offs)[ThicknessData::MAX_ZONES_COUNT] = primaryData.offsetOfTime;
			if(count > dimention_of(offs)) count = dimention_of(offs);
			wchar_t num[16];
			dprint("count times zones %d  -------------------------------------", count);
			for(int i = 0; i < count; ++i)
			{
				wsprintf(num, L"_%02d", i);
				offs[i] = rec->Fields->GetItem(num)->GetValue();
				dprint("index %d   time %f", i, offs[i]);
			}
			thicknessData.countZones = count;
			primaryData.countZones = count;
			DeleteTimeOffset(); 



			return true;
		}
		catch(...)
		{
			dprint("base catch+++++++++++++++++++");
			return false;
		}
		dprint("base Ok +++++++++++++++++++++");
		return true;
	}
	dprint("base errror    -------------");
	return false;
}

void DataExchange::DeleteTimeOffset()
{
	if(base.IsOpen()) 
	{
		base.ConnectionSQL(L"DELETE FROM Strobes");
	}
}

namespace {
CHAR_PARAM(Result, 2, "X")
DEFINE_PARAM(Date, COleDateTime, 0.0);
//DEFINE_PARAM(MinThickness, float, 5.33f)
struct InfoTube
 {
	 typedef TL::MkTlst<
		 Result
		 , Date
		 , MinThickness
	 >::Result items_list;
	 typedef NullType unique_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"InfoTube";}
 };
void SetTubeNum(CExpressBase &b)
{
   InfoTube info;
   COleDateTime tme = COleDateTime::GetCurrentTime();
   info.items.get<Date>().value = tme;
   info.items.get<MinThickness>().value = 4.3f;
   Insert_Into<InfoTube>(info, b).Execute();
}
}

void DataExchange::SendData()
{
	if(base.IsOpen())
	{
		SetTubeNum(base);
		ThickDef thickDef;
		unsigned id = Get_ID_TubeLast();
		thickDef.items.get<ID_Tube>().value = id;

		dprint("last InfoTube ID %d", id);

		for(int i = 0, len = thicknessData.countZones; i < len; ++i)
		{
			thickDef.items.get<Zone>().value = i;
			thickDef.items.get<MinThickness>().value = (float)thicknessData.zonesMin[i];
			Insert_Into<ThickDef>(thickDef, base).Execute();
		}
	}
}
#else
DataExchange::DataExchange(){}

unsigned DataExchange::Get_ID_TubeLast(){return 0;}

bool DataExchange::GetTimeOffset(){return true;}

void DataExchange::DeleteTimeOffset(){}

void DataExchange::SendData(){}
#endif
