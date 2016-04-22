#pragma once
//#include <GdiPlus.h>
//#include <GdiPlusBitmap.h>
class ColorLabel
{		
public:  
	int fontHeight;
	int left;
	int top;
	unsigned backColor;
	wchar_t buffer[512];	
	Gdiplus::RectF rect;
	ColorLabel(): fontHeight(7), left(30), top(5), backColor(0xffaaaaaa){}
	wchar_t *operator=(wchar_t *);
	wchar_t *operator=(char *);
	void Draw(Gdiplus::Graphics &);	
};