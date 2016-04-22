#include "stdafx.h"
#include "Tubes.h"
#include "Chart.h"
#include "DebugMess.h"
using namespace Gdiplus;
Tube::Tube(Chart &chart)
	: chart(chart)
{
	length = 10;
	offsX = 15;
	y = 30;
}
//--------------------------------------------------------------
void Tube::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );

	int width = chart.rect.right - chart.rect.left - chart.offsetAxesRight - chart.offsetAxesLeft;
	double dX = (double)(width) / length;
	int x0 = chart.rect.left + chart.offsetAxesLeft;

	double dx = chart.deltaTickX / chart.deltaDigitX;
	int offs = int(chart.offsetGridX + dx * offsX);

	int w = int(dx * length);

	int offsY = chart.rect.bottom - chart.offsetAxesBottom - y;

	chart.g->FillRectangle(&SolidBrush(Color(color)), offs - w, offsY, w, 10);

	point.x = offs - w;
	point.y = offs;

	chart.g->SetClip(&Region());
	//zprint(" %d", offs - w);
}
//--------------------------------------------------------------
bool Tube::InRect(int x)
{
	return point.x < x && point.y > x;
}
//void Tube::Restart()
//{
//	point.x = 0;
//}
//-------------------------------------------------------------
Sensor::Sensor(Chart &chart)
	: chart(chart)
{
	x = 5;
	y = 5;
}
//----------------------------------------------------------------
void Sensor::Draw()
{
	int offsY = chart.rect.bottom - chart.offsetAxesBottom - y;

	double dx = chart.deltaTickX / chart.deltaDigitX;
	int offs = int(chart.offsetGridX + dx * x);

	unsigned color = 0;
	if(tube->InRect(offs))
	{
		color = 0xff00ff00;
		*port |= bit;
	}
	else 
	{
		color = 0xff777777;
		*port &= ~bit;
	}

	chart.g->FillRectangle(&SolidBrush(Color(color)), offs, offsY - 10, 10, 10);

	PointF origin;
	Font font(L"Arial", (REAL)7, FontStyleBold);
	int len = wcslen(name);
	StringFormat format;
	RectF rect;	
	chart.g->MeasureString(name, len, &font, origin, &format, &rect);
	origin.X = (REAL)(offs + 10);//rect.GetRight());
	origin.Y = REAL(offsY - rect.GetBottom());
	chart.g->DrawString(name, len, &font, origin, &SolidBrush(0xff333333));
}
//-------------------------------------------------------------
//-------------------------------------------------------------
OutPut::OutPut(Chart &chart)
	: chart(chart)
{
	x = 5;
	y = 5;
}
//----------------------------------------------------------------
void OutPut::Draw()
{
	int offsY = chart.rect.bottom - chart.offsetAxesBottom - y;

	double dx = chart.deltaTickX / chart.deltaDigitX;
	int offs = int(chart.offsetGridX + dx * x);

	unsigned color = 0xff777777;
	if(0 != (*port & bit))
	{
		color = 0xffff0000;
	}

	chart.g->FillRectangle(&SolidBrush(Color(color)), offs, offsY - 10, 10, 10);

	PointF origin;
	Font font(L"Arial", (REAL)7, FontStyleBold);
	int len = wcslen(name);
	StringFormat format;
	RectF rect;	
	chart.g->MeasureString(name, len, &font, origin, &format, &rect);
	origin.X = (REAL)(offs + 10);//rect.GetRight());
	origin.Y = REAL(offsY - rect.GetBottom());
	chart.g->DrawString(name, len, &font, origin, &SolidBrush(0xff333333));
}
Arrow::Arrow(Chart &chart)
	: chart(chart)	
{
	x = 2;
	y = 70;
	blink = 5;
}
//---------------------------------------------
void Arrow::Draw()
{
	
	int offsY = chart.rect.bottom - chart.offsetAxesBottom - y;

	double dx = chart.deltaTickX / chart.deltaDigitX;
	int offs = int(chart.offsetGridX + dx * x);
	//chart.g->FillRectangle(&SolidBrush(0xffff0000), offs, offsY, 140, 12);



	POINT left[] = {{0, 0}, {0, 2}, {2, 1}};
	POINT right[] = {{2, 0}, {2, 2}, {0, 1}};

	POINT *p = NULL;

	if(*forward && !*backward) p = left;
	else if(!*forward && *backward) p = right;

	if(NULL != p)
	{
		PointF pf[3];

		blink += 1;
		if(blink > 12) blink = 0;
		//*
		chart.g->SetClip(&Region(RectF(
			REAL(offs)
			, REAL(offsY)
			, REAL(140)
			, REAL(12)
			)),
			CombineModeReplace
			);
		//*/
		for(int i = 0; i < 3; ++i)
		{
			pf[i].X = REAL(p[i].x * 6 + offs + blink - 5);
			pf[i].Y = REAL(p[i].y * 6);
			pf[i].Y += offsY;
		}
		chart.g->FillPolygon(&SolidBrush (0xffffff00), pf, 3);

		for(int k = 0; k < 9; ++k)
		{
			for(int i = 0; i < 3; ++i)
			{
				pf[i].X += 15;		
			}
			chart.g->FillPolygon(&SolidBrush (0xffffff00), pf, 3);
		}
	}
	chart.g->SetClip(&Region());
}
