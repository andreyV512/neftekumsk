#include "stdafx.h"
#include "Messages.h"
#include "InOutBits.h"
#include "ArchiveEvents.h"
#include "typelist.hpp"
#include "AppBase.h"

namespace EventLogSpace
{
	unsigned &input = archiveEvents.map->input;
	unsigned &output = archiveEvents.map->output;
	unsigned last_input;
	unsigned last_output;
	unsigned tmp;
	template<class O, class P>struct __set_to_event_log_input__
	{
		const unsigned &bit;
		__set_to_event_log_input__(): bit(Singleton<InputBitsTable>::Instance().items.get<O>().value){}
		bool operator()(O *, P *)
		{			
			if(tmp & bit)
			{
				tmp &= ~bit;
				archiveEvents.Set<O>(input & bit);
			}
			return 0 != tmp;
		}
	};
	template<class O, class P>struct __set_to_event_log_output__
	{
		const unsigned &bit;
		__set_to_event_log_output__(): bit(Singleton<OutputBitsTable>::Instance().items.get<O>().value){}
		bool operator()(O *, P *)
		{			
			if(tmp & bit)
			{
				tmp &= ~bit;
				archiveEvents.Set<O>(output & bit);
			}
			return 0 != tmp;
		}
	};
}

void EventLogInputPort()
{
	using namespace EventLogSpace;
	tmp = input ^ last_input;
	TL::find<input_bits_list, __set_to_event_log_input__>()((TL::Factory<input_bits_list> *)0, (int *)0);
	last_input = input;
}
//-----------------------------------------------------------------------------------------
void EventLogOutputPort()
{
	using namespace EventLogSpace;
	tmp = output ^ last_output;
	TL::find<output_bits_list, __set_to_event_log_output__>()((TL::Factory<output_bits_list> *)0, (int *)0);
	last_output = output;
}
//-----------------------------------------------------------------------------------------
