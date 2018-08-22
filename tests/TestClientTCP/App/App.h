#pragma once
#include <Windows.h>
class CrossData;
class MainWindow;
class CrossViewerData;

class App
{
public:
	static const int zonesCount = 60;
	static const int maxSensorCrossCount = 18;
	static const int adcBufferLength = 10000 * 60;
	static HANDLE ProgrammExitEvent;
	static HANDLE hEventStrobes;
public:
	class MainWindow &mainWindow;
	App();
	void Init();
	void Destroy();
	void MainWindowTopLabel(wchar_t *);
	void MainWindowBottomLabel(int, wchar_t *);
	bool ChangeTypeSize(wchar_t *);
};

extern App app;
