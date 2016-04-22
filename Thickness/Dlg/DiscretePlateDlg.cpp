#include "stdafx.h"
#include "Dlg.h"
#include "..\DlgTemplates\ParamDlg.hpp"

namespace{
	template<class O, class P>struct __compress__
	{
		void operator()(O *o, P *p)
		{
			unsigned offs = p->get<O>().value;
			unsigned bit = 0;
			while(offs >>= 1) ++bit;
			o->value = bit;
		}
	};
	template<class O, class P>struct __set__
	{
		void operator()(O *o, P *p)
		{
			p->get<O>().value = o->value;
		}
	};
	struct putsDlg_OkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			TL::foreach<Owner::list, __uncompress__>()(&owner.items,  (int *)NULL);
			OkBtn().BtnHandler(owner, h);
		}
	};

#define OFFS(n, txt)template<>struct __data_from_widget__<DlgItem<n>, unsigned>\
	{\
		typedef int T;\
		typedef DlgItem<n> O;\
		T operator()(O &o)\
		{ \
			wchar_t buf[64];\
			GetWindowText(o.hWnd, buf, dimention_of(buf));\
			unsigned t = Wchar_to<T>()(buf);\
			return 1 << t;\
		}\
	};\
	template<>struct DlgSubItems<n, unsigned>: UpDownSubItem<n>{}; \
	DO_NOT_CHECK(n)\
	PARAM_TITLE(n, txt)

	OFFS(SQ1BitIn           , L"SQ1 сканирующего устройства(вход трубы)")
	OFFS(SQ3BitIn           , L"SQ3 сканирующего устройства(выход трубы)")
	OFFS(StateBBitIn        , L"В(состояние частотного преобразователя)")
	OFFS(StateABitIn        , L"А(состояние частотного преобразователя)")
	OFFS(RunBitIn           , L"RUN(состояние частотного преобразователя)")
	OFFS(ControlCircuitBitIn, L"Цепи управления")
	OFFS(WorkBitIn          , L"Работа(из шкафа 9955-191/16)")
	OFFS(SycleBitIn         , L"Цикл(из шкафа 9955-191/16)")
	OFFS(BaseBitIn    	   	, L"База(из шкафа 9955-191/16)")

	OFFS(PowerInverterBitOut, L"Питание частотного преобразователя")
	OFFS(SpeedRLBitOut    	, L"Скорость RL")
	OFFS(SpeedRMBitOut    	, L"Скорость RM")
	OFFS(SpeedRHBitOut    	, L"Скорость RH")
	OFFS(STFBitOut        	, L"STF")
	OFFS(PowerScannerBitOut	, L"Питание сканирующего устройства")
	OFFS(ReadyBitOut      	, L"Готовность(в шкаф 9955-191/16)")
	OFFS(ControlBitOut    	, L"Контроль(в шкаф 9955-191/16)")
	OFFS(ResultBitOut     	, L"Результат(в шкаф 9955-191/16)")
#undef OFFS

	PARAM_TITLE(NamePlate1730, L"Дискриптор платы")
	DO_NOT_CHECK(NamePlate1730)
}

void DiscretePlateInputsDlg::Do(HWND h)
{
   InputBitsTable table;
   TL::foreach<InputBitsTable::items_list, __compress__>()(&table.items,  &Singleton<InputBitsTable>::Instance().items);
   TemplDialog<InputBitsTable>dlg(table);
   if(dlg.Do(h, L"Входные порты дискретной платы"))
   {
   TL::foreach<InputBitsTable::items_list, __set__>()(&table.items,  &Singleton<InputBitsTable>::Instance().items);
   }
}

void DiscretePlateOutputsDlg::Do(HWND h)
{
	OutputBitsTable table;
	TL::foreach<OutputBitsTable::items_list, __compress__>()(&table.items,  &Singleton<OutputBitsTable>::Instance().items);
	TemplDialog<OutputBitsTable>dlg(table);
	if(dlg.Do(h, L"Выходные порты дискретной платы"))
	{
		TL::foreach<OutputBitsTable::items_list, __set__>()(&table.items,  &Singleton<OutputBitsTable>::Instance().items);
	}
}

void DiscretePlateDescriptorDlg::Do(HWND h)
{
	if(TemplDialog<NamePlate1730ParametersTable>(Singleton<NamePlate1730ParametersTable>::Instance()).Do(h, L"Дискриптор платы"))
	{
	}
}