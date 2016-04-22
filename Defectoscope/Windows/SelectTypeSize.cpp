#include "stdafx.h"
#include <windowsx.h>
#include "SelectTypeSize.h"
#include "AppBase.h"
#include "MainWindow.h"
//#include "EditDlg.h"
#include "EmptyWindow.h"
#include "Pass.h"
//#include "Dialogs.h"
//#include "Corel.h"
//#include "GlobalHandlers.h"
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
			//CurrentParametersTable param;
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
//	LONG l = (LONG)&SelectTypeSize::Do;
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
	void operator()(O *o, P *p)
	{
		typedef typename O::__template_must_be_overridden__ noused;
	}
};

template<class Table, class P>struct __update_param__<ID<Table>, P>
{
	typedef ID<Table> O;
	void operator()(O *o, P *p)
	{
		Select<Table>(p->base).ID(CurrentId<O>(p->base)).Execute(Singleton<Table>::Instance());
	}
};

template<class P>struct __update_param__<NameParam, P>
{
	typedef NameParam O;
	void operator()(O *, P *){}
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
			if(id != 0)
			{
				/*
				Singleton<ParametersTable>::Instance().items.get<NameParam>().value = name;
				Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(1).Execute();
				MainWindow *o = (MainWindow *)GetWindowLongPtr(m.hwnd, GWLP_USERDATA);
				TL::foreach<ParametersTable::items_list, __update_param__>()(&typeSizeParam.items, &__update_param_data__(base, *o));
				RepaintWindow(mainWindow.signalViewer.hWnd);
				//CurrentParametersTable current;
				CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
				Select<CurrentParametersTable>(base).ID(1).Execute(current);
				//corel.Clear();
				corel.LoadFromBase(current.items.get<CurrentID>().value);
				UpdateReferencePointsGrid()();
				*/
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//------------------------------------------------------------
bool TypeSizeNewBtn(HWND hWnd, wchar_t *buf)
{
	int len = wcslen(buf);
	if(0 < len && 128 > len)
	{
		//bool b = false;
		//{
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{			
				Holder<> name;
				name = buf;
				int id = Select<ParametersTable>(base).eq<NameParam>(name).Execute();
				if(id != 0)
				{
					MessageBox(hWnd, L"Название типоразмера есть в базе данных", L"Ошибка", MB_ICONERROR);
					return false;
				}
/*
				CurrentParametersTable &param = Singleton<CurrentParametersTable>::Instance();
				Select<CurrentParametersTable>(base).ID(1).Execute(param);
				//	CurrentParametersTable &param = Singleton<CurrentParametersTable>::Instance();
				id = param.items.get<CurrentID>().value;
				ParametersTable typeSizeParam;
				Select<ParametersTable>(base).ID(id).Execute(typeSizeParam);
				typeSizeParam.items.get<NameParam>().value = buf;
				Insert_Into<ParametersTable>(typeSizeParam, base).Execute();
				id = Select<ParametersTable>(base).eq<NameParam>(name).Execute();
				Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(1).Execute();
				MainWindow *o = (MainWindow *)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
				ComboBox_AddString(o->select.hWnd, buf);
				int num = ComboBox_FindStringExact(o->select.hWnd, 0, typeSizeParam.items.get<NameParam>().value);
				if(CB_ERR != num) ComboBox_SetCurSel(o->select.hWnd, num);

				CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
				Select<CurrentParametersTable>(base).ID(1).Execute(current);
				corel.Clear();
				corel.NewTypeSizeItem(id, name.buffer);		
				*/
				return true;
			}			
		MessageBox(hWnd, L"Не могу открыть базу данных", L"Ошибка", MB_ICONERROR);
		return false;
	}
	MessageBox(hWnd, L"Строка должна быть в пределе 1 - 120 символов", L"Ошибка", MB_ICONERROR);
	return false;
}
//-----------------------------------------------------------------
/*
void SelectTypeSizeNew::Do(HWND h)
{	
	if(TypesizePasswordDlg().Do(h))
	{
	EditDlg(h, L"Создать типоразмер", L"Поименовать типоразмер", TypeSizeNewBtn);
//	RepaintWindow(thicknessViewer.hWnd);
	}
}
//------------------------------------------------------------
//-------------------------------------------------------------
void SelectTypeSizeDelete::Do(HWND hWnd)
{	
	if(TypesizePasswordDlg().Do(hWnd))
	{
	CBase base(ParametersBase().name());
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}	
	if(1 < i)
	{	
		MainWindow *o = (MainWindow *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		wchar_t buf[128];
		GetWindowText(o->select.hWnd, buf, dimention_of(buf));
		NameParam::type_value txt;
		txt = buf;
		Delete<ParametersTable>(base).eq<NameParam>(txt).Execute();
		int num = ComboBox_FindStringExact(o->select.hWnd, 0, buf);
		ComboBox_DeleteString(o->select.hWnd, num);
		ComboBox_SetCurSel(o->select.hWnd, 0);

		GetWindowText(o->select.hWnd, buf, dimention_of(buf));
		txt = buf;
		ParametersTable typeSizeParam;
		int id = Select<ParametersTable>(base).eq<NameParam>(txt).Execute(typeSizeParam);
		if(id != 0)
		{
			Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(1).Execute();
			TL::foreach<ParametersTable::items_list, __update_param__>()(&typeSizeParam.items, &__update_param_data__(base, *o));
		}
		//RepaintWindow(thicknessViewer.hWnd);
	}
	else
	{
		MessageBox(hWnd, L"Количество типоразмеров должно быть больше 1", L"Сообщение", MB_ICONINFORMATION);
	}
	}
}
*/
//------------------------------------------------------------