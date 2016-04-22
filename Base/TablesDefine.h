#pragma once

#define DEFINE_PARAM(z, type, def_val) struct z\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z;}\
	z() : value(def_val), default_value(def_val) {}\
};

template<class T>struct ID
{
	typedef typename T::__template_must_be_overridded__ noused;
};

#define DEFINE_PARAM_ID(z, type, def_val) template<> struct ID<z>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#z##L"ID";}\
	ID() : value(def_val), default_value(def_val) {}\
};

#define DEFINE_PARAM_NUM(z, n, type, def_val) template<>struct z<n>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L###z##L###n;}\
	z() : value(def_val), default_value(def_val) {}\
};

#define STR_PARAM(namePar, len, txt)struct namePar\
{\
	typedef Holder<len> type_value;\
	type_value value;\
	type_value default_value;\
	const wchar_t *name(){return L###namePar;}\
	namePar()\
	{\
		default_value = txt;\
		value = default_value;\
	}\
};

#define CHAR_PARAM(namePar, len, txt)struct namePar\
{\
	typedef CharHolder<len> type_value;\
	type_value value;\
	type_value default_value;\
	const wchar_t *name(){return L###namePar;}\
	namePar()\
    {\
	   default_value = txt;\
	   value = default_value;\
    }\
};

#define DEFINE_PARAM_WAPPER(wapper, z, type, def_val) template<> struct wapper<z>\
{\
	typedef type type_value;\
	type_value value;\
	const type_value default_value;\
	const wchar_t *name(){return L#wapper##L#z;}\
	wapper() : value(def_val), default_value(def_val) {}\
};