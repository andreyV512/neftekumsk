#pragma once
#ifdef DEBUG_ITEMS
class EmulLan
{
public:
	void Init(){}
	void Destroy();
	void Start(double &);
	void Stop();
	bool Ready();
	void GetBuf(char *);
};

extern EmulLan emulLan;
#endif