#pragma once
class Automat
{
	bool testMode, workMode, exitTubeMode;
	static DWORD WINAPI Proc(PVOID);
	static DWORD WINAPI ProcTest(PVOID);
	static DWORD WINAPI ProcExitTube(PVOID);
	void Do();
	void DoTest();
	void ExitTubeDo();
public:
	Automat();
	void Init();
	void Destroy();
	void AutomatMode();
	void SingleMode();
	void StopMode();
	bool TestMode();
	void ExitTube();
	static void RotationalSpeedSensorsStart();
	static void RotationalSpeedSensorsStop();
};
extern Automat automat;

