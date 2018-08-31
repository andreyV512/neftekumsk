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

STATUS_LABEL(LessMinimumEnergy, "������� ������� ������ ���������� ��������")
STATUS_LABEL(GreaterMaximumEnergy, "������� ������� ������ ���������� ��������")
STATUS_LABEL(ValueLessThreshold, "������ ������ ���������� ��������")
STATUS_LABEL(ThicknessLessThreshold, "����������� ������� ������ ������������ ��������")
STATUS_LABEL(Undefined, "��������� �� ��������")
STATUS_LABEL(DeathZone, "̸����� ����")

STATUS_LABEL(Nominal, "<ff00>������� � �������")
STATUS_LABEL(Defect, "<ff0000>����")
STATUS_LABEL(Treshold2Class, "<ff>����� 2")
STATUS_LABEL(Treshold3Class, "<ff>����� 3")
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
COLOR_DATA(Undefined)
COLOR_DATA(DeathZone)


COLOR(Nominal)
COLOR(Defect)
COLOR(Treshold2Class)
COLOR(Treshold3Class)

#undef COLOR
#undef COLOR_DATA
//------------------------------------------------------------------------------------------------
