#pragma once
#include <windows.h>
#include "message.h"
#include "MainWindowToolBar.h"
#include "EmulViewer.h"
#include "CheckBoxWidget.h"

class MainWindow
{	
	template<int N>struct Check
	{		
		static Check *self;
		bool &value;
		Check(bool &v)
			: value(v)
		{
			self = this;
		}
		bool Init()
		{
			return value;
		}
		static void Command(TCommand &m, bool b)
		{
		    self->value = b;
		}
	};
	template<bool EmulViewer::*val>struct CheckX
	{		
		bool &value;
		CheckX(bool &v): value(v){}
		bool Init(){return value;}
		static void Command(TCommand &m, bool b)
		{
			((MainWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA))->oEmulViewer.*val = b;
		}
	};
	template<int N>struct CheckBoxInputPort
	{
		static const unsigned bit = N;
		bool initValue;
		bool Init(){return initValue;}
		static void Command(TCommand &m, bool b)
		{
			unsigned &t = ((MainWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA))->oEmulViewer.inputPort;
			if(b) t |= 1 << N; else t &= ~(1 << N);
		}
	};
public:
	EmulViewer  oEmulViewer;
	MainWindowToolBar toolBar;
	CheckBoxWidget<CheckX<&EmulViewer::forward> > forwardCheckBox;
	CheckBoxWidget<Check<1> > backwardCheckBox;

	CheckBoxWidget<CheckBoxInputPort<3> > inverterOk;
	CheckBoxWidget<CheckBoxInputPort<5> > controlCircuit;
	CheckBoxWidget<CheckBoxInputPort<6> > signalWork;
	CheckBoxWidget<CheckBoxInputPort<7> > signalSycle;
public:
	HWND hWnd;
	HWND hStatusWindow;
	MainWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	unsigned operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TMessage &);
	void operator()(TKeyDown &);
	void operator()(TRButtonDown &);
};

template<int N> MainWindow::Check<N> *MainWindow::Check<N>::self;
extern MainWindow mainWindow;
