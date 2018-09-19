#include "stdafx.h"
#pragma warning(disable :4995)
#pragma warning(disable :4996)
#include "Dlg.h"
#include "MainWindow.h"
//#include "SaveSizeWindow.h"
#include <Shlwapi.h>
#include "LogBuffer.h"
#include <iostream>
#include <fstream>
#include <locale>



void SaveDateFile::Do(HWND h)
{
	KillTimer(h, MessWindow::IDT_TIMER1);
	wchar_t path[512];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\ArchiveEvent\\");
	CreateDirectory(path, NULL);
	int length = wcslen(path);
	time_t t = time(NULL);
	tm *timeinfo = localtime (&t);
	wsprintf(&path[length], L"%.2d%.2d%.2d%.2d%.2d%.2d.txt"
		, timeinfo->tm_year - 100, 1 + timeinfo->tm_mon, timeinfo->tm_mday
		, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec
		);	
	char c[512];
	std::ofstream out(path);
	out.imbue(std::locale(""));
	Log::TData *d = NULL;
	Log::TData *d0 = NULL;
	for(int i = 0; i < 1024; ++i)
	{
		if(Log::IsRow(i, d))
		{
			int tme = Log::IsRow(i + 1, d0) ? d->time - d0->time: 0;
			sprintf(c, "%7d   ", tme);
			int len = strlen(c);
			LogMess::FactoryMessages::Instance().Text(d->id, &c[len], d->value);
			strcat(c, "\n");
			out << c;
		}
	}
	out.close();

	MessageBox(h, L"Данные сохранены", L"Сообщение", MB_ICONINFORMATION);
	SetTimer(h,             // хэндл главного окна
		MessWindow::IDT_TIMER1,            // идентификатор таймера
		1000,                 // интервал - 1 секунд
		(TIMERPROC) NULL);     // процедуры таймера нет
}

void MainExit::Do(HWND h)
{
	DestroyWindow(h);
}

