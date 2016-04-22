#include "stdafx.h"
#include "Dlg.h"
#include "..\DlgTemplates\ParamDlg.hpp"

#define num0 1
#define num1 2
#define num2 3
#define num3 4
#define num4 5
#define num5 6

#define z(n) L#n
#define zz(n) z(n)
#define xx(n) num##n
#define nnn(a, b, c) a##b##c
#define nn(a, b, c) nnn(a, zz(xx(b)), c)

#define PARA(n)\
MIN_EQUAL_VALUE(MinEnergy<n>, 0)\
MAX_EQUAL_VALUE(MinEnergy<n>, 1000)\
MIN_EQUAL_VALUE(MaxEnergy<n>, 0)\
MAX_EQUAL_VALUE(MaxEnergy<n>, 10000)\
MIN_EQUAL_VALUE(Peak<n>, 0.01)\
MAX_EQUAL_VALUE(Peak<n>, 0.9)\
PARAM_TITLE(MinEnergy<n>, nn(L"Сигнал ", n, L" минимальная энергия"))\
PARAM_TITLE(MaxEnergy<n>, nn(L"Сигнал ", n, L" максимальная энергия"))\
PARAM_TITLE(Peak<n>, nn(L"Сигнал ", n, L" энергия к первому пику"))

PARA(0)
PARA(1)
PARA(2)
PARA(3)
PARA(4)
PARA(5)

#undef z
#undef zz
#undef xx
#undef nnn
#undef nn

#undef PARA

//template<int n>struct DlgSubItems<MaxEnergy<n>, double>: NOPItems<MaxEnergy<n> >{};

void SignalDlg::Do(HWND h)
{
	if(TemplDialog<SignalParametersTable>(Singleton<SignalParametersTable>::Instance()).Do(h, L"Параметры сигнала"))
	{
		;
	}
}

