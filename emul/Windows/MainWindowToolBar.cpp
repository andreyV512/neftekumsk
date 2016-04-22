#include "stdafx.h"
#include "MainWindowToolBar.h"
#include "MainWindow.h"
#include "Config.h"
#include "DebugMess.h"
#include "Resource.h"
#include "InitToolBar.hpp"
#include "message.h"

void SycleMeashurement(HWND);//обработчик для кнопки "Циклическое измерение"

void StopMeashurement(HWND);//обработчик для кнопки "Остановка измерения"

void TresholdsViewBtn(HWND);

#define TOOL_LIP_TEXT(name, text)struct name{static wchar_t *Do(){return text;}};

TOOL_LIP_TEXT(ToolLipCycleBtn     , L"Цикл")
TOOL_LIP_TEXT(ToolLipReset  	  , L"Стоп")
TOOL_LIP_TEXT(ToolTresholdsViewBtn  , L"Настр.пор.")
  
#undef TOOL_LIP_TEXT

typedef TL::MkTlst<
  SeparatorToolbar<0>
  , ButtonToolbar<IDB_CycleBtn, SycleMeashurement, ToolLipCycleBtn>  
  , ButtonToolbar<IDB_Reset, StopMeashurement    , ToolLipReset>  
  , ButtonToolbar<IDB_OptionsBtn, TresholdsViewBtn, ToolTresholdsViewBtn>
  , SeparatorToolbar<1>
>::Result tool_button_list;
//----------------------------------------------------------------------------------
void SycleMeashurement(HWND h)
{
  zprint("");
  MainWindow *o = (MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA);  
  o->oEmulViewer.Start();
}
//-------------------------------------------------------------------------------
void TresholdsViewBtn(HWND h)
{
 zprint("");
}
//-----------------------------------------------------------------------------
void StopMeashurement(HWND h)
{
	 zprint("");
	 MainWindow *o = (MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA);  
     o->oEmulViewer.Stop();
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


