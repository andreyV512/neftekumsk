#include "stdafx.h"
#include "MedianFiltre.h"
#include <windows.h>
MedianFiltre::MedianFiltre()
	: length_(5)
	//, offset(0)
	, middle(3)
	, length(length_)
{
	//Clear();
}
//------------------------------------------------------------------------
void MedianFiltre::SetLength(int l)
{
	length_ = l;
	if(length > 32) length_ = 32;
	if(0 == (l & 1)) ++length_;
	middle = length / 2 + 1;
}
//------------------------------------------------------------------------
/*
void MedianFiltre::Clear()
{
	offset = 0;
	ZeroMemory(data, sizeof(data));
	ZeroMemory(index, sizeof(index));
}
*/
//-------------------------------------------------------------------------
double MedianFiltre::operator()(double *d, int &offs)
{
	//ZeroMemory(index, sizeof(index));
	int index[32] = {0};
	for(int i = 0; i < length - 1; ++i)
	{
		for(int j = i + 1; j < length; ++j)
		{
			if(d[i] > d[j]) ++index[i];
			else if(d[i] < d[j]) ++index[j];
		}
	}
	int max = 0;
	int ind = 0;
	for(int i = 0; i < length; ++i)
	{
		int z = index[i];
		if(z < middle)
		{
			if(z >= max)
			{
				max = z;
				ind = i;
			}
		}
	}
	offs = ind;
	return d[ind];
}
//----------------------------------------------------------------
bool MedianFiltre::operator()(double *data, char *status)
{
}
//----------------------------------------------------------------