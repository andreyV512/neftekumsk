#pragma once
#include "GridNotify.hpp"
#include "LogMessages.h"

class MainWindowGrid
{
	//LogMess::FactoryMessages &mess;
public :
	struct Handlers
	{
		void operator()(TCellData &);
		void operator()(TCellColor &);
	//	void operator()(TItemChanged &);
	};
	HWND hWnd;
	MainWindowGrid();
	GridEventHandler<Handlers> handlers;
	void Init(HWND );
};