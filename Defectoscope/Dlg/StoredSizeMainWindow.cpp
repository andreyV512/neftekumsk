#include "stdafx.h"
#include "Dlg.h"
#include <typeinfo.h>
#include "MainWindow.h"
#include "SaveSizeWindow.h"

void StoredSizeMainWindow::Do(HWND h)
{
	RECT r;
	GetWindowRect(h, &r);
	r.right -= r.left;
	r.bottom -= r.top;
	SizeWindow().Set(*(MainWindow *)NULL, r);
}
