#pragma once

class GetHostByName
{
	wchar_t buf[64];
public:
	wchar_t *operator()(wchar_t *);
};