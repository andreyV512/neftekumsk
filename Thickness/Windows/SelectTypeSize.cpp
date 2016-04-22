#include "stdafx.h"
#include <windowsx.h>
#include "SelectTypeSize.h"
#include "AppBase.h"
#include "MainWindow.h"
#include "EmptyWindow.h"
#include "Pass.h"
extern HINSTANCE hInstance;
//--------------------------------------------------------------
void FillSelect(HWND hWnd)
{
	CBase base(ParametersBase().name());
	if(base.IsOpen())
	{
		ADODB::_RecordsetPtr rec;
		if(base.OpenTable(ParametersTable().name(), rec))
		{
			while (!rec->EndOfFile) 
			{
				wchar_t *c = _bstr_t(rec->Fields->GetItem(NameParam().name())->GetValue());
				ComboBox_AddString(hWnd, c);
				rec->MoveNext(); 
			}
			CurrentParametersTable &param = Singleton<CurrentParametersTable>::Instance();
			Select<CurrentParametersTable>(base).ID(1).Execute(param);
			int id = param.items.get<CurrentID>().value;
			ParametersTable typeSizeParam;
			Select<ParametersTable>(base).ID(id).Execute(typeSizeParam);
			int num = ComboBox_FindStringExact(hWnd, 0, typeSizeParam.items.get<NameParam>().value);
			if(CB_ERR != num) ComboBox_SetCurSel(hWnd, num);
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//-------------------------------------------------------------
void SelectTypeSize::Create(HWND h)
{
	hWnd = CreateWindow(L"COMBOBOX", NULL
		, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST|CBS_AUTOHSCROLL |WS_VSCROLL 
		, 0, 0, 0, 0, h, 0, hInstance, 0
		);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)&SelectTypeSize::Do);
	HFONT font = CreateFont(40,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Times New Roman"); 
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	FillSelect(hWnd);
}
//--------------------------------------------------------------
void SelectTypeSize::Size(int x, int y, int width)
{	
	MoveWindow(hWnd, x, y, width, 525, TRUE);
}
//-----------------------------------------------------------------------
//--------------------------------------------------------------------------

template<class O, class P>struct __update_param__
{
	void operator()(O *o, P *p){}
};

template<class Table, class P>struct __update_param__<ID<Table>, P>
{
	typedef ID<Table> O;
	void operator()(O *o, P *p)
	{
		Select<Table>(*p).ID(CurrentId<O>(*p)).Execute(Singleton<Table>::Instance());
	}
};

struct __update_param_data__
{
	CBase &base;
    MainWindow &mainWindow;
	__update_param_data__(CBase &base, MainWindow &mainWindow) : base(base), mainWindow(mainWindow){}
};


void SelectTypeSize::Do(TCommand &m)
{
	if(1 == m.isAcselerator)
	{
		wchar_t buf[128];
		GetWindowText(m.hControl, buf, dimention_of(buf));
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{			
			NameParam::type_value name;
			name = buf;
			ParametersTable typeSizeParam;
			int id = Select<ParametersTable>(base).eq<NameParam>(name).Execute(typeSizeParam);
			dprint("select type size %d\n", id);
			if(id != 0)
			{
				Singleton<ParametersTable>::Instance().items.get<NameParam>().value = name;
				Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(1).Execute();
				TL::foreach<ParametersTable::items_list, __update_param__>()(&typeSizeParam.items, &base);
				CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
				Select<CurrentParametersTable>(base).ID(1).Execute(current);
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

