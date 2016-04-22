#pragma once

class MedianFiltre
{
//	double data[64];
//	int index[64];
	int length_;
	//int offset;
	int middle;
public:
	const int &length;
	MedianFiltre();
	void SetLength(int);
	double operator()(double *, int &);
	bool operator()(double *, char *);
	//void Clear();	
};