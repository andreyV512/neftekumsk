#pragma once

DEFINE_PARAM(PacketSize, int, 474)
DEFINE_PARAM(Gain0, int, 40)
DEFINE_PARAM(SyncLevel, int, 80)
DEFINE_PARAM(SyncGain, int, 10)
DEFINE_PARAM(StartDelay, int, 100)
//----------------------------------------------------------------
DEFINE_PARAM(Frequency, int, 25) //50
DEFINE_PARAM(NumberPackets, int, 100)

DEFINE_PARAM(SyncInput, bool, false)
DEFINE_PARAM(MeasurementInput, bool, false)
DEFINE_PARAM(SynchronizationEdge, bool, true)


struct LanParametersTable
{
	typedef TL::MkTlst<
		PacketSize
		, Gain0
		, SyncLevel
		, SyncGain
		, StartDelay
		, Frequency
		, NumberPackets
		, SyncInput
		, MeasurementInput
		, SynchronizationEdge
	>::Result items_list;
	typedef NullType unique_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"LanParameters";}
};
