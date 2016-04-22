#pragma once
#include "DebugMess.h"
#ifdef XDEBUG
#define dprint debug.print
#else
#define dprint
#endif

template<class T>struct List
{
	struct Item
	{
		Item *pred, *next;
		T data;
	};
	 Item *items;
	 Item *droped;
	void AddToDrop(int n)
	{
		for(int i = 0; i < n; ++i)
		{
			Item *x = new Item;
			x->next = droped;
			droped = x;
		}
	}
	void DropList(Item *x)
	{
		while(x)
		{
			Item *z = x->next;
			delete x;
			x = z;
		}
	}
	List()
		: items(NULL)
	    , droped(NULL)
	{
		AddToDrop(5);
	}
	~List()
	{
		DropList(items);
		DropList(droped);
	}
	void Add(T &d)
	{
		Item *x = droped;
	//	dprint("xx %x", x);
		if(NULL == x)
		{
			AddToDrop(5);
			x = droped;
		}
	//	dprint("x %x", x);
		droped = x->next;
	//	dprint("droped %x", droped);
		x->data = d;
		x->pred = NULL;
		x->next = items;
	//    if(NULL != items) items->pred = x;
		items = x;
	}
	void Drop(Item *d)
	{
		/*
		Item *x = items;
		while(NULL != x)
		{
			if(x == d)
			{
				if(NULL == x->pred)
				{
					items = x->next;
					if(items)items->pred = NULL;
					x->next = droped;
					droped = x;
					return;
				}
				if(NULL == x->next)
				{
					x->pred->next = NULL;
					x->next = droped;
					droped = x;
					return;
				}
				x->pred->next = x->next;
				x->next->pred = x->pred;
				x->next = droped;
				droped = x;
				return;
			}
			x = x->next;
		}
		*/
	//	dprint("dddd %x", d);
		if(NULL == d->pred)
		{
			Item *tmp = d->next; 
			if(NULL != tmp)
			{
				tmp->pred = NULL;
			}
			items = tmp;
		}
		else if(NULL == d->next)
		{
			Item *tmp = d->pred; 
			if(NULL != tmp)
			{
				tmp->next = NULL;
			}
		}
		else
		{
			d->pred->next = d->next;
			d->next->pred = d->pred;
	    }
		d->next = droped;
		droped = d;
	//	dprint("droped = d; %x", droped);
	}	
};