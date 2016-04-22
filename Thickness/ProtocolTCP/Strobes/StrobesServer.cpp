#include "stdafx.h"
#include "StrobesProtocol.h"
#include "../Frame.h"
#include "../NetFrame.h"
#include "CommunicationWapper\CommunicationWapper.h"
#include "DebugMess.h"
#include "ThicknessData.h"
#include "PrimaryData.h"

struct StrobeData
{
	int count;
	unsigned data[60];
};

namespace StrobesProtocol
{
	bool Server::Do(Frame *f)
	{
		unsigned (&offs)[ThicknessData::MAX_ZONES_COUNT] = primaryData.indexOffsetZone;
		StrobeData *strobe = (StrobeData *)f->data.buf;

		if(strobe->count > ThicknessData::MAX_ZONES_COUNT) strobe->count = ThicknessData::MAX_ZONES_COUNT;

		thicknessData.countZones = strobe->count - 1;
		primaryData.countZones = strobe->count - 1;
		memset(offs, 0, sizeof(offs));
		memmove(offs, strobe->data, sizeof(unsigned) * thicknessData.countZones);
		
		wcscpy_s((wchar_t *)f->data.buf, 6, L"Ok");		
		f->length = 6;
		dprint("Strobes 0#############\n");
		SetEvent(App::hEventStrobes);
		dprint("Strobes 1#############\n");
		return true;
	}
}