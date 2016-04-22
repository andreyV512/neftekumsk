#include "stdafx.h"
#include "BarSeries.h"

using namespace Gdiplus;
//---------------------------------------------------------------------------------------------------------------
BarSeries::BarSeries(Chart &chart)
    : chart(chart) 
{
	SetColorBarHandler(this, &BarSeries::GetColorBarDefault);
}
//----------------------------------------------------------------------------------------------------------
void BarSeries::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );	
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2;
	double dY = (double)(chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	unsigned color;
	double data;
	REAL y = 0;
	for(int i = 0; (obj->*SetColorBar)(i, data, color); ++i)
	{
		y = (REAL)((data - chart.minAxesY) * dY);
		chart.g->FillRectangle(&SolidBrush(color), (REAL)x, (REAL)(bottom - y), (REAL)dX, (REAL)bottom);
		x += dX;
	}
	chart.g->SetClip(&Region());
}
//-------------------------------------------------------------------------------------------------------------------
void BarSeries::DrawSingleBar(int offs, int count)
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
		CombineModeReplace
		);	
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2 + dX * offs;
	double dY = (double)(chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	unsigned color;
	double data;
	REAL y = 0;
	for(int i = offs, len = offs + count; (obj->*SetColorBar)(i, data, color) && i < len; ++i)
	{
		y = (REAL)((data - chart.minAxesY) * dY);
		chart.g->FillRectangle(&SolidBrush(color), (REAL)x, (REAL)(bottom - y), (REAL)dX, (REAL)bottom);
		x += dX;
	}
	chart.g->SetClip(&Region());
}
//-------------------------------------------------------------------------------------------------------------------
bool BarSeries::GetColorBarDefault(int zone, double &data, unsigned &color)
{
	data = zone % 7;
	color = 0xffff0000;
	if(data > 4.0) {color = 0xff00ff00; return zone < 33;}
	if(data > 2.0) color = 0xffffff00;
	return zone < 33;
}
//-----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------
BarSeriesDouble::BarSeriesDouble(Chart &chart)
    : chart(chart) 
{
	SetColorBarHandler(this, &BarSeriesDouble::GetColorBarDefault);
}
//----------------------------------------------------------------------------------------------------------
void BarSeriesDouble::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );	
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2;
	double dY = (double)(chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	unsigned color;
	double data;

	unsigned color1;
	double data1;
	REAL y = 0;
	for(int i = 0; (obj->*SetColorBar)(i, data, color, data1, color1); ++i)
	{
		y = (REAL)((data - chart.minAxesY) * dY);
		chart.g->FillRectangle(&SolidBrush(color), (REAL)x, (REAL)(bottom - y), (REAL)dX, (REAL)bottom);

		y = (REAL)((data1 - chart.minAxesY) * dY);
		chart.g->FillRectangle(&SolidBrush(color1), (REAL)x, (REAL)(bottom - y), (REAL)dX, (REAL)bottom);
		x += dX;
	}
	chart.g->SetClip(&Region());
}
//-------------------------------------------------------------------------------------------------------------------
void BarSeriesDouble::DrawSingleBar(int offs, int count)
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
		CombineModeReplace
		);	
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);
	double x = chart.rect.left + chart.offsetAxesLeft - dX / 2 + dX * offs;
	double dY = (double)(chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	unsigned color;
	double data;

	unsigned color1;
	double data1;
	REAL y = 0;
	for(int i = offs, len = offs + count; (obj->*SetColorBar)(i, data, color, data1, color1) && i < len; ++i)
	{
		y = (REAL)((data - chart.minAxesY) * dY);
		chart.g->FillRectangle(&SolidBrush(color), (REAL)x, (REAL)(bottom - y), (REAL)dX, (REAL)bottom);
		y = (REAL)((data1 - chart.minAxesY) * dY);
		chart.g->FillRectangle(&SolidBrush(color1), (REAL)x, (REAL)(bottom - y), (REAL)dX, (REAL)bottom);
		x += dX;
	}
	chart.g->SetClip(&Region());
}
//-------------------------------------------------------------------------------------------------------------------
bool BarSeriesDouble::GetColorBarDefault(int zone, double &data, unsigned &color, double &, unsigned &)
{
	data = zone % 7;
	color = 0xffff0000;
	if(data > 4.0) {color = 0xff00ff00; return zone < 33;}
	if(data > 2.0) color = 0xffffff00;
	return zone < 33;
}