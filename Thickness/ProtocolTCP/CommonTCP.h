#pragma once

class CommonClient
{
protected:
	static void CALLBACK Stop(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
	template<class T>static void CALLBACK Send(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if (0 == dwError && 0 != dwTransferred)
		{
			T *t = Frame::AdditionalData<T>(f);
			if(t->Do())
			{
				NetLink::Send(f);
				return;
			}
		}
		closesocket(f->socket);
		Frame::Drop(f);
	}
	template<class T>static void CALLBACK Recv(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if (0 == dwError && 0 != dwTransferred)
		{
			f->proc = Send<T>;
			NetLink::Recv(f);
		}
		else
		{
			closesocket(f->socket);
			Frame::Drop(f);
		}
	}
};

struct Frame;
template<class T>class CommonServer
{
protected:
public:
	static void CALLBACK Stop(DWORD, DWORD, LPWSAOVERLAPPED lpOverlapped, DWORD)
	{
		Frame *f = (Frame *)lpOverlapped;
		closesocket(f->socket);
		Frame::Drop(f);
	}
public:
	static void CALLBACK RecvCallBack(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if(0 == dwError &&  0 != dwTransferred && T::Do(f))
		{
			if(NULL == f->proc) f->proc = Send;
			NetLink::Send(f);
		}
		else
		{
			closesocket(f->socket);
			Frame::Drop(f);
		}
	}
	static void CALLBACK Send(DWORD dwError, DWORD dwTransferred, LPWSAOVERLAPPED lpOverlapped, DWORD dwFlags)
	{
		Frame *f = (Frame *)lpOverlapped;
		if (0 == dwError && 0 != dwTransferred)
		{
			f->proc = RecvCallBack;
			NetLink::Recv(f);
			return;
		}
		closesocket(f->socket);
		Frame::Drop(f);
	}
};