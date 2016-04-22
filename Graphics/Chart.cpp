#include "stdafx.h"
#include <math.h>
#include <limits>
#include "Chart.h"
#include "AppConst.h"


//#include "Debug1730.h"
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif

using namespace Gdiplus;
const int COLOR_AXES = 0xff777777;
const int COLOR_AXES_FONT = 0xff000033;
const int COLOR_GRID = 0xff007700;
const int BACK_GROUND = 0xffdddddd;
Chart::Chart(Gdiplus::Bitmap *&backScreen)
  : offsetAxesLeft(52)
  , offsetAxesBottom(20)
  , offsetAxesTop(0)
  , offsetAxesRight(10)
  , minAxesX(0)
  , maxAxesX(1)
  , minAxesY(0)
  , maxAxesY(1)
  , minAxesYRight(-1.0)
  , maxAxesYRight(1.0)
  , colorAxes(COLOR_AXES)
  , colorFontAxes(COLOR_AXES_FONT)
  , fontHeight(7)
  , colorGrid(COLOR_GRID)
  , backScreen(backScreen)
  , dScaleX(1.0)
  {
}
Chart::~Chart()
{
}
//---------------------------------------------------------------------
#pragma warning(disable : 4996)
//--------------------------------------------------------------------------------------
void OffsetAxes(int offs, int length, double min, double max, double &digit, double &tick, double &minDigit, double &minTick)
{
	double dLength = (double)length / offs;
	double dValue = (max - min) / dLength;
	double coeff = 1.0;
	if(max == min) dValue = 1.0;

	if(dValue > 1.0)
	{
		while(dValue > 10.0f)
		{
			dValue *= 0.1f;
			coeff *= 10.0f;
		}		
	}
	else
	{
		while(dValue < 1.0f)
		{
			dValue *= 10.0f;
			coeff *= 0.1f;
		}
	}
	double k[] = {1.0, 2.0, 5.0};
	digit = k[0] * coeff;
	tick =  offs * k[0] / dValue;
	for(int i = 1; i < dimention_of(k); ++i)
	{
		double t = offs * k[i] / dValue;
		if(t < offs)
		{
			digit = k[i] * coeff;
			tick =  t;
		}
	}
	double rem = fmod(min, digit);
	if(rem >= 0.0)
	{
		double x = rem / digit;
		if(x > 0.9) rem = 0.0;
		minTick = tick * (1 - (digit - rem) / digit);
	}
	else
	{
		minTick = tick * (digit + rem) / digit;		
	}
	minDigit = min - minTick * digit / tick;	
}
#pragma warning(disable : 4996)
int Chart::GetCountDigit(double min, double max, double &height, Font &font, int &len)
{
	StringFormat format;
	PointF origin;
	RectF rect;
	char buf[32];
	wchar_t wbuf[32];
	gcvt(min, 3, buf);
	len = strlen(buf);
	mbstowcs(wbuf, buf, 32);
	g->MeasureString(wbuf, wcslen(wbuf), &font, origin, &format, &rect);
	int mn = (int)rect.Width; 
	gcvt(max, 3, buf);
	int lenMax = strlen(buf);
	mbstowcs(wbuf, buf, 32);
	g->MeasureString(wbuf, wcslen(wbuf), &font, origin, &format, &rect);
	int mx = (int)rect.Width; 
	height = rect.Height;
	if(len < lenMax) len = lenMax;
	return mx > mn ? mx : mn;
}
void Chart::Draw(Gdiplus::Graphics &graph)
{
	g = &graph;
	graph.FillRectangle(&SolidBrush(Color(0xffaaaaaa)), rect.left, rect.top, rect.right, rect.bottom);
}
//------------------------------------------------------------------------------
LeftAxes::LeftAxes(Chart &chart) 
 : chart(chart)
// , minA(100)
// , maxA(100)
{}
//-----------------------------------------------------------------------------------
void LeftAxes::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	double height;
	
	int maxLen = 0;
	chart.offsetAxesLeft = 7 + chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font, maxLen);
	maxLen += 2;
	int x = chart.rect.left + chart.offsetAxesLeft;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
	char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	OffsetAxes(
		int(height * 2)
		, chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop
		, chart.minAxesY
		, chart.maxAxesY
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;
	chart.deltaTickY = deltaTick;
	chart.deltaDigitY = deltaDigit;
	while(bottom < offs)
	{
		offs -= deltaTick;
		digit += deltaDigit;
	}
	origin.X = (REAL)chart.rect.left - 5;
	int len;
	while(offs > chart.rect.top + chart.offsetAxesTop)
	{
		chart.g->DrawLine(&pen, (REAL)x - 5, (REAL)offs, (REAL)x, (REAL)offs);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		len = wcslen(wbuf);
		if(len <= maxLen)
		{
			chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
			origin.Y = REAL(offs - deltaTick + deltaTick/2);
			chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		}
		offs -= deltaTick;
		digit += deltaDigit;
	}	
}
//----------------------------------------------------------------------------------------------
BottomAxes::BottomAxes(Chart &chart)
: chart(chart) 
, minA(100)
, maxA(100)
{
}
void BottomAxes::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	double height;	
    int y = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, chart.rect.left + chart.offsetAxesLeft, y, chart.rect.right - chart.offsetAxesRight, y);

    char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
    double length = chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
    double deltaTick = 0;
	double deltaDigit = 0;
    double digit = 0;
    double minTick = 0;
    int maxLen = 0;
	OffsetAxes(
		int(2.5 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, chart.minAxesX
		, chart.maxAxesX
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
    chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;
	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;
    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 5);
    while(offs < offsMax)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
		origin.X = (REAL)offs;
		chart.g->DrawString(wbuf, len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	maxA = digit;
}
//-----------------------------------------------------------------------------------------------------------------
void BottomAxes::OffsetToPixel(WORD &offs, int delta)
{
	int offsX = offs;
	double t = offs - offsMin - deltaTickDigit * delta;
	t /= deltaTickDigit;
	double tt = (int)(t + 0.5);

	tt *= deltaTickDigit;
	offs = (WORD)tt + offsMin;
	if(offsMin >= offs){offs = offsMin + 3; return;}
	if(offsMax <= offs){offs = offsMax - 3; return;}
	if(offsX == offs) ++offs;
}
//-----------------------------------------------------------------------------------------------------------------
BottomAxesMeters::BottomAxesMeters(Chart &chart)
: chart(chart) 
, minA(100)
, maxA(100)
{
}
void BottomAxesMeters::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	double height;	
    int y = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, chart.rect.left + chart.offsetAxesLeft, y, chart.rect.right - chart.offsetAxesRight, y);

    char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
    double length = chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
    double deltaTick = 0;
	double deltaDigit = 0;
    double digit = 0;
    double minTick = 0;
    int maxLen = 0;
	OffsetAxes(
		int(3.5 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, minBorder
		, maxBorder
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
    chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;
	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;
    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 5);
    while(offs < offsMax)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
		origin.X = (REAL)offs;
		chart.g->DrawString(wbuf, len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	maxA = digit;
}
//-----------------------------------------------------------------------------------------------------------------
void BottomAxesMeters::OffsetToPixel(WORD &offs, int delta)
{
	double deltaTickDigit_ = deltaTickDigit / zone_length;
	int offsX = offs;
	double t = offs - offsMin - deltaTickDigit_ * delta;
	t /= deltaTickDigit_;
	double tt = (int)(t + 0.5);

	tt *= deltaTickDigit_;
	offs = (WORD)tt + offsMin;
	if(offsMin >= offs){offs = offsMin + 3; return;}
	if(offsMax <= offs){offs = offsMax - 3; return;}
	if(offsX == offs) ++offs;
}
//-----------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
BottomAxesInt::BottomAxesInt(Chart &chart)
: chart(chart) 
, minA(100)
, maxA(100)
{
}
void BottomAxesInt::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	double height;	
    int y = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, chart.rect.left + chart.offsetAxesLeft, y, chart.rect.right - chart.offsetAxesRight, y);

    char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
    double length = chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
    double deltaTick = 0;
	double deltaDigit = 0;
    double digit = 0;
    double minTick = 0;
    int maxLen = 0;
	OffsetAxes(
		int(2.0 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, chart.minAxesX
		, chart.maxAxesX
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
	if(deltaDigit < 1)
	{
		deltaTick /= deltaDigit;
		deltaDigit = 1;
	}
	chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;

	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;

    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 5);
    while(offs < chart.rect.right - chart.offsetAxesRight)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
		origin.X = (REAL)offs;
		chart.g->DrawString(wbuf, len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	maxA = digit;
}
//----------------------------------------------------------------------------------------------------------------
void BottomAxesInt::OffsetToPixel(WORD &offs, int delta)
{
	int offsX = offs;
	double t = offs - offsMin - deltaTickDigit * delta;
	t /= deltaTickDigit;
	double tt = (int)(t + 0.5);

	tt *= deltaTickDigit;
	offs = (WORD)tt + offsMin;
	if(offsMin >= offs){offs = offsMin + 3; return;}
	if(offsMax <= offs){offs = offsMax - 3; return;}
	if(offsX == offs) ++offs;
}
//----------------------------------------------------------------------------------------------------------------
void Grid::Draw()
{
	Color color(chart.colorGrid);
	Pen pen(color, 1);
    double offs = chart.offsetGridX;
    int y_b = chart.rect.bottom - chart.offsetAxesBottom - 3;
    int y_t = chart.rect.top + chart.offsetAxesTop + 3;
    while(offs < chart.rect.right - chart.offsetAxesRight - *deltaY * 0.2)
    {
		chart.g->DrawLine(&pen, (int)offs, y_b, (int)offs, y_t);
        offs += chart.deltaTickX;
    }
    offs = chart.offsetGridY;
    y_b = chart.rect.left + chart.offsetAxesLeft + 3;
    y_t = chart.rect.right - chart.offsetAxesRight - 3;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	while(bottom <= offs) offs -= *deltaY;
    while(offs > chart.rect.top + chart.offsetAxesTop + *deltaY * 0.2)
    {
		chart.g->DrawLine(&pen, y_b, (int)offs, y_t, (int)offs);
        offs -= *deltaY;
    }
}
//---------------------------------------------------------------------------------------------------------------------
LineSeries::LineSeries(Chart &chart) : chart(chart), data(NULL), color(0xff0000ff) {}
//----------------------------------------------------------------------------------------------------------------------
void LineSeries::Draw()
{
  if(NULL != data)
  {
    Color col(color);
	Pen pen(col, 2);
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );
    double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);

    double y = chart.rect.bottom - chart.offsetAxesBottom;
   
	int width = chart.rect.right - chart.rect.left - chart.offsetAxesRight - chart.offsetAxesLeft;
	double dX = (double)(width) / count;
	int x0 = chart.rect.left + chart.offsetAxesLeft;
	Point points[2000];
	points[0].X = x0;
	points[0].Y = int(y - data[0] * dY);
	double x = 0;
	int last = 0;
	for(int i = 1; i < count; ++i)
	{
		if(dimention_of(points) < last) break;
		x += dX;
		int k = i;//(int)x;
		points[k].X = int(x + x0);
		if(last == k)
		{
			int tmp = int(y - data[i] * dY);
			if(points[k].Y < tmp) points[k].Y = tmp;
		}
		else
		{
			points[k].Y =  int(y - data[i] * dY);
		}
		last = k;
	}
	chart.g->DrawLines(&pen, points, count);
	chart.g->SetClip(&Region());
  }
}
//-------------------------------------------------------------------------------------------------------------------
void LineSeries::SetData(double *d, int c, double min, double max)
{
	data = d;
	count = c;
	chart.minScaleX = min;
	chart.maxScaleX = max;
	chart.dScaleX = (max - min) / count;
}
//-------------------------------------------------------------------------------------------------------------
Cursor::Cursor(Chart &chart) 
: chart(chart)
, oMove(NULL), ptrMove(NULL)
{}	

bool Cursor::VerticalCursor(TMouseMove &l, VGraphics &g)
{
	double xx, xy;
	if(chart.AxesValues(l.x, l.y, xx, xy)	)
	{		
		Color col(0xffffffff);
		Pen pen(col, 1);	
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
      
        if(oMove&&ptrMove)(oMove->*ptrMove)(l, g);

		g().DrawLine(&pen, l.x, top, l.x, bottom);
		return true;
	}
	return false;
}
void Cursor::VerticalCursorNoTest(TMouseMove &l, VGraphics &g)
{
	Color col(0xffffffff);
	Pen pen(col, 1);	
	int top = chart.rect.top + chart.offsetAxesTop + 3;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;

	int left = chart.rect.left + chart.offsetAxesLeft;
	int right = chart.rect.right - chart.offsetAxesRight;

	if(l.x < left) l.x = right;
	else if(l.x > right) l.x = left;

	if(oMove&&ptrMove)(oMove->*ptrMove)(l, g);

	g().DrawLine(&pen, l.x, top, l.x, bottom);
}
bool Cursor::CrossCursor(TMouseMove &l, VGraphics &g)
{
	double xx, xy;
	if(chart.AxesValues(l.x, l.y, xx, xy)	)
	{		
		Color col(0xffffffff);
		Pen pen(col, 1);	
		int left = chart.rect.left + chart.offsetAxesLeft + 3;
		int right = chart.rect.right - chart.offsetAxesRight - 3;
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
      
        if(oMove&&ptrMove)(oMove->*ptrMove)(l, g);

		g().DrawLine(&pen, l.x, top, l.x, bottom);
		g().DrawLine(&pen, left, l.y, right, l.y);
		return true;
	}
	return false;
}

void Cursor::Draw()
{
	int xx, xy;
	if(chart.ValuesAxes(x0, y0, xx, xy))
	{
        Color col(0xff0000ff);
		Pen pen(col, 1);	
		int left = chart.rect.left + chart.offsetAxesLeft + 3;
		int right = chart.rect.right - chart.offsetAxesRight - 3;
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
		chart.g->DrawLine(&pen, xx, top, xx, bottom);
		chart.g->DrawLine(&pen, left, xy, right, xy);
	}
	if(chart.ValuesAxes(x1, y1, xx, xy))
	{
        Color col(0xffff0000);
		Pen pen(col, 1);	
		int left = chart.rect.left + chart.offsetAxesLeft + 3;
		int right = chart.rect.right - chart.offsetAxesRight - 3;
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
		chart.g->DrawLine(&pen, xx, top, xx, bottom);
		chart.g->DrawLine(&pen, left, xy, right, xy);
	}
}
//-------------------------------------------------------------------------------------------
bool Chart::AxesValues(int x, int y, double &dx, double &dy)
{	
	if(
         x > rect.left + offsetAxesLeft
		 && x < rect.right - offsetAxesRight
		 && y > rect.top + offsetAxesTop
		 && y < rect.bottom - offsetAxesBottom
		)
	{
		double dY = (maxAxesY - minAxesY) / (rect.bottom - rect.top - offsetAxesBottom - offsetAxesTop);
		dy = (rect.bottom - offsetAxesBottom - y) * dY + minAxesY;
		dy = floor(dy);
		double dX = (maxAxesX - minAxesX) / (rect.right - rect.left - offsetAxesLeft - offsetAxesRight);
		dx = (x - rect.left - offsetAxesLeft) * dX + minAxesX;
		return true;
	}
	return false;
}
int Chart::BetweenLeftRight(int x)
{
	return  x < rect.left + offsetAxesLeft ? -1 : x > rect.right - offsetAxesRight ? 1 : 0;
}
 bool Chart::ValuesAxes(double dx, double dy, int &x, int &y)
 {
	 dx -= minAxesX;
     double dX = (maxAxesX - minAxesX) / (rect.right - rect.left - offsetAxesLeft - offsetAxesRight);
	 dx /= dX;
	 x = int(dx + rect.left + offsetAxesLeft);
	 dy -= minAxesY;
     double dY = (maxAxesY - minAxesY) / (rect.bottom - rect.top - offsetAxesBottom - offsetAxesTop);
	 dy /= dY;
	 y = int(rect.bottom - offsetAxesBottom - dy);

     return  x > rect.left + offsetAxesLeft && x < rect.right - offsetAxesRight 
		  && y > rect.top + offsetAxesTop && y < rect.bottom - offsetAxesBottom;
 }
 //----------------------------------------------------------------------------------------------
 void Chart::Restore(HWND hwnd)
 {
	 maxAxesY = maxScaleY;
	 minAxesY = minScaleY;

	 maxAxesX = maxScaleX;
	 minAxesX = minScaleX;
	 Graphics g(backScreen);
	 Draw(g);
	 InvalidateRect(hwnd, NULL, true);
 }
 //---------------------------------------------------------------------------------------------
 void FrameDraw(Gdiplus::Graphics &g, RECT &r)
 {
	 /*
	 Pen penUp(-1, 1);
	 g.DrawLine(&penUp, r.left, r.top, r.right, r.top);
	 g.DrawLine(&penUp, r.left, r.top + 1, r.left, r.bottom - 1);

	 Pen penDown(0xff000000, 1);
	 g.DrawLine(&penDown, r.left, r.bottom - 1, r.right, r.bottom - 1);
	 g.DrawLine(&penDown, r.right - 1, r.top + 1, r.right - 1, r.bottom - 1);
	 */
 }
 //-----------------------------------------------------------------------------------------------
 /*
 bool Cursor::MouseMoveScale(TMouseMove &l, VGraphics &g)
 {
	 double xx, yy;
	 if(mouseDown && chart.AxesValues(l.x, l.y, xx, yy))
	 {
		Color col(0xffffffff);
		Pen pen(col, 1);		
      
		if( l.x > dxScaleDown)
		{
			if(l.y > dyScaleDown)g().DrawRectangle(&pen, dxScaleDown, dyScaleDown, l.x - dxScaleDown, l.y - dyScaleDown);
			else             g().DrawRectangle(&pen, dxScaleDown, l.y    , l.x - dxScaleDown, dyScaleDown - l.y);
		}
		else
		{
			if(l.y > dyScaleDown)g().DrawRectangle(&pen, l.x, dyScaleDown, dxScaleDown - l.x, l.y - dyScaleDown);
			else 
			{
				g().DrawRectangle(&pen, l.x, l.y, dxScaleDown - l.x, dyScaleDown - l.y);
			}
		}
		return true;
	 }
	 return false;
 }
 */
 /*
 bool Cursor::LButtonDownScale(TLButtonDown &l)
 {	 
	 double xx, yy;
	 if(chart.AxesValues(l.x, l.y, xx, yy))
	 {
		 dxScaleDown = l.x;
		 dyScaleDown = l.y;
		 mouseDown = true;
	 }
	 return false;
 }
 bool Cursor::LButtonUpScale(TLButtonUp &l)
 {	
	 dxScaleUp = l.x;
	 dyScaleUp = l.y;
	 mouseDown = false;
	 double x0, y0, x1, y1;
	 if(chart.AxesValues(dxScaleDown, dyScaleDown, x0, y0) && chart.AxesValues(dxScaleUp, dyScaleUp, x1, y1))
	 {
		 bool ok = false;
		 if(dxScaleDown < dxScaleUp && dyScaleDown < dyScaleUp)
		 {
			 if((x1 - x0 < 2) || (y0 - y1 < 2)) return false;
			 chart.minAxesX = x0;
			 chart.maxAxesX = x1;
			 chart.minAxesY = y1;
			 chart.maxAxesY = y0;
			 ok = true;
		 }
		 else if(dxScaleDown > dxScaleUp && dyScaleDown > dyScaleUp)
		 {
			 if(abs(x0) > 10e8 || abs(x1) > 10e8 || abs(y0) > 10e8 || abs(y1) > 10e8) return false;
			  double dy = y1 - y0;
			  double minMaxY = chart.maxAxesY - chart.minAxesY;
			  double yy = minMaxY / dy;

			  double minY = y0 - chart.minAxesY;			  
              chart.minAxesY -= minY * yy;
			  double maxY = chart.maxAxesY - y1;
              chart.maxAxesY += maxY * yy;

			  double dx = x0 - x1;
			  double minMaxX = chart.maxAxesX - chart.minAxesX;
			  double xx = minMaxX / dx;

			  double minX = x1 - chart.minAxesX; 
			  chart.minAxesX -= minX *xx;
			  chart.minAxesX = floor(chart.minAxesX / chart.dScaleX + 0.5) * chart.dScaleX;
			  double maxX = chart.maxAxesX - x0;
			  chart.maxAxesX += maxX *xx;
			  chart.maxAxesX = floor(chart.maxAxesX / chart.dScaleX + 0.5) * chart.dScaleX;
			  ok = true;
		 }
		 if(ok)
		 {
			 Graphics g(chart.backScreen);
			 chart.Draw(g);
			 InvalidateRect(l.hwnd, NULL, true);
			 dxScaleDown = dxScaleUp = 0;
			 return true;
		 }
	 }
	 return false;
 }
 */
 //------------------------------------------------------------------------------------------------------------
 /*
 bool Cursor::MouseMoveMove(TMouseMove &l, VGraphics &g)
 {   
	 if(l.flags & MK_LBUTTON)
	 {
		 double x0, y0, x1, y1;
		 if(chart.AxesValues(dxScaleDown, dyScaleDown, x0, y0)&& chart.AxesValues(l.x, l.y, x1, y1))
		 {
			 double dx = x0 - x1;
			 double dy = y0 - y1;
			 chart.minAxesX += dx;
			 chart.maxAxesX += dx;
			 chart.minAxesX = floor(chart.minAxesX / chart.dScaleX + 0.5) * chart.dScaleX;
			 chart.maxAxesX = floor(chart.maxAxesX / chart.dScaleX + 0.5) * chart.dScaleX;
			 chart.minAxesY += dy;
			 chart.maxAxesY += dy;
			 dxScaleDown = l.x;
			 dyScaleDown = l.y;
			 chart.Draw(g());
			 InvalidateRect(l.hwnd, NULL, true);
			 return true;
		 }
	 }
	 return false;
 }
 */
 /*
 bool Cursor::LButtonDownMove(TLButtonDown &l)
 {
	 dxScaleDown = l.x; dyScaleDown = l.y;
	 return false;
 }
 bool Cursor::LButtonUpMove(TLButtonUp &)
 {
	 dxScaleDown = 0;
	 return false;
 }
 */
 //-------------------------------------------------------------------------------------------------
/*
TBrush::TBrush(): count(0){}

SolidBrush &TBrush::Color(unsigned c)
{
	int i = 0;
	for(; i < count; ++i)
	{
		if(arr[i].color == c) return arr[i].brush;
	}
	if(i >= dimention_of(arr)) return arr[0].brush;
	arr[i].color = c;
	arr[i].brush.SetColor(c);
	++count;
	return arr[i].brush;
}
*/
 
