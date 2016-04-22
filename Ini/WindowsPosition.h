#pragma  once

class WindowPosition
{
static void Get_(const char *, RECT &);
static void Set_(const char *, RECT &);
public:
	template<class T>static void Get(RECT &r)
	{
		Get_(typeid(T).name(), r);
	}
	template<class T>static void Set(RECT &r)
	{
		Set_(typeid(T).name(), r);
	}
};

template<class T>class StoredWindowPosition
{
public:
	static void Do(HWND h)
	{
		RECT r;
		GetWindowRect(h, &r);
		if(r.left < 0) r.left = 0;
		if(r.top < 0) r.top = 0;
		r.right -= r.left;
		r.bottom -= r.top;
		WindowPosition::Set<T>(r);
	}
};