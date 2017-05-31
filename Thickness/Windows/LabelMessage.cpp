#include "stdafx.h"
#include "LabelMessage.h"
#include "PrimaryData.h"
#include "AppBase.h"
//-----------------------------------------------------------------
template<class O, class P>struct __status_label__
{
	typedef typename O::__template_must_be_overridded__ noused;
};
StatusLabel::StatusLabel(char status)
	: status(status)
	, text("")
{
	TL::find<PrimaryData::message_list, __status_label__>()((TL::Factory<PrimaryData::message_list> *)0, this);
}
#define STATUS_LABEL(n, txt)template<class P>struct __status_label__<PrimaryData::Mess<PrimaryData::n>, P>\
{\
	typedef PrimaryData::Mess<PrimaryData::n> O;\
	bool operator()(O *, P *p)\
    {\
        if(O::value == p->status)\
		{\
		     p->text = txt;\
             return false;\
		}\
		return true;\
    }\
};

STATUS_LABEL(LessMinimumEnergy, "Энергия сигнала меньше порогового значения")
STATUS_LABEL(GreaterMaximumEnergy, "Энергия сигнала больше порогового значения")
STATUS_LABEL(ValueLessThreshold, "Сигнал меньше порогового значения")
STATUS_LABEL(ThicknessLessThreshold, "Вычисленная толщина меньше минимального значения")
STATUS_LABEL(Undefined, "Результат не определён")
STATUS_LABEL(DeathZone, "Мёртвая зона")

STATUS_LABEL(Nominal, "<ff00>Толщина в допуске")
STATUS_LABEL(Defect, "<ff0000>Брак")
STATUS_LABEL(Treshold2Class, "<ff>Класс 2")
STATUS_LABEL(Treshold3Class, "<ff>Класс 3")
#undef STATUS_LABEL
//---------------------------------------------------------------------------
template<class O, class P>struct __set_color_bar__
{
	typedef typename O::__template_must_be_overridded__ noused;
};

ColorBar::ColorBar(double &data, unsigned &color, char status)
: data(data)
, color(color)
, status(status)
{
	TL::find<PrimaryData::message_list, __set_color_bar__>()((TL::Factory<PrimaryData::message_list> *)0, this);
}
#define COLOR(n)template<class P>struct __set_color_bar__<PrimaryData::Mess<PrimaryData::n>, P>\
{\
	typedef PrimaryData::Mess<PrimaryData::n> O;\
	int &color;\
	__set_color_bar__() : color(Singleton<ColorTable>::Instance().items.get<n>().value){}\
	bool operator()(O *, P *p)\
    {\
        if(O::value == p->status)\
		{\
		     p->color = color;\
             return false;\
		}\
		return true;\
    }\
};

#define COLOR_DATA(n)template<class P>struct __set_color_bar__<PrimaryData::Mess<PrimaryData::n>, P>\
{\
	typedef PrimaryData::Mess<PrimaryData::n> O;\
	int &color;\
	double &res;\
	__set_color_bar__() \
        : color(Singleton<ColorTable>::Instance().items.get<n>().value)\
		, res(Singleton<ThresholdsTable>::Instance().items.get<Border2Class>().value)\
        {}\
	bool operator()(O *, P *p)\
    {\
        if(O::value == p->status)\
		{\
		     p->color = color;\
			 p->data = res;\
             return false;\
		}\
		return true;\
    }\
};

COLOR_DATA(LessMinimumEnergy)
COLOR_DATA(GreaterMaximumEnergy)
COLOR_DATA(ValueLessThreshold)
COLOR_DATA(ThicknessLessThreshold)
//COLOR_DATA(ThicknessGreaterThreshold)
COLOR_DATA(Undefined)
COLOR_DATA(DeathZone)


COLOR(Nominal)
COLOR(Defect)
COLOR(Treshold2Class)
COLOR(Treshold3Class)
//COLOR(DefectMinMax)
//COLOR(DefectDifferentWall)
//
//COLOR(DefectDifferentWallMin)
//COLOR(DefectDifferentWallMax   )
//COLOR(DefectDifferentWallMinMax)

#undef COLOR
#undef COLOR_DATA
//------------------------------------------------------------------------------------------------
