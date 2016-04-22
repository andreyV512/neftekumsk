#pragma once
 struct Server
 {
	 typedef void (CALLBACK *Proc)(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
	 static Proc Handler(unsigned indexProc);
 };
