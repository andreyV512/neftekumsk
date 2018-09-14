#pragma once
class	CFFT
{
	int    h[1024];
	double c[512];
public:
	int		n;
private:
	int		m;
	int		nv;
	double *x;
	int lastSize;
public:
	const int &bufferSize;
	CFFT ();
	void     Init(int);
	void	 Direct(double *);
	void	 Inverse(double *);
	void	 Spectrum(double *);
};
