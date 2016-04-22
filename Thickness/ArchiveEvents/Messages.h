#pragma once
#include <Windows.h>
#include "typelist.hpp"
#include "InOutBits.h"
#pragma warning(disable : 4995)

template<class T>struct MessageField
{
	//typedef typename T::__template_must_be_overridded__ noused; 
	static wchar_t *Text(){return L"MessageField";}
};

#define ms(n, text) \
struct n;\
template<>struct MessageField<n>\
{\
	static wchar_t *Text(){return text;}\
};

struct NotMessage{};

ms(NotMessage    , L"")
ms(SQ1BitIn    , L"Труба в модуле измерения(SQ1)")
#undef ms

template<class T>struct DataField
{
	//typedef typename T::__template_must_be_overridded__ noused; 
	static wchar_t *Text(double d)
	{
	return L"DataField";
	}
};

#define ms_data(n) template<>struct DataField<n>\
{\
	static wchar_t *Text(double d)\
    {\
	   return 0.0 != d ? L"ВКЛ" : L"ОТКЛ";\
    }\
};

template<>struct DataField<NotMessage>
{
	static wchar_t *Text(double d)
    {
	   return L"";
    }
};
ms_data(SQ1BitIn)
#undef ms_data

typedef TL::MkTlst<
 NotMessage
, SQ1BitIn           
, SQ3BitIn           
, StateBBitIn        
, StateABitIn        
, RunBitIn           
, ControlCircuitBitIn
, WorkBitIn          
, SycleBitIn         
, BaseBitIn 
, PowerInverterBitOut
, SpeedRLBitOut      
, SpeedRMBitOut      
, SpeedRHBitOut      
, STFBitOut          
, PowerScannerBitOut
, ReadyBitOut        
, ControlBitOut      
, ResultBitOut   
>::Result message_list; 

void EventLogInputPort();
void EventLogOutputPort();










