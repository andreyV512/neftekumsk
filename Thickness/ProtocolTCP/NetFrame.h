#pragma once

struct Frame;
struct NetLink
{
	static void Send(Frame *);
	static void Recv(Frame *);
};