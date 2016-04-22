#pragma once

struct StatusLabel
{
	char status;
	char *text;
    StatusLabel(char status);		
};

struct ColorBar
{
	double &data;
	unsigned &color;
	char status;
    ColorBar(double &data, unsigned &color, char status);
};