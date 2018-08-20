#pragma once

#define __double_1 " %.1f" 
#define __double_2 " %.2f"
#define __int " %d"
#define __void ""
#define __bool " %s"
#define __const_char_ptr " %s"
#define _cat(a, b) a##b

#define MESS(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};
#define MESS1(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};
#define MESS2(name, tpe, txt, bc, tc) template<>struct Row<name>\
{\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_2);}\
};

namespace LogMess
{
	enum ID
	{
		StartSycle
		, StopSycle
		, OverheadTemperature
		, ProgramOpen

		, TimeoutPipe
		, AutomatMode
		, SingleMode

		, ProgramClosed
		, ErrStop
		, AlarmExitTubeInUnit
		, AlarmExitDirectionChainOn
		, AlarmExitInverterFaulty

		, AlarmExitRunBitIn			 
		, AlarmExitControlCircuitBitIn 
		, AlarmExitWorkBitIn			 
		, AlarmExitSycleBitIn		  

		, InfoOnWorkBitIn 
		, InfoOffWorkBitIn
		, InfoDataReceive
		, InfoOnResultBitOut
		, InfoReadyBitOut	            
		, InfoOnSycleBitIn  
		, InfoRotationalSpeedSensorsStart
		, InfoOnRunBitIn                 
		, InfoControlBitOut              
		, InfoOnSQ1BitIn	                
		, InfoPowerScannerBitOut         
		, InfoOnSQ3BitIn	                
		, InfoOffSQ1BitIn                
		, InfoOffPowerScannerBitOut         
		, InfoOffSQ3BitIn 
		, InfoUserStop
		, InfoDataCollectionComplete
		//, Info

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
		, WaitStrobes

		//, InfoRotationScanningModule

		, max_row
	};

	//typedef const char * const_char_ptr;

	static const int green = 0xff00;
	static const int blue  = 0xff0000;
	static const int black = 0x0;
	static const int white = 0xffffff;
	static const int red = 0xff;
	static const int yellow = 0xffff;

	template<int >struct Row;
	///\brief ������������� ����� ����������
	///\param ����� �� ������������ ID(��. ����)
	///\param ��� ��������������� ���������
	///\param ���������
	///\param ���� ����
	///\param ���� ������
	MESS(StartSycle			 , void  , "���� �����"                     , green, black)
    MESS(StopSycle			 , void  , "���� ����"				        , blue , white)
	MESS1(OverheadTemperature, double, "����������� ���������"	        , red  , yellow)
	MESS(ProgramOpen		 , void  , "��������� �������"              , black, white)

	MESS(TimeoutPipe		 , void  , "���������� ����� ��������", red  , yellow)

	MESS(AutomatMode		 , void  , "����� \"��������������\""		, blue , white)
	MESS(SingleMode			 , void  , "����� \"��������� ���������\""	, blue , white)

	MESS(ProgramClosed		 , void  , "��������� �������"	, red  , yellow)

	MESS(ErrStop             , void  , "���������� ����� ���������", red, yellow)

	MESS(AlarmExitTubeInUnit          , void, "����� � ������, ��������� �����", red  , yellow)
	MESS(AlarmExitDirectionChainOn    , void, "�������� ���� ����������", red  , yellow)
	MESS(AlarmExitInverterFaulty      , void, "��������� ��������������� �� �������", red  , yellow)

	MESS(AlarmExitRunBitIn			  , void, "������ ���������� ���������������\"", red  , yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "��� ������� \"���� ����������\""	   , red  , yellow)
	MESS(AlarmExitWorkBitIn			  , void, "��� ������� \"������\""			   , red  , yellow)
	MESS(AlarmExitSycleBitIn		  , void, "��� ������� \"����\""			   , red  , yellow)


	MESS(InfoOnWorkBitIn                 , void, "�������� ������� \"������\""					  , blue , white)
	MESS(InfoOffWorkBitIn				 , void, "�������� ������ ������� \"������\""			  , green, blue)
	MESS(InfoDataReceive				 , void, "������ ������������� �������"                   , blue, yellow)
	MESS(InfoReadyBitOut	             , void, "��������� ������ \"����������\""				  , blue , white)
	MESS(InfoOnResultBitOut              , void, "��������� ������ \"���������\""                 , blue , white)
	MESS(InfoOnSycleBitIn                , void, "�������� ������� \"����\""					  , blue , white)
	MESS(InfoRotationalSpeedSensorsStart , void, "��������� �������� ������ �������� ������������", blue , white)
	MESS(InfoOnRunBitIn                  , void, "�������� ��������� ������ �������� ������������", blue , white)
	MESS(InfoControlBitOut               , void, "��������� ������ \"��������\""				  , blue , white)
	MESS(InfoOnSQ1BitIn	                 , void, "�������� ����� �� ������� SQ1"				  , blue , white)
	MESS(InfoPowerScannerBitOut          , void, "��������� ������������ ����������"			  , blue , white)
	MESS(InfoOnSQ3BitIn	                 , void, "�������� ����� �� ������� SQ3"				  , blue , white)
	MESS(InfoOffSQ1BitIn                 , void, "�������� ������ ����� � ������� SQ1"			  , blue , white)
	MESS(InfoOffPowerScannerBitOut       , void, "���������� ������������ ����������"		      , blue , white)
	MESS(InfoOffSQ3BitIn                 , void, "�������� ������ ����� � ������� SQ3"			  , blue , white)
	MESS(InfoUserStop					 , void, "�������� ����� �� ����� ����� ������"			  , red  , yellow)

	MESS(InfoDataCollectionComplete		 , void, "���� ������ ��������"                           , green, white);

	MESS(SQ1BitIn             , bool, "SQ1BitIn            ", white, black)
	MESS(SQ3BitIn             , bool, "SQ3BitIn            ", white, black)
	MESS(StateBBitIn          , bool, "StateBBitIn         ", white, black)
	MESS(StateABitIn          , bool, "StateABitIn         ", white, black)
	MESS(RunBitIn             , bool, "RunBitIn            ", white, black)
	MESS(ControlCircuitBitIn  , bool, "ControlCircuitBitIn ", white, black)
	MESS(WorkBitIn            , bool, "WorkBitIn           ", white, black)
	MESS(SycleBitIn           , bool, "SycleBitIn          ", white, black)
	MESS(BaseBitIn 			  , bool, "BaseBitIn 		   ", white, black)
	MESS(PowerInverterBitOut  , bool, "PowerInverterBitOut ", white, black)
	MESS(SpeedRLBitOut        , bool, "SpeedRLBitOut       ", white, black)
	MESS(SpeedRMBitOut        , bool, "SpeedRMBitOut       ", white, black)
	MESS(SpeedRHBitOut        , bool, "SpeedRHBitOut       ", white, black)
	MESS(STFBitOut            , bool, "STFBitOut           ", white, black)
	MESS(PowerScannerBitOut	  , bool, "PowerScannerBitOut  ", white, black)
	MESS(ReadyBitOut          , bool, "ReadyBitOut         ", white, black)
	MESS(ControlBitOut        , bool, "ControlBitOut       ", white, black)
	MESS(ResultBitOut   	  , bool, "ResultBitOut   	 "  , white, black)

	MESS(WaitStrobes, void, "�������� ������� � ������������", blue, yellow)

	//MESS(InfoRotationScanningModule, void, "�������� �������� �����������", red  , yellow)
	//MESS(AlarmRotationScanningModule, void, "�������� �������� �����������", red  , yellow)

	class FactoryMessages
	{
		struct Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned &backColor, unsigned &textColor);
		bool Text(int i, char *buf, double val);
		static FactoryMessages &Instance();
	};
}
#undef MESS
#undef MESS1
#undef MESS2
#undef __double_1 
#undef __double_2 
#undef __int 
#undef __void 
#undef _cat


