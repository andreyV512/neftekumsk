#pragma once
#include "windows.h"
static const int WM_GET_SCAN_CODE = WM_USER + 5;//0x400;
#undef HOOK_KEY_DLL
#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall SetHWND(HWND h);
#pragma comment (linker, "/export:SetHWND=_SetHWND@4")
extern "C"__declspec(dllexport) void __stdcall StartKeyHook(HWND h);
#pragma comment (linker, "/export:StartKeyHook=_StartKeyHook@4")
#else
void  SetHWND(HWND h);
void  StartKeyHook(HWND h);
#endif


