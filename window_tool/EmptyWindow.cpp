#include "stdafx.h"
#include "EmptyWindow.h"
extern HINSTANCE hInstance;
ATOM MyRegisterClass(WNDPROC WndProc
					 , const wchar_t *windowClassName
					 , int style
					 , int idIcon
					 , int idSmIcon
					 , HBRUSH backGround
					 )
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= style;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(idIcon));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= backGround;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= windowClassName;
	wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(idSmIcon));
	return RegisterClassEx(&wcex);
}

HWND MyCreateWindow(const wchar_t *windowClass, const wchar_t *title					
					, void *data
					, int style
					, HWND hParent
					, int x, int y
					, int width, int height  
					)
{
	return CreateWindowEx(WS_EX_CLIENTEDGE, windowClass, title, style, x, y, width, height, hParent, NULL, hInstance, data);   
}
//----------------------------------------------------------------
HWND CreateChildWindow(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data)
{
	MyRegisterClass(WndProc, classStr, CS_DBLCLKS);
	return CreateWindowEx(WS_EX_CLIENTEDGE, classStr, L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hParent, NULL, hInstance, data);  
}
//-----------------------------------------------------------------------------------------------------
HWND CreateChildWindowBackground(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data)
{
	MyRegisterClass(WndProc, classStr, CS_DBLCLKS, NULL, NULL, (HBRUSH)COLOR_WINDOW);
	return CreateWindowEx(WS_EX_CLIENTEDGE, classStr, L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hParent, NULL, hInstance, data);  
}
//----------------------------------------------------------------------------------------------------
void RepaintWindow(HWND h)
{
	RECT r;
	GetClientRect(h, &r);
	SendMessage(h, WM_SIZE, 0, (r.bottom << 16) | r.right); 
	InvalidateRect(h, NULL, true);
}