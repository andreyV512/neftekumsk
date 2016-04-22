#pragma once
#include "message.h"
#include "DebugMess.h"

/**
* \brief Шаблон для чекбокса 
* \param T - шаблонный параметр пример использования смотри  TestCheckBox
*/

template<class T>class CheckBoxWidget : public T
{
	HWND hWnd;
public:
	CheckBoxWidget(){}
	template<class Z>CheckBoxWidget(Z &z) : T(z){}
	/**
	* \brief выполнить метод в обработчике WM_CREATE
	* \param hOwner - смотри winapi
	* \param txt - текст около кнопки
	* \param bool T::Init(HWND) - инициировать состояние чекбокса(true - галочка включена, false - отключена)
	*/
	void Init(HWND hOwner, const wchar_t *txt)
	{
		hWnd = CreateWindow(L"button", txt
			, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
			, 0, 0, 0, 0, hOwner, NULL, GetModuleHandle(NULL), NULL
			);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)&CheckBoxWidget::Command);
		Button_SetCheck(hWnd, T::Init() ? BST_CHECKED : BST_UNCHECKED);
	}
	/**
	* \brief выполнить в обработчике WM_SIZE
	*/
	void Size(int left, int top, int width, int height)
	{
		MoveWindow(hWnd, left, top, width, height, true);
	}
	bool GetCheck(){return  BST_CHECKED == Button_GetCheck(hWnd);}
private:
	/**
	* \brief выполняется при снятии или установки галочки 
	* \param T::Command(bool) - определить в шаблонном параметре
	*/
	static void Command(TCommand &m)
	{
		(T::Command)(m, BST_CHECKED == Button_GetCheck(m.hControl));
	}
};

template<int N>struct TestCheckBox
{
	bool Init()
	{
		dprint(__FUNCTION__);
		return true;
	}
	static void Command(TCommand &m, bool b)
	{
		dprint(__FUNCTION__"%d", b);
	}
};