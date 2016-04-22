#pragma once 

#include "typelist.hpp"
#include "TypeSize/TypeSizeProtocol.h"
#include "Thickness/ThicknessProtocol.h"
#include "Strobes/StrobesProtocol.h"
#include "StopCycle/StopCycleProtocol.h"

typedef TL::MkTlst<
		 ThicknessProtocol::Server
		 , TypeSizeProtocol::Server
		 , StrobesProtocol::Server
		 , StopCycleProtocol::Server
	 >::Result __ip_protocol_procedure_list__;