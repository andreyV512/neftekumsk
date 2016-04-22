#pragma once
#include "typelist.hpp"
#include "message.h"

struct VGraphics
{
  virtual Gdiplus::Graphics &operator()() = 0;
  virtual ~VGraphics(){};
};
struct HDCGraphics:VGraphics
{
	HWND h;
	Gdiplus::Bitmap *bitmap;
	HDC hdc;
	Gdiplus::Graphics graphics;
	HDCGraphics(HWND h, Gdiplus::Bitmap *bitmap) 
		: h(h)
		, bitmap(bitmap)
		, hdc(GetDC(h))
		, graphics(hdc)
	{
		graphics.DrawCachedBitmap(&Gdiplus::CachedBitmap(bitmap, &graphics), 0, 0);
	} 
	~HDCGraphics(){ReleaseDC(h, hdc);}
	Gdiplus::Graphics &operator()(){return graphics;}
};
struct PaintGraphics:VGraphics
{
	Gdiplus::Graphics &graphics;
	explicit PaintGraphics(Gdiplus::Graphics &g) 
		:graphics(g)
	{}
	Gdiplus::Graphics &operator()(){return graphics;}
};

inline bool InRect(int x, int y, RECT &r)
{
	return r.left < x && r.right > x && r.top < y && r.bottom > y;
}

class Chart
{
public:
  RECT rect;
  int offsetAxesLeft, offsetAxesBottom, offsetAxesTop, offsetAxesRight;
  double minAxesX, maxAxesX, minAxesY, maxAxesY, minAxesYRight, maxAxesYRight;
  double minScaleX, maxScaleX, dScaleX, minScaleY, maxScaleY;
  int colorAxes;
  int colorFontAxes;
  int fontHeight;
  int colorGrid;
  double offsetGridX, offsetGridY;
  double deltaTickX, deltaTickY, deltaDigitX, deltaTickYRight, deltaDigitY, deltaDigitYRight;
  Gdiplus::Bitmap *&backScreen;
  Gdiplus::Graphics *g;
  Chart(Gdiplus::Bitmap *&backScreen);
  ~Chart();
  int GetCountDigit(double , double, double &, Gdiplus::Font &, int &); 
  bool AxesValues(int, int, double &, double &);
  bool ValuesAxes(double, double, int &, int &);
  virtual void Draw(Gdiplus::Graphics &graph);
  void Restore(HWND);
  int BetweenLeftRight(int); //-1 0 1
};
class LeftAxes
{
public:
  Chart &chart;
  LeftAxes(Chart &chart);
  void Draw();
};
class BottomAxes
{
	double minA, maxA;
	int offsMax;
	int offsMin;
	double deltaTickDigit;
public:
  Chart &chart;
  BottomAxes(Chart &chart);
  void Draw();
  void OffsetToPixel(WORD &, int);
};

class BottomAxesMeters
{
	double minA, maxA;
	int offsMax;
	int offsMin;
	double deltaTickDigit;
public:
  Chart &chart;
  double minBorder, maxBorder;
  BottomAxesMeters(Chart &chart);
  void Draw();
  void OffsetToPixel(WORD &, int);
};

class BottomAxesInt
{
	double minA, maxA;
	int offsMax;
	int offsMin;
	double deltaTickDigit;
public:
  Chart &chart;
  BottomAxesInt(Chart &chart);
  void Draw();
  void BottomAxesInt::OffsetToPixel(WORD &, int );
};

class Grid
{
public:
  Chart &chart;
  double *deltaY;
  Grid(Chart &chart) : chart(chart), deltaY(&chart.deltaTickY) {}
  void Draw();
  void DrawSample(Chart &, int);
};
class LineSeries
{
public:
	Chart &chart;
	int count;
	double *data;
	LineSeries(Chart &chart);// : chart(chart), data(NULL) {}
	int color;
	void Draw();
	void SetData(double *data, int countSamples, double minValue, double maxValue);
};
class Cursor
{
	struct TObj{};
	typedef bool(TObj::*TPtrMove)(TMouseMove &, VGraphics &);	
	TObj *oMove;
	TPtrMove ptrMove;	
public:
   double x0, y0, x1, y1;
   Chart &chart;
   Cursor(Chart &chart);
   void Draw();

   bool VerticalCursor(TMouseMove &, VGraphics &);
   bool CrossCursor(TMouseMove &, VGraphics &);
   void VerticalCursorNoTest(TMouseMove &, VGraphics &);
   template<class T>void SetMouseMoveHandler(T *t, bool(T::*to)(TMouseMove &, VGraphics &))
   {
	   oMove = (TObj *)t;
	   ptrMove = (TPtrMove)to;
   }   
};
void FrameDraw(Gdiplus::Graphics &, RECT &);
//--------------------для внутреннего использования
void OffsetAxes(int , int , double , double , double &, double &, double &, double &);




