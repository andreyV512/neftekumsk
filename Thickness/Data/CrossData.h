#pragma once

class CrossData
{
public:
	int currentOffset;
	double buffer[App::maxSensorCrossCount][App::adcBufferLength];
	void Start();
};

class CrossViewerData
{
public:
    int currentOffset;
	double buffer[App::maxSensorCrossCount][App::zonesCount];
	int status[App::maxSensorCrossCount][App::zonesCount];
	int offsets[App::zonesCount + 1];
	CrossViewerData();
};