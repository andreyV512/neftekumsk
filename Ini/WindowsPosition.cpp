#include "stdafx.h"
#include "WindowsPosition.h"
#include <Shlwapi.h>
#include "ItemIni.h"
#include "typelist.hpp"

#pragma warning(disable : 4996)
namespace {
bool TestWindowRect(RECT &inp)
{
	RECT r;
	GetWindowRect(0, &r);
	return (unsigned)inp.left < (unsigned)r.right && (unsigned)inp.top < (unsigned)r.bottom;// && inp.right > 200 && inp.bottom > 200;
}
void GetPath(wchar_t (&path)[1024])
{
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\windows.ini");
}
}

#pragma warning(disable: 4996)

void WindowPosition::Get_(const char *n, RECT &r)
{
	wchar_t path[1024];
	GetPath(path);

	wchar_t name[256];
	mbstowcs(name, &n[6], dimention_of(name));
	r.left   = ItemIni::Get(name, L"X", CW_USEDEFAULT, path);
	r.top    = ItemIni::Get(name, L"Y", CW_USEDEFAULT, path);
	r.right  = ItemIni::Get(name, L"Width", CW_USEDEFAULT, path);
	r.bottom = ItemIni::Get(name, L"Height", CW_USEDEFAULT, path);
 /*
	if(!TestWindowRect(r))
	{
		r.left   = 	20;
		r.top    = 20;
		r.right  = 200;
		r.bottom = 150;
	}
	*/
}
void WindowPosition::Set_(const char *n, RECT &r)
{
	wchar_t path[1024];
	GetPath(path);

	if(!TestWindowRect(r))
	{
		r.left   = CW_USEDEFAULT;
		r.top    = CW_USEDEFAULT;
		r.right  = CW_USEDEFAULT;
		r.bottom = CW_USEDEFAULT;
	}

	wchar_t name[256];
	mbstowcs(name, &n[6], dimention_of(name));

	ItemIni::Set(name, L"X"     , r.left  , path);
	ItemIni::Set(name, L"Y"     , r.top   , path);				 
	ItemIni::Set(name, L"Width" , r.right , path);
	ItemIni::Set(name, L"Height", r.bottom, path);
}