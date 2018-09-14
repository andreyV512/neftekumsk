#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "Config.h"
#include "DebugMess.h"
#include "../Resource.h"
#include "InitToolBar.hpp"
#include "message.h"
#include "DetailedWindow.h"
#include "ThicknessViewer.h"
#include "Emptywindow.h"
#include "Automat.h"
#include "DataExchange.h"
#include "CommunicationWapper\CommunicationWapper.h"
#include "App/App.h"
#include "MainWindow.h"

void SycleMeashurement(HWND);//обработчик для кнопки "Циклическое измерение"

void StopMeashurement(HWND);//обработчик для кнопки "Остановка измерения"

void TresholdsViewBtn(HWND);

void TestBtn(HWND);

void TestBase(HWND);	//для отладки

void ExitTubeButton(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};

TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Цикл")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Стоп")
TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"Просмотр")
TOOL_LIP_TEXT(ToolTestBtn  , L"Тест")
TOOL_LIP_TEXT(ToolTestBase  , L"Тест Base")

TOOL_LIP_TEXT(ToolExitTube  , L"Вращение")
  
#undef TOOL_LIP_TEXT

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  , ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  , ButtonToolbar<IDB_QueryBtn, TestBtn             , ToolTestBtn>  
  , ButtonToolbar<IDB_MashBtn, TresholdsViewBtn  , ToolTresholdsViewBtn>
  , SeparatorToolbar<1>
  , ButtonToolbar<IDB_SensorsUnit, ExitTubeButton  , ToolExitTube>
  , SeparatorToolbar<2>
 // , ButtonToolbar<IDB_ClampBtn, TestBase  , ToolTestBase>	  //
>::Result tool_button_list;
bool ex_tube = true;
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
  zprint("");
  automat.AutomatMode();
}
//-------------------------------------------------------------------------------
void TresholdsViewBtn(HWND h)
{
	zprint("");
	DetailedWindow::Open();
	thicknessViewer.openDetailedWindow = true;
	RepaintWindow(DetailedWindow::Instance().hWnd);
}
//-----------------------------------------------------------------------------
void TestBtn(HWND)
{
	zprint("");
	if(automat.TestMode())
	{
		thicknessViewer.Update();
	}
}
//----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	if(!ex_tube) {automat.RotationalSpeedSensorsStop(); ex_tube ^= true;}
	 zprint("");
	 automat.StopMode();
}
//--------------------------------------------------------------------------------------------
HWND MainWindowToolBar::Init(HWND h)
{
	 return hWnd = InitToolbar<tool_button_list>()(h);
}
//-------------------------------------------------------------------
void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------	

 void ExitTubeButton(HWND)
 {
	 if(ex_tube)automat.RotationalSpeedSensorsStart();
	 else automat.RotationalSpeedSensorsStop();
	 ex_tube ^= true;
 }
//test

void TestBase(HWND)
{
	  DataExchange dataExchange;
	  dataExchange.SendData();
}

void MainWindowToolBar::Cycle()
{
	HWND h = app.mainWindow.hWnd;
	SendMessage(h, TB_ENABLEBUTTON, IDB_CycleBtn   , MAKELONG(FALSE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_Reset      , MAKELONG(TRUE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_QueryBtn   , MAKELONG(FALSE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_MashBtn    , MAKELONG(FALSE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(FALSE, 0));
}

void MainWindowToolBar::NoCycle()
{
	HWND h = app.mainWindow.hWnd;
	SendMessage(h, TB_ENABLEBUTTON, IDB_CycleBtn   , MAKELONG(TRUE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_Reset      , MAKELONG(FALSE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_QueryBtn   , MAKELONG(TRUE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_MashBtn    , MAKELONG(TRUE, 0));
	SendMessage(h, TB_ENABLEBUTTON, IDB_SensorsUnit, MAKELONG(TRUE, 0));
}


