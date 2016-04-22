#include "stdafx.h"
#include "GridItems.h"
void SetRow(HWND hWnd, int count)
{
	LV_ITEM l;
	l.mask = LVIF_TEXT | LVIF_IMAGE;
	for(int i = 0; i < count; ++i)
	{
		l.iItem = i;
		l.iSubItem = 0;
		l.pszText = LPSTR_TEXTCALLBACK;
		l.cchTextMax = 0;
		ListView_InsertItem(hWnd, &l);
	}
}