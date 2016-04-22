#pragma once
#include "typelist.hpp"
#include "Base.hpp"
#include "AppBase.h"
#include "message.h"
#include "ParamDlg.hpp"
#include "Pass.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
#if 0
class ParametersDialog
{
	static LRESULT CALLBACK Proc(HWND , UINT , WPARAM , LPARAM );
protected:	
	virtual bool OkBtn(HWND) = 0;
	virtual bool CancelBtn(HWND);
	virtual void Init(HWND h, int &width, int &height) = 0;
	virtual void Command(TCommand &);
public:
	bool Do(HWND hWnd, wchar_t *title);
};

class ParametersDialogOkCancelDelete
{
	static LRESULT CALLBACK Proc(HWND , UINT , WPARAM , LPARAM );
protected:	
	virtual bool OkBtn(HWND) = 0;
	virtual bool DeleteBtn(HWND);
	virtual bool CancelBtn(HWND);
	virtual void Init(HWND h, int &width, int &height) = 0;
	virtual void Command(TCommand &){}
public:
	bool Do(HWND hWnd, wchar_t *title);
};
#endif
//-------------------------------------------------------------------------------------------------------------------------------
template<class T>struct DlgItem;
bool TemplDlg_Do(HWND hWnd, wchar_t *title, DLGPROC proc, LPARAM param);

template<class O, class T>struct ChangeInner;
template<template<class >class O, class Inner, class T>struct ChangeInner<O<Inner>, T>
{
	typedef O<T> Result;
};
// 
template<class O, class P>struct __make_btn__
{
	void operator()(O *o, P *p)
	{
		dprint("%s", __FUNCTION__);
		CreateWindow(L"button", o->Title()
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			,p->offs, p->height, O::width, 30, p->h, (HMENU)O::ID, hInstance, NULL
			);
		p->offs += O::width + 10;
	}
};
template<class TableParam, class ButtonsList = TL::MkTlst<OkBtn, CancelBtn>::Result>class TemplDialog
{
	struct __command_data__
	{
		//TMessage &mes;
		HWND hwnd;
		unsigned id;
		TemplDialog &owner;
	//	__command_data__(TMessage &mes, TemplDialog &owner)
		__command_data__(HWND hwnd, unsigned id, TemplDialog &owner)
			//: mes(mes)
			: hwnd(hwnd)
			, id(id)
			, owner(owner)
		{}
	};
	template<class O, class P>struct __command__
	{
		bool operator()(O *o, P *p)
		{
			//if(p->mes.wParam == O::ID)
			if(p->id == O::ID)
			{
				//o->BtnHandler(p->owner, p->mes.hwnd);
				o->BtnHandler(p->owner, p->hwnd);
				return false;
			}
			return true;
		}
	};
	template<class List>struct __btn_width__;
	template<class Head, class Tail>struct __btn_width__<Tlst<Head, Tail> >
	{
		static const int value = Head::width + __btn_width__<Tail>::value;
	};
	template<>struct __btn_width__<NullType>
	{
		static const int value = 0;
	};
	struct __make_btn_data__
	{
		int offs;
		int height;
		HWND h;
		__make_btn_data__(int offs, int height, HWND h)
			: offs(offs)
			, height(height)
			, h(h)
		{}
	};	
	
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
	{
	case WM_COMMAND:
		{
			TemplDialog *e = (TemplDialog *)GetWindowLong(h, GWL_USERDATA);	
			if(!TL::find<ButtonsList, __command__>()(&e->buttons
				, &__command_data__(h, wParam, *e))
				) return TRUE;
		}
		break;
	case WM_INITDIALOG:
		{			
			SetWindowLong(h, GWL_USERDATA, lParam);
			TemplDialog *e = (TemplDialog *)lParam;
			int width = 450;
			int height = 10;
			TL::foreach<list, __init__>()(&e->items, &__table_data__(h, width, height));
			int offs = __btn_width__<ButtonsList>::value + (TL::Length<ButtonsList>::value - 1) * 10;

			offs = (width - offs) / 2;
			height += 15;

			TL::foreach<ButtonsList, __make_btn__>()(&e->buttons, &__make_btn_data__(offs, height, h));

			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			
			height += 65;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
	}
protected:
public:
	typedef TableParam Table;
	Table &table;	
	typedef typename TL::TypeToTypeLst<typename Table::items_list, DlgItem>::Result list;
	TL::Factory<list> items;
	TL::Factory<ButtonsList> buttons;
public:
	TemplDialog(Table &table_) : table(table_), items(table_.items){}
	bool Do(HWND hWnd, wchar_t *title)
	{
		return TemplDlg_Do(hWnd, title, (DLGPROC)Proc, (LPARAM)this);
	}
};

template<class Table>struct TestPassword;

struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Table>()(h))
		{
			if(__ok_table_btn__<
				Owner::Table
				, typename TL::SubListFromMultyList<ParametersBase::multy_type_list, Owner::Table>::Result
			>()(h, owner))
			{
				EndDialog(h, TRUE);
			}
		}
	}
};

struct CancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
	    EndDialog(h, FALSE);
	}
};

template<class Table>struct TestPassword
{
	bool operator()(HWND h)
	{
		return (0 == TL::TypeInMultyList<ParametersBase::multy_type_list, Table>::Result::value)
			? TypesizePasswordDlg().Do(h)
			: OptionPasswordDlg().Do(h);
	}
};




