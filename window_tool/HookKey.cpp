#include "StdAfx.h"
#include "HookKey.h"

#include <windows.h>
#include <winuser.h>
#include <stdio.h>

HHOOK hKeyHook;
HWND hWnd;
extern HINSTANCE hInstance;
__declspec(dllexport) LRESULT CALLBACK KeyEvent (

  int nCode,      // The hook code
  WPARAM wParam,  // The window message (WM_KEYUP, WM_KEYDOWN, etc.)
  KBDLLHOOKSTRUCT *hooked   // A pointer to a struct with information about the pressed key

) {
    if  ((nCode == HC_ACTION) &&       // HC_ACTION means we may process this event
        ((wParam == WM_SYSKEYDOWN) ||  // Only react if either a system key ...
        (wParam == WM_KEYDOWN)))       // ... or a normal key have been pressed.
    {
		PostMessage(hWnd, WM_GET_SCAN_CODE, 0, hooked->vkCode);
    }
    return CallNextHookEx(hKeyHook,
        nCode,wParam,(LPARAM)hooked);

}
void MsgLoop()
{
    MSG message;
    while (GetMessage(&message,NULL,0,0)) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }
}
DWORD WINAPI KeyLogger(LPVOID lpParameter)
{
    hKeyHook = SetWindowsHookEx (  // install the hook:
        WH_KEYBOARD_LL,            // as a low level keyboard hook
        (HOOKPROC) KeyEvent,       // with the KeyEvent function from this executable
        hInstance,                      // and the module handle to our own executable
        NULL                       // and finally, the hook should monitor all threads.
    );
    MsgLoop();
    UnhookWindowsHookEx(hKeyHook);
    return 0;
}
#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall SetHWND(HWND h)
#else
void SetHWND(HWND h)
#endif
{
	hWnd = h;
}
#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall StartKeyHook(HWND h)
#else
void StartKeyHook(HWND h)
#endif
{
	SetHWND(h);
	CloseHandle(CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)KeyLogger, NULL, NULL, NULL));
}

