#pragma once
#define LAN_DEVICE_TIME_OUT L"LanDeviceTimeOut"
#define LAN_DEVICE_FRAME_READY L"LanDeviceFrameReady"
namespace Lan
{
	extern const size_t &bufSize;
	unsigned Init();
	void Destroy();
	unsigned SetParams();
	void Start();
	void Stop();
}