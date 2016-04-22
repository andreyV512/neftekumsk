#pragma once
#include "AppBase.h"

class SQEvents
{
public:
public:
	unsigned input;
	unsigned last;
	unsigned msk;
public:
	template<class T>struct Event;
	template<class T>struct On{};
	template<class T>struct Off{};

///\brief если поднимается бит - срабатывает событие с соответствующем именем 
#define EVENT_ON(name)template<>struct Event<On<name> >\
	{\
	typedef name inner_type;\
	HANDLE hEvent;\
	HANDLE &h;\
	unsigned &bit;\
	template<class Z>Event(Z &z)\
		: h(z.items.get<Event<Off<name> > >().hEvent)\
		, bit(Singleton<InputBitsTable>::Instance().items.get<name>().value)\
		{}\
	wchar_t *Name(){return L#name##L"ON";}\
	void Do(){SetEvent(hEvent); ResetEvent(h);}\
	};

///\brief если падает бит - срабатывает событие с соответствующем именем 
#define EVENT_OFF(name)template<>struct Event<Off<name> >\
	{\
	HANDLE hEvent;\
	HANDLE &h;\
	unsigned &bit;\
	template<class Z>Event(Z &z)\
		: h(z.items.get<Event<On<name> > >().hEvent)\
		, bit(Singleton<InputBitsTable>::Instance().items.get<name>().value)\
		{}\
	wchar_t *Name(){return L#name##L"OFF";}\
	void Do(){SetEvent(hEvent);ResetEvent(h);}\
	};

#define EVENT_(name) EVENT_ON(name) EVENT_OFF(name)
	
	EVENT_(SQ1BitIn           )
	EVENT_(SQ3BitIn           )
	EVENT_(StateBBitIn        )
	EVENT_(StateABitIn        )
	EVENT_(RunBitIn           )
	EVENT_(ControlCircuitBitIn)
	EVENT_(WorkBitIn          )
	EVENT_(SycleBitIn         )
	EVENT_(BaseBitIn          )

#undef EVENT_ON
#undef EVENT_OFF

#undef EVENT_
#define USER_EVENT(name)struct name{};\
		template<>struct Event<name>\
		{\
		HANDLE hEvent;\
		template<class Z>Event(Z &){}\
		wchar_t *Name(){return L#name;}\
		};
		USER_EVENT(AutomaticMode)
		USER_EVENT(SingleMode)
		USER_EVENT(AlarmExit)
		USER_EVENT(UserStop)
#undef USER_EVENT
	typedef TL::MkTlst<
		  Event<On<SQ1BitIn            > >
		, Event<On<SQ3BitIn            > >
		, Event<On<StateBBitIn         > >
		, Event<On<StateABitIn         > >
		, Event<On<RunBitIn            > >
		, Event<On<ControlCircuitBitIn > >
		, Event<On<WorkBitIn           > >
		, Event<On<SycleBitIn          > >
		, Event<On<BaseBitIn           > >

		, Event<Off<SQ1BitIn            > >
		, Event<Off<SQ3BitIn            > >
		, Event<Off<StateBBitIn         > >
		, Event<Off<StateABitIn         > >
		, Event<Off<RunBitIn            > >
		, Event<Off<ControlCircuitBitIn > >
		, Event<Off<WorkBitIn           > >
		, Event<Off<SycleBitIn          > >
		, Event<Off<BaseBitIn           > >

		, Event<AutomaticMode    > 
		, Event<SingleMode    > 
		, Event<AlarmExit>
		, Event<UserStop>
	>::Result type_items;
	TL::Factory<type_items> items;
	static DWORD WINAPI Proc(PVOID);
	void Do();
	template<class O, class P>struct __maska_bits__
	{
		void operator()(O *o, P *p)
		{
		   *p |= o->value;
		}
	};
	/*
	template<class O, class P>struct __all_bits__
	{
		bool operator()(O *o, P *p)
		{
			return 0 != (p & o->value);
		}
	};
	*/
public:
	SQEvents();
	void Init();
	void Destroy();
	void Reset();

	void SetBits(unsigned);
	void ClrBits(unsigned);

	//template<class T>bool Bit(){return 0 != (input & Singleton<InputsTable>::Instance().items.get<T>().value);}
	//template<class T>unsigned OffsBit(){return Singleton<InputsTable>::Instance().items.get<T>().value;}

	/**********************************************************************************************//**
	 * \fn	template<class List>void SQEvents::SetBits()
	 * \brief	устанавливает биты в дискретном выходном порту платы 1730
	 * \tparam	List	Type of the list.
	 **************************************************************************************************/
	template<class List>void SetBits()
	{
		unsigned t = 0;
		TL::foreach<List, __maska_bits__>()(&Singleton<OutputBitsTable>::Instance().items, &t);
		SetBits(t);
	}

	/**********************************************************************************************//**
	 * \fn	template<class List>void SQEvents::ClrBits()
	 * \brief	очищает биты в дискретном выходном порту платы 1730
	 * \tparam	List	Type of the list.
	 **************************************************************************************************/
	template<class List>void ClrBits()
	{
		unsigned t = 0;
		TL::foreach<List, __maska_bits__>()(&Singleton<OutputBitsTable>::Instance().items, &t);
		ClrBits(t);
	}

	/**********************************************************************************************//**
	 * \fn	template<class List>bool SQEvents::AllBits()
	 * \brief	возвращает true, если все из списка биты равны 1.
	 * \tparam	List	Type of the list.
	 **************************************************************************************************/
	template<class List>bool AllBits()
	{
		//return TL::foreach<List, __all_bits__>()(&Singleton<InputsTable>::Instance().items, &input);
		unsigned t = 0;
		TL::foreach<List, __maska_bits__>()(&Singleton<InputBitsTable>::Instance().items, &t);
		return t == (input & t);
	}

	/**********************************************************************************************//**
	 * \fn	template<class List>bool SQEvents::AnyBits()
	 * \brief	возвращает true, если один из списка бит равен 1.
	 * \tparam	List	Type of the list.
	 **************************************************************************************************/
	template<class List>bool AnyBits()
	{
		unsigned t = 0;
		TL::foreach<List, __maska_bits__>()(&Singleton<InputBitsTable>::Instance().items, &t);
		return 0 != (input & t);
	}
};

/** \brief	The set bits. */
#define SET_BITS(...) sQEvents.SetBits<TL::MkTlst<__VA_ARGS__>::Result>()
/** \brief	The colour bits. */
#define CLR_BITS(...) sQEvents.ClrBits<TL::MkTlst<__VA_ARGS__>::Result>()

/** \brief	all bits. */
#define ALL_BITS(...) sQEvents.AllBits<TL::MkTlst<__VA_ARGS__>::Result>()
/** \brief	any bits. */
#define ANY_BITS(...) sQEvents.AnyBits<TL::MkTlst<__VA_ARGS__>::Result>()

extern SQEvents sQEvents;