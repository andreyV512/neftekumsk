#include "stdafx.h"
#include "Device1730.h"
#include "DebugMess.h"
#include "ArchiveEvents.h"
#include "Config.h"

using namespace Automation::BDaq;

#define Mess(n)

Device1730::Device1730()
	: device(NULL)
	, dio(NULL)
	, input(archiveEvents.map->input)
	, output(archiveEvents.map->output)
{}
//------------------------------------------------------------------------------
#ifndef DEBUG_ITEMS
bool Device1730::Init(int deviceDescription)
{
	Destroy();
	long errorCode = BDaqDevice::Open( deviceDescription, ModeWrite, device);
	if(BioFailed(errorCode))
	{
		Mess(DeviceOpenError)
		return isOpen = false;
	}
	errorCode = device->GetModule(0,dio);
	if(BioFailed(errorCode)) 
	{
		Mess(GetModuleError)
		return isOpen = false;
	}
	Mess(AllOk)	
	return isOpen = true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{
	if(device != NULL)
	{
		device->Close();
	}
}
//--------------------------------------------------------------------------
bool Device1730::IsOpen()
{
	return isOpen;
}
//--------------------------------------------------------------------------
unsigned Device1730::Read()
{
	dio->DiRead(startPoint, 2, (BYTE *)&input);
	return input;
}
//--------------------------------------------------------------------------
void Device1730::Write()
{
	dio->DoWrite(startPoint, 2, (BYTE *)&output);
}
void Device1730::Write(unsigned out)
{
	output = out;
	dio->DoWrite(startPoint, 2, (BYTE *)&output);
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{
	unsigned res;
	dio->DoRead(startPoint, 2, (BYTE *)&res);
	return res;
}
//----------------------------------------------------------------------------
#else
bool Device1730::Init(int deviceDescription)
{
	Mess(AllOk)	
	return isOpen = true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{
}
//--------------------------------------------------------------------------
bool Device1730::IsOpen()
{
	return isOpen;
}
//--------------------------------------------------------------------------
unsigned Device1730::Read()
{
	return input;
}
//--------------------------------------------------------------------------
void Device1730::Write()
{
}
void Device1730::Write(unsigned out)
{
	output = out;
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{
	return output;
}
#endif


