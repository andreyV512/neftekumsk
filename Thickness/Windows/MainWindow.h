#pragma once
#include <windows.h>
#include "message.h"
#include "TopLabelViewer.h"
#include "CheckBoxWidget.h"
#include "MainWindowToolBar.h"
#include "window_tool\SelectTypeSizeList.h"

class ThicknessViewer;

class MainWindow
{
public:
	SelectTypeSizeList select;
	HWND hWnd;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
//	CheckBoxWidget<TestCheckBox<0> > testCheckBox0;
//	CheckBoxWidget<TestCheckBox<1> > testCheckBox1;
//	CheckBoxWidget<TestCheckBox<2> > testCheckBox2;
	TopLabelViewer oTopLabelViewer;
	//CrossViewer oCrossViewer;
	ThicknessViewer &oThicknessViewer;
	MainWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
	void operator()(TMouseWell &);
};
