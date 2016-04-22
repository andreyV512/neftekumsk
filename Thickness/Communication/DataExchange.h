#pragma once
#include "ExpressBase.hpp"
class DataExchange
{
	CExpressBase base;
public:
	DataExchange();
	unsigned Get_ID_TubeLast();
	bool GetTimeOffset();
	void DeleteTimeOffset();
	void SendData();	
};