#pragma once

#define DLG_DO(name)struct name{static void Do(HWND);};



DLG_DO(SaveDateFile)
DLG_DO(MainExit)
//DLG_DO(SaveCoordinateWindow)

#undef DLG_DO