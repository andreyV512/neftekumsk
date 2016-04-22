#pragma once
#include <windows.h>
#include <commctrl.h>
#include "typelist.hpp"
#include "Resource.h"
extern HINSTANCE hInstance;

template<class T>struct __tool_btn_width__
{
	static const int value = 10;
};
template<int idb, void(*ptr)(HWND), class z, template<int, void(*)(HWND), class>class W>struct __tool_btn_width__<W<idb, ptr, z> > 
{
	static const int value = 71;
};
template<class List>struct __tool_bar_width__;
template<class Head, class Tail>struct __tool_bar_width__<Tlst<Head, Tail> >
{
	static const int value = __tool_btn_width__<Head>::value + __tool_bar_width__<Tail>::value;
};
template<>struct __tool_bar_width__<NullType>
{
	static const int value = 0;
};	

struct ToolTipDefault{static wchar_t *Do(){return L"";}};
template<int idb, void(*ptr)(HWND), class TOOLTIP = ToolTipDefault>struct ButtonToolbar
{
	static const int IDB_ = idb;
	static const int TBSTYLE_ = TBSTYLE_BUTTON | BTNS_SHOWTEXT;
	typedef TOOLTIP ToolLip;
	static void Do(HWND h)
	{
		(*ptr)(h);
	}
};
template<int N>struct SeparatorToolbar
{
   static const int IDB_ = IDB_SEP;
   static const int TBSTYLE_ = TBSTYLE_SEP;
};

template<class list, int Height = 64, int SEPARATOR_WIDTH = 10>class InitToolbar
{
	typedef InitToolbar Self;
	HIMAGELIST himl;
	TBBUTTON tbb[TL::Length<list>::value];
    template<class T>struct no_IDB_SEP
	{
		static const bool value = IDB_SEP != T::IDB_;
	};
public:
	typedef typename TL::EraseAllParam<list, no_IDB_SEP>::Result only_buttons_list;	
private:
	template<int>struct Set
	{
		template<class O>void Do(TBBUTTON &tbb, Self *self)
		{
			HBITMAP hbmp = LoadBitmap(hInstance, MAKEINTRESOURCE(O::IDB_)); 
			ImageList_AddMasked(self->himl,hbmp, RGB(255,255,255));  
			DeleteObject(hbmp);

			tbb.iBitmap  = TL::IndexOf<only_buttons_list, O>::value;
			tbb.dwData = (DWORD_PTR)O::Do;
			tbb.idCommand =  O::IDB_;				
			tbb.fsState = TBSTATE_ENABLED;
			tbb.fsStyle = O::TBSTYLE_; 
			tbb.iString = (INT_PTR)O::ToolLip::Do();
		}
	};
	template<>struct Set<IDB_SEP>
	{
		template<class O>void Do(TBBUTTON &tbb, Self *self)
		{
			tbb.iBitmap  = SEPARATOR_WIDTH;
			tbb.fsStyle = O::TBSTYLE_; 
		}
	};
	template<class O, class P>struct loc
	{
		void operator()(O *, P *p)
		{
			Set<O::IDB_>().Do<O>(p->tbb[TL::IndexOf<list, O>::value], p);
		};
	};	    
	public:
#if 1
		HWND operator()(HWND hwnd)
		{
			memset(tbb, 0, sizeof(tbb));
			himl = ImageList_Create(Height, Height, ILC_COLOR24|ILC_COLORDDB|ILC_MASK, TL::Length<only_buttons_list>::value, 0);
			TL::foreach<list, loc>()((TL::Factory<list> *)0, this);
			HWND hToolBar = CreateToolbarEx(hwnd, TBSTYLE_FLAT | CCS_ADJUSTABLE | CCS_NODIVIDER | WS_CHILD | WS_VISIBLE
				, (UINT)tbb
				, TL::Length<list>::value, hInstance, NULL
				, tbb, TL::Length<list>::value,0,0,0,0, sizeof(TBBUTTON)
				);
			//Свяжите image list с ToolBar -ом
			SendMessage(hToolBar, TB_SETIMAGELIST, 0, (LPARAM)himl);
			SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);
			return hToolBar;
		}	
#else
		HWND operator()(HWND hwnd)
		{
			memset(tbb, 0, sizeof(tbb));
			himl = ImageList_Create(Height, Height, ILC_COLOR24|ILC_COLORDDB|ILC_MASK, TL::Length<only_buttons_list>::value, 0);
			TL::foreach<list, loc>()((TL::Factory<list> *)0, this);
			HWND hToolBar = CreateToolbarEx(hwnd, WS_CHILD | WS_VISIBLE | WS_BORDER 
                                      | TBSTYLE_FLAT
                                      | WS_CLIPSIBLINGS
                                      | WS_CLIPCHILDREN
                                      | TBSTYLE_TOOLTIPS
                                      | CCS_NODIVIDER
                                      | CCS_TOP
                                      | CCS_NODIVIDER
				, (UINT)tbb
				, TL::Length<list>::value, hInstance, NULL
				, tbb, TL::Length<list>::value,0,0,0,0, sizeof(TBBUTTON)
				);
			//Свяжите image list с ToolBar -ом
			SendMessage(hToolBar, TB_SETIMAGELIST, 0, (LPARAM)himl);
			SendMessage(hToolBar, TB_AUTOSIZE, 0, 0);
			SendMessage(hToolBar, TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_MIXEDBUTTONS);
			SendMessage(hToolBar, TB_SETMAXTEXTROWS, 0, 0);
			return hToolBar;
		}	
#endif
	};