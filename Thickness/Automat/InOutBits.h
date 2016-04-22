#pragma once
#include "typelist.hpp"

#define IN_BITS(z, def_val) struct z##BitIn\
{\
	typedef unsigned type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z##L"BitIn";}\
	z##BitIn() : value(def_val), default_value(def_val) {}\
};

IN_BITS(SQ1           , 1 << 0)
IN_BITS(SQ3           , 1 << 1)
IN_BITS(StateB        , 1 << 3)
IN_BITS(StateA        , 1 << 2)
IN_BITS(Run           , 1 << 4)
IN_BITS(ControlCircuit, 1 << 5)
IN_BITS(Work          , 1 << 6)
IN_BITS(Sycle         , 1 << 7)
IN_BITS(Base          , 1 << 8)


typedef TL::MkTlst<
    SQ1BitIn           
    , SQ3BitIn           
    , StateABitIn 
	, StateBBitIn
    , RunBitIn           
    , ControlCircuitBitIn
    , WorkBitIn          
    , SycleBitIn         
    , BaseBitIn          
>::Result input_bits_list;

#define OUT_BITS(z, def_val) struct z##BitOut\
{\
	typedef unsigned type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z##L"BitOut";}\
	z##BitOut() : value(def_val), default_value(def_val) {}\
};

OUT_BITS(PowerInverter, 1 << 0)
OUT_BITS(SpeedRL      , 1 << 1)
OUT_BITS(SpeedRM      , 1 << 2)
OUT_BITS(SpeedRH      , 1 << 3)
OUT_BITS(STF          , 1 << 4)
OUT_BITS(PowerScanner , 1 << 5)
OUT_BITS(Ready        , 1 << 6)
OUT_BITS(Control      , 1 << 7)
OUT_BITS(Result       , 1 << 8)


typedef TL::MkTlst<
   PowerInverterBitOut
   , SpeedRLBitOut      
   , SpeedRMBitOut      
   , SpeedRHBitOut      
   , STFBitOut          
   , PowerScannerBitOut
   , ReadyBitOut        
   , ControlBitOut      
   , ResultBitOut        
>::Result output_bits_list;


