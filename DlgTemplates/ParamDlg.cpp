#include "stdafx.h"
#include "ParamDlg.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else	
#define  dprint
#endif
//-----------------------------------------------------------------------------------------
#pragma warning(disable : 4996)
#if 0
bool ParametersDialog::Do(HWND hWnd, wchar_t *title)
{
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	bool result = 0 != DialogBoxIndirectParam(hInstance, &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
    LocalFree((HLOCAL)p);
	return result;
}
//-------------------------------------------------------------------------
bool ParametersDialog::CancelBtn(HWND h)
{
	return true;
}
//----------------------------------------------------------------------------------------
LRESULT CALLBACK ParametersDialog::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				ParametersDialog *e = (ParametersDialog *)GetWindowLong(h, GWL_USERDATA);				
				if(e->OkBtn(h)) EndDialog(h, TRUE);
			}
			return TRUE;
		case IDCANCEL: 
			{
				ParametersDialog *e = (ParametersDialog *)GetWindowLong(h, GWL_USERDATA);		
				if(e->CancelBtn(h)) EndDialog(h, FALSE);
			}
			return TRUE;
		default : ((ParametersDialog *)GetWindowLong(h, GWL_USERDATA))->Command((TCommand &)h);
		}		
		break;
	case WM_INITDIALOG:
		{			
			SetWindowLong(h, GWL_USERDATA, lParam);
			ParametersDialog *e = (ParametersDialog *)lParam;
			int width = 450;
			int height = 10;
			e->Init(h, width, height);
			int offs = width / 2 - 120;
			height += 5;
			CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, height, 110, 30, h, (HMENU)IDOK, hInstance, NULL
				);
			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs + 130, height, 110, 30, h, (HMENU)IDCANCEL, hInstance, NULL
				);
			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			
			height += 73;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}
/*
struct TCommand
{
	HWND hwnd;
	UINT uMsg;
	WORD id;
	WORD isAcselerator;
	HWND hControl;
};
*/
 void ParametersDialog::Command(TCommand &l)
 {
	 dprint("%d %d %d %d", l.uMsg, l.id, l.isAcselerator, l.hControl);
 }
//////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
bool ParametersDialogOkCancelDelete::Do(HWND hWnd, wchar_t *title)
{
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	bool result = 0 != DialogBoxIndirectParam(hInstance, &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
    LocalFree((HLOCAL)p);
	return result;
}
//-------------------------------------------------------------------------
bool ParametersDialogOkCancelDelete::CancelBtn(HWND h)
{
	return true;
}
//--------------------------------------------------------------------------
bool ParametersDialogOkCancelDelete::DeleteBtn(HWND)
{
	return true;
}
//----------------------------------------------------------------------------------------
LRESULT CALLBACK ParametersDialogOkCancelDelete::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				ParametersDialogOkCancelDelete *e = (ParametersDialogOkCancelDelete *)GetWindowLong(h, GWL_USERDATA);				
				if(e->OkBtn(h)) EndDialog(h, TRUE);
			}
			return TRUE;
		case IDCANCEL: 
			{
				ParametersDialogOkCancelDelete *e = (ParametersDialogOkCancelDelete *)GetWindowLong(h, GWL_USERDATA);		
				if(e->CancelBtn(h)) EndDialog(h, FALSE);
			}
			return TRUE;
		case IDABORT:
			{
				ParametersDialogOkCancelDelete *e = (ParametersDialogOkCancelDelete *)GetWindowLong(h, GWL_USERDATA);				
				if(e->DeleteBtn(h)) EndDialog(h, TRUE);
			}
			return TRUE;
		}
		((ParametersDialogOkCancelDelete *)GetWindowLong(h, GWL_USERDATA))->Command((TCommand &)h);
		break;
	case WM_INITDIALOG:
		{			
			SetWindowLong(h, GWL_USERDATA, lParam);
			ParametersDialogOkCancelDelete *e = (ParametersDialogOkCancelDelete *)lParam;
			int width = 450;
			int height = 10;
			e->Init(h, width, height);
			int offs = width / 3 - 120;
			height += 5;
			CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, height, 110, 30, h, (HMENU)IDOK, hInstance, NULL
				);
			offs += 130;
			CreateWindow(L"button", L"Закрыть"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs, height, 110, 30, h, (HMENU)IDCANCEL, hInstance, NULL
				);
			offs += 130;
			CreateWindow(L"button", L"Удалить"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs, height, 110, 30, h, (HMENU)IDABORT, hInstance, NULL
				);
			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			
			height += 73;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}
#endif
//--------------------------------------------------------------------------------
bool TemplDlg_Do(HWND hWnd, wchar_t *title, DLGPROC proc, LPARAM param)
{
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	bool result = 0 != DialogBoxIndirectParam(hInstance, &d, hWnd, proc, param);
    LocalFree((HLOCAL)p);
	return result;
}


