#pragma once
#include <windows.h>
#include "message.h"
#include "typelist.hpp"
#include "DebugMess.h"
//------------------------------------------------------------------------------------------
template<class T>struct MenuListItem{};
template<class T>struct MenuItem{};
template<class T>struct TopMenu{};
template<class T>struct SubMenu{typedef NullType list;};
template<int>struct Separator{};
#pragma warning(disable:4995)
template<class T>struct NameMenu
{
	wchar_t *operator()(HWND)
	{
		static const int i = dimention_of(__FUNCTION__);
		static wchar_t b[i];
		wsprintf(b, L"%S", __FUNCTION__);              
        b[i - 14] = 0;
		return b;
	}
};
template<class T>struct EnableMenuInit
{
	int operator()(HWND)
	{
		return MFS_ENABLED;
	}
};
template<class T>struct Event;
template<class T>struct Event<TopMenu<T> >{static void Do(HWND){}};
template<class T>struct Event<SubMenu<T> >{static void Do(HWND){}};

template<class T>struct ReturnItemMenu
{
	bool operator()(){return true;}
};
//------------------------------------------------------------------------------------------
template<class List>class Menu
{
public:
	HMENU Init(HWND hWnd)
	{
		if(NULL != hWnd) DestroyMenu(GetMenu(hWnd));
		Param param(CreateMenu(), hWnd);
		TL::find<List, __insert_menu__>()((TL::Factory<List> *)0, &param);
		SetMenu(hWnd, param.h); 
		DrawMenuBar(hWnd);
		return param.h;
	}
};
template<class List>class PopupMenu
{
	HMENU hMenu;
public:
	void Init(HWND hWnd)
	{		
		Param param(CreatePopupMenu(), hWnd);
		TL::find<List, __insert_menu__>()((TL::Factory<List> *)0, &param);
		hMenu = param.h;
	}
	void Do(HWND hWnd)
	{
		POINT p;
		GetCursorPos(&p);
		UINT flags = TPM_BOTTOMALIGN | TPM_NONOTIFY | TPM_RETURNCMD;

		flags |= (GetSystemMetrics(SM_MENUDROPALIGNMENT) == 0 ) ? TPM_LEFTALIGN: TPM_RIGHTALIGN;

		if(unsigned d = TrackPopupMenuEx(hMenu, flags, p.x, p.y, hWnd, NULL))
		{	
			MENUITEMINFO mii;
			mii.cbSize = sizeof(MENUITEMINFO);
			mii.fMask = MIIM_DATA;
			if(GetMenuItemInfo(hMenu, d, false, &mii))
			{
				((void (__cdecl *)(HWND))(mii.dwItemData))(hWnd);
			}
		}
	}
	void Destroy()
	{
		DestroyMenu(hMenu);
	}
};
#define index Index()
struct Param
{
	MENUITEMINFO m;
	HMENU h;
	HWND  hWnd;
	int &Index(){static int x; return x;}
	Param(HMENU x, HWND hWnd) : h(x), hWnd(hWnd)
	{
		m.cbSize = sizeof(MENUITEMINFO);	
		m.fType = MFT_STRING;
		m.hSubMenu = NULL;	
		m.cch = 0;
	}
};
template<class O, class P>struct __insert_item_menu__
{
	bool operator()(O *o, P *p)
	{
         debug.print(__FUNCTION__);
		 return true;
	}
};
template<class O, class P>struct __insert_menu__;
template<class>struct MenuItem;
template<class S, class P>struct __insert_item_menu__<MenuItem<S>, P>
{
	typedef MenuItem<S> O;
	bool operator()(O *o, P *p)
	{      
		if(ReturnItemMenu<O>()())
		{
			p->m.fMask = MIIM_DATA | MIIM_TYPE | MIIM_ID | MIIM_STATE;			
			p->m.dwTypeData = NameMenu<O>()(p->hWnd);	
			p->m.dwItemData = (ULONG_PTR)Event<O>::Do;	
		    p->m.wID = p->m.dwItemData & 0xffff;
			p->m.fState = EnableMenuInit<O>()(p->hWnd);
			InsertMenuItem(p->h, p->index++, false, &p->m);
			return true;
		}
		return false;
	}
};
template<class O, class P>struct __insert_menu__
{
	bool operator()(O *o, P *p)
	{	
		Param param(CreatePopupMenu(), p->hWnd);
		TL::find<typename O::list, __insert_item_menu__>()((TL::Factory<typename O::list> *)0, &param);
		p->m.fMask = MIIM_SUBMENU | MIIM_TYPE | MIIM_DATA | MIIM_ID | MIIM_STATE;
		p->m.hSubMenu = param.h;
		p->m.dwTypeData = NameMenu<O>()(p->hWnd);	
		p->m.dwItemData = (ULONG_PTR)Event<O>::Do;			
		p->m.wID = p->m.dwItemData & 0xffff;
		p->m.fState = EnableMenuInit<O>()(p->hWnd);
		InsertMenuItem(p->h, p->index++, false, &p->m);
		return true;
	}
};
template<class S, class P>struct __insert_item_menu__<SubMenu<S>, P>
{
	typedef SubMenu<S> O;
	bool operator()(O *o, P *p)
	{      
		if(ReturnItemMenu<O>()())
		{
			__insert_menu__<O, P>()(o, p);
			return true;
		}
		return false;
	}
};
template<class P, int N>struct __insert_item_menu__<Separator<N>, P>
{	
	bool operator()(Separator<N> *, P *p)
	{        	
		unsigned t = p->m.fType;
		p->m.fType = MFT_SEPARATOR;
		InsertMenuItem(p->h, p->index++, false, &p->m);
		p->m.fType = t;
		return true;
	}
};
template<class T>void EnableMenu(HWND h, bool state = true)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STATE;
	HMENU hMenu = GetMenu(h);
	static const unsigned short id = (unsigned short)Event<T>::Do;
	if(GetMenuItemInfo(hMenu, id, false, &mii))
	{
		mii.fState = state ? MFS_ENABLED : MFS_DISABLED;
		SetMenuItemInfo(hMenu, id, false, &mii);
		DrawMenuBar(h);
	}
}
template<class T>void ChangeTextSubMenu(HWND h, wchar_t *text)
{
	MENUITEMINFO mii;
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;
	mii.dwTypeData = text;
	SetMenuItemInfo(GetMenu(h), (unsigned short)Event<T>::Do, false, &mii);
}

#define MENU_TEXT(txt, item)template<>struct NameMenu<item >{wchar_t *operator()(HWND){return txt;}};
#define MENU_ITEM(txt, item) MENU_TEXT(txt, MenuItem<item>) template<>struct Event<MenuItem<item> >:item{};

void GetMenuToolBarEvent(TCommand &m);
