#pragma once

#include <winsock2.h>
struct Frame: OVERLAPPED
{
	Frame *next;
	SOCKET socket;
	void (CALLBACK *proc)(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
private:
	struct Additional
	{
		char data[16];
	} additional;
public:
	int length;
	struct Data
	{
		unsigned id;
		char buf[1024];
	} data;
	static void Drop(void *);
	template<class T>static T *AdditionalData(Frame* f)
	{
	   sizeof(size_of_type_Additional_must_be_greater_than_or_equal_to_the_type_of_T<sizeof(Additional) >= sizeof(T), sizeof(T)>);
	   return (T *)&f->additional;
	}
	static Frame *Get()
	{
		return _Get();
	}
private:
	static Frame *_Get();
	template<bool, int>struct size_of_type_Additional_must_be_greater_than_or_equal_to_the_type_of_T;
	template<int N>struct size_of_type_Additional_must_be_greater_than_or_equal_to_the_type_of_T<true, N>{};
	Frame();
	Frame(const Frame &);
	const Frame &operator=(const Frame &);
};