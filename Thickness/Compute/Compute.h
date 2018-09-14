#pragma once
#include <windows.h>
#include "FFT.h"
#include "AppBase.h"
#include "PrimaryData.h"
#include "MedianFiltre.h"
#include "List.hpp"
class Compute
{	
	void SubRecalculation(int, int);
	void DeathZoneFront(int);
	void DeathZoneBack(int);
public:
	static const int FirstClassPipe       = 1;
	static const int TubeOfTheSecondClass = 2;
	static const int DefectivePipe        = 3;
	template<class , class >struct __init_offset__;
	template<class , class >struct __init_energy__;
	template<class , class >struct __init_left_acf__;
	template<class , class >struct __init_right_acf__;
	struct Point{unsigned start, stop;};
	List<Point> list;
	static const int countTreshold = 2;
	//int trimOffset;
	PrimaryData &primaryData;
	HANDLE hThread[countTreshold];
    double deltaTime;
	unsigned deltaTimePaintMarker;
	unsigned lastOffset;
	unsigned lastZone; //
	unsigned lastFrame;
	//const unsigned &referenceOffset1;
	//const unsigned &referenceOffset2;
	//const unsigned &referenceOffset3;
	//const unsigned &referenceOffset4;
	const unsigned referenceOffsetSQ3;
	//const int &frequencyFrames;
	const int &frameSize;
	HANDLE hEvent;
	CRITICAL_SECTION cs0, cs1;
	CFFT fft;
	ThicknessData &thicknessData;
	ThicknessData sensorsData[count_sensors];
	unsigned minOffset[count_sensors];
	unsigned maxOffset[count_sensors];
	//unsigned leftAcf[count_sensors];
	//unsigned rightAcf[count_sensors];
	double minEnergy[count_sensors];
	double maxEnergy[count_sensors];
public:
	double peak[count_sensors];
public:
	double coefficientA[count_sensors];
	double coefficientB[count_sensors];
public:
	/*
	double &nominalThickness;
	double &nominalPercentMin;
	double &nominalPercentMax;
	double &defectDifferentWallPercent;
	*/
	double &treshold2Class;
	double &treshold3Class;
	double &tresholdDefect;
	//double lengthTube;    
	int &deadAreaMM0;
	int &deadAreaMM1;
	double offsetOfZonesOffset;
	MedianFiltre filtre[count_sensors];
	const int &medianFiltreLength;
	int acfBorderLeft[count_sensors];
	int acfBorderRight[count_sensors];
	int stop;
	int cuttingZone0;
	int cuttingZone1;
	char usefulPiecePipeOk;
	//double defectBorderMin;
	//double defectBorderMax;
	//double defectBorder3;
	static void ThreadProc(Compute *);	
public:
	int allData[count_sensors];
	int goodData[count_sensors];
	Compute(PrimaryData &);
	unsigned deltaTimeZoneStart;
	unsigned deltaTimeZoneStop1;
	unsigned deltaTimeZoneStop2;
	unsigned stopMeshuring;
	void MeshuringBaseStart();
	void EndAdjustmentsTube();


	void CalculationFftFrame();
	void Calculation(int, int);

	void InitParam();

	void MeshuringBaseStop1();
	void MeshuringBaseStop2();

	void Calculation();
	void CalculationOneFrame(int sensor, char *data, double &result, char &status);
	void Stop();
	void Destroy();

	void Recalculation();
	void CalculationZones();
	void StopMeshuring();
};

extern Compute compute;

