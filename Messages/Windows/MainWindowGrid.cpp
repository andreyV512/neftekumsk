#include "stdafx.h"
#include "MainWindowGrid.h"
#include "DebugMess.h"
#include "LogBuffer.h"

//------------------------------------------------------------------------
extern HINSTANCE hInstance;
//------------------------------------------------------------------------
template<class T>struct header_table;
#define Z(T, w, txt)\
struct T{};\
template<>struct header_table<T>\
{\
	LPWSTR name(){return L##txt;}\
	static const int width = w;\
};

Z(Group     , 70, "Время")
Z(Color, 500, "Сообщение")
#undef Z

typedef TL::MkTlst< Group, Color>::Result ParameterNameList;
//--------------------------------------------------------------------------------
MainWindowGrid::MainWindowGrid()
{
}
//--------------------------------------------------------------------------------
void MainWindowGrid::Init(HWND h)
{
	hWnd = handlers.Init<TL::MkTlst<Group, Color>::Result>(h, 1024);
}
//--------------------------------------------------------------------
#pragma warning(disable : 4995)
void MainWindowGrid::Handlers::operator()(TCellData &l)
{
#pragma warning(disable : 4996)
	Log::TData *d = NULL;
	if(Log::IsRow(l.row, d))
	{
		switch(l.col)
		{
		case 0: 
			{
				Log::TData *d0 = NULL;
				if(Log::IsRow(l.row + 1, d0))
				{
					_itow(d->time - d0->time, l.data, 10);
				}
				else
				{
					l.data[0] = '0';
					l.data[1] = '\0';
				}
			//	dprint("row %d", l.row);
			}
			break;
		case 1:
			char buf[1024];
			LogMess::FactoryMessages::Instance().Text(d->id, buf, d->value);
			
			int len = 1 + strlen(buf);
			MultiByteToWideChar(1251, 0, buf, len, l.data, len);			
			break;
		}
	}
}
//--------------------------------------------------------------------
void MainWindowGrid::Handlers::operator()(TCellColor &l)
{	
	//if(l.row % 2 == 0) l.bkColor = 0xff; else l.bkColor = 0xff00;
	Log::TData *d = NULL;
	if(Log::IsRow(l.row, d))
	{
		LogMess::FactoryMessages::Instance().Color(d->id, (unsigned &)l.bkColor, (unsigned &)l.textColor);
	}
}
//---------------------------------------------------------------------------
/*
void MainWindowGrid::Handlers::operator()(TItemChanged &l)
{
	if( l.newState != l.oldState)dprint("TItemChanged %d %x %x", l.row, l.newState, l.oldState);
}
*/
