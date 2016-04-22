#pragma once

#define DLG_DO(name)struct name{static void Do(HWND);};

DLG_DO(TestDlg)
DLG_DO(StoredSizeMainWindow)

#undef DLG_DO