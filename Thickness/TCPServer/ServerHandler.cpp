#include "stdafx.h"
#include "ServerHandler.h"
#include "Frame.h"
#include "NetFrame.h"
#include "IPprotocolProcedureList.hpp"

namespace 
{
template<class List>struct Handlers;
template<class Head, class Tail>struct Handlers<Tlst<Head, Tail> > : Handlers<Tail>
{
	const Server::Proc proc;
	Handlers() 
		: Handlers<Tail>()
		, proc(Head::RecvCallBack)
	{}
};
template<class Head>struct Handlers<Tlst<Head, NullType> >
{
	const Server::Proc proc;
	Handlers() : proc(Head::RecvCallBack){}
};
static Handlers<TL::Reverse<__ip_protocol_procedure_list__>::Result> handlers;
}

Server::Proc Server::Handler(unsigned indexProc)
{
	return indexProc < TL::Length<__ip_protocol_procedure_list__>::value
		? ((Server::Proc *)&handlers)[indexProc]
	: NULL;
}