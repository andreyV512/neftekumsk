#pragma once
#include "SelectTypeSize.h"
#include <windows.h>
#include "message.h"
#include "CrossViewer.h"
#include "TopLabelViewer.h"
#include "CheckBoxWidget.h"
#include "MainWindowToolBar.h"

class MainWindow
{
public:
	SelectTypeSize select;
	HWND hWnd;
	HWND hStatusWindow;
	MainWindowToolBar toolBar;
	CheckBoxWidget<TestCheckBox<0> > testCheckBox0;
	CheckBoxWidget<TestCheckBox<1> > testCheckBox1;
	CheckBoxWidget<TestCheckBox<2> > testCheckBox2;
	TopLabelViewer oTopLabelViewer;
	CrossViewer oCrossViewer;
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
};
