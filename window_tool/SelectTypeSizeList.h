#pragma once
#include "window_tool/MessagesInterface.h"
class SelectTypeSizeList: public TEvent
{
	struct TObj{};
	TObj *obj;
	void (TObj::*ptr)();
public:
   HWND hWnd;
public: 
	SelectTypeSizeList();
	void Create(HWND);
	void Size(int x, int y, int width);
	void Do(TCommand &);
	void AddMenuItem(wchar_t *);
	void DelMenuItem(wchar_t *);
	template<class T>void SetCommandHandler(T *t, void (TObj::*t_ptr)())
	{
		obj = (TObj *)t;
		ptr = (void (TObj::*)())t_ptr;
	}
};