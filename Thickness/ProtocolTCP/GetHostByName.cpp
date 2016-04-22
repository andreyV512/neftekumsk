#include "stdafx.h"
#include "GetHostByName.h"
#include "DebugMess.h"

//gethostbyname
/*
SOCKET ListenSocket;
struct sockaddr_in saServer;
hostent* localHost;
char* localIP;

// Create a listening socket
ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Get the local host information
localHost = gethostbyname("");
localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);

// Set up the sockaddr structure
saServer.sin_family = AF_INET;
saServer.sin_addr.s_addr = inet_addr(localIP);
saServer.sin_port = htons(5150);

// Bind the listening socket using the
// information in the sockaddr structure
bind( ListenSocket,(SOCKADDR*) &saServer, sizeof(saServer) );
*/

wchar_t *GetHostByName::operator()(wchar_t *name)
{
	char host_name[128];
	size_t count;
	wcstombs_s(&count, host_name, name, sizeof(host_name));
	struct hostent *remoteHost  = gethostbyname(host_name);	
	in_addr  addr;
	addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
	wsprintf(buf, L"%d.%d.%d.%d"
		, addr.S_un.S_un_b.s_b1
		, addr.S_un.S_un_b.s_b2
		, addr.S_un.S_un_b.s_b3
		, addr.S_un.S_un_b.s_b4
		);
	return buf;
} 