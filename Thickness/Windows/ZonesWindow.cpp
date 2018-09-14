#include "stdafx.h"
#include "ZonesWindow.h"
#include "SensorViewer.h"
#include "typelist.hpp"
#include "WindowsEventTemplate.hpp"
#include "EmptyWindow.h"
#include "PrimaryData.h"
#include "Compute.h"
#include "ZonesWindowToolBar.h"
#include "MenuApi.h"
#include "ZonesWindowMenu.hpp"
#include "DebugMess.h"
//-------------------------------------------------------------------------
ZonesWindow::ZonesWindow() 
	: hWnd(NULL)	
	, signalLength(Singleton<LanParametersTable>::Instance().items.get<PacketSize>().value)
//	, frequencyFrames(Singleton<AdditionalSettingsTable>::Instance().items.get<FrequencyFrames>().value)
{
	zonesViewer.SetMouseMoveHandler(this, &ZonesWindow::MouseMoveHandler);
}
//-----------------------------------------------------------------------------
void ZonesWindow::operator()(TCommand &m)
{
	GetMenuToolBarEvent(m);
}
//--------------------------------------------------------------------------------------
void ZonesWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hToolBar, 0, 0, 0, 0, false);
	RECT rt;
	GetClientRect(hToolBar, &rt);
	int dy = rt.bottom;
	RECT r;
	GetClientRect(m.hwnd, &r);	
    MoveWindow(zonesViewer.hWnd , 0, dy, r.right, 200, true);
	dy += 200;
	double dx = (double)r.right / 3;
    MoveWindow(signalViewer.hWnd , 0, dy, int(dx), r.bottom - dy, true);
	MoveWindow(correlationViewer.hWnd, int(dx), dy, int(dx), r.bottom - dy, true);	
	MoveWindow(acfViewer.hWnd, int(dx * 2), dy, int(dx), r.bottom - dy, true);
}
//-------------------------------------------------------------------------
unsigned ZonesWindow::operator()(TCreate &m)
{
	Menu<ZonesWindowMenu::MainMenu>().Init(m.hwnd);
	hToolBar = InitToolbar<detailied_tool_button_list, 16>()(m.hwnd);
	zonesViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<ZonesViewer>::Proc, L"ZonesViewer", &zonesViewer);
    signalViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<SignalViewer>::Proc, L"SignalViewer", &signalViewer);
    correlationViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<CorrelationViewer>::Proc, L"CorrelationViewer", &correlationViewer);
	acfViewer.hWnd = CreateChildWindow(m.hwnd, (WNDPROC)&Viewer<ACFViewer>::Proc, L"ACFViewer", &acfViewer);
	return 0;
}
//-------------------------------------------------------------------------
unsigned ZonesWindow::operator()(TMessage &m)
{
	return 0;
}
//-------------------------------------------------------------------------
void ZonesWindow::operator()(TGetMinMaxInfo &m)
{
	if(NULL != m.pMinMaxInfo)
	{
		m.pMinMaxInfo->ptMinTrackSize.x = 600;
		m.pMinMaxInfo->ptMinTrackSize.y = 600;
		RepaintWindow(m.hwnd);
	}	
}
//-------------------------------------------------------------------------
void ZonesWindow::operator()(TDestroy &m)
{
	hWnd = 0;
}
//-----------------------------------------------------------------------------
void ZonesWindow::Open_(unsigned sensor_, unsigned zone_)
{
	int count = int((primaryData.offsetOfTime[0]) );
	offset = 0;
	++zone_;
	if(zone_ > 0) 
	{
		int t = int((primaryData.offsetOfTime[zone_]) );
		if(t > primaryData.GetCurrentOffset()) t = primaryData.GetCurrentOffset() - 2;
		offset = int((primaryData.offsetOfTime[zone_ - 1]) );
		count = t - offset + 1;
		
	}
	if(0 < count)
	{
		fft.Init(signalLength);
		zone = zone_;
		sensor = sensor_;
		if(dimention_of(zonesViewer.data.zones) < count ) count = dimention_of(zonesViewer.data.zones);
		for(int i = 0, j = offset; i < count + 1; ++i, ++j)
		{
			zonesViewer.data.zones[i] = primaryData.result[sensor][j];
			zonesViewer.data.status[i] = primaryData.status[sensor][j];
		}
		wchar_t buf[128];
		wsprintf(buf, L"Просмотр первичного сигнала. Датчик %d Зона %d", 1 + sensor, zone);
		zonesViewer.data.countZones = count;
		zonesViewer.zonesOffset = offset;

		signalViewer.coefficientA = compute.coefficientA[sensor];
		signalViewer.coefficientB = compute.coefficientB[sensor];

		correlationViewer.coefficientA = compute.coefficientA[sensor];
		correlationViewer.coefficientB = compute.coefficientB[sensor];
		correlationViewer.minEnergy = compute.minEnergy[sensor];

		acfViewer.coefficientA = compute.coefficientA[sensor];
		acfViewer.coefficientB = compute.coefficientB[sensor];
		acfViewer.peak = compute.peak[sensor];
		acfViewer.minEnergy = compute.minEnergy[sensor];

		acfViewer.acfBorderLeft = compute.acfBorderLeft[sensor];
		acfViewer.acfBorderRight = compute.acfBorderRight[sensor];

		if(0 == hWnd)
		{		
			RECT r;
			WindowPosition::Get<ZonesWindow>(r);
			WindowTemplate(this, L"", r.left, r.top, r.right, r.bottom);
		}
		SetWindowText(hWnd, buf);
		ShowWindow(hWnd, SW_SHOW);
		SetForegroundWindow(hWnd);
	    RepaintWindow(zonesViewer.hWnd);
		MouseMoveHandler(zonesViewer.offset);
	}
}
//--------------------------------------------------------------------------------------
void ZonesWindow::Open(unsigned sensor, unsigned zone)
{	
	Instance().Open_(sensor, zone);
}
//------------------------------------------------------------------------------
ZonesWindow &ZonesWindow::Instance()
{
	static ZonesWindow x; return x;
}
//------------------------------------------------------------------------------------
void ZonesWindow::MouseMoveHandler(unsigned offsetInZone)
{
	unsigned offs = offset + offsetInZone;
	char *s = primaryData.SensorData(sensor, offs);
	for(int i = 0; i < signalLength; ++i)
	{
		signalViewer.data[i] = s[i] + 128;
	}
	RepaintWindow(signalViewer.hWnd);
	//{
		double (&data)[1024] = correlationViewer.data;
		int i = 0;
		int dx = signalLength / 2;
		for(; i < dx; ++i)
		{
			data[i] = s[i] * 1.0/dx * i;
		}
		for(; i < signalLength; ++i)
		{
			data[i] = s[i] * (1.0 - 1.0/dx * (i - dx));
		}
		fft.Direct(data);
		fft.Spectrum(data);
		//--------------------------------------отсечение в частотной области
		ZeroMemory(data, sizeof(double) * compute.acfBorderLeft[sensor]);
		ZeroMemory(&data[compute.acfBorderRight[sensor]], sizeof(double) * (fft.bufferSize - compute.acfBorderRight[sensor]));

		dx = (compute.acfBorderRight[sensor] - compute.acfBorderLeft[sensor]) / 2;
		double dy = 1.0 / dx;
		int middle = compute.acfBorderLeft[sensor] + dx;

		int j = compute.acfBorderLeft[sensor];
		int xxxx = j;
		int right = compute.acfBorderRight[sensor];
		for(; j < middle; ++j)
		{
			data[j] *= dy * (j - xxxx);
		}
		xxxx = j;
		for(; j < right; ++j)
		{
			data[j] *= (1.0 - dy * (j - xxxx));
		}
		//---------------------------------------------------------------------------------------
		fft.Direct(data);
		fft.Spectrum(data);		
		correlationViewer.thickness = primaryData.result[sensor][offs];
		correlationViewer.status = primaryData.status[sensor][offs];
		correlationViewer.chart.maxAxesY = data[0];
		correlationViewer.chart.maxAxesX = fft.bufferSize / 2 - 1;
	//	{
			//-----------------------------------поиск пика	
			int maxOffs = int((Singleton<BorderCredibilityTable>::Instance().items.get<MaximumThicknessPipeWall>().value 
				- correlationViewer.coefficientB) / correlationViewer.coefficientA);
			int z = int((Singleton<BorderCredibilityTable>::Instance().items.get<MinimumThicknessPipeWall>().value 
				- correlationViewer.coefficientB) / correlationViewer.coefficientA);

			z /= 2;
			double minVal = data[z];
			double maxVal = minVal;
			double val = 0;
			double min = minVal;
			int offsMin = z;
			int offsMax = z;
			
			for(; z < maxOffs; ++z)
			{
				if(minVal > data[z]) 
				{
					minVal = data[z];
					offsMin = z;
					if(offsMin > offsMax) 
					{
						minVal = maxVal = data[z];
						continue;
					}
				}
				if(maxVal < data[z])
				{
					maxVal = data[z];
					offsMax = z;
				}
				double t = maxVal - minVal;
				if(val < t) 
				{
					val = t;
					min = minVal;
				}
			}
			correlationViewer.peak = compute.peak[sensor] * data[0] + min;
			correlationViewer.bottomBorder = min;
	//	}		
		RepaintWindow(correlationViewer.hWnd);
//	}
	{
		double (&data)[1024] = acfViewer.data;
		int i = 0;
		for(; i < signalLength; ++i)
		{
			data[i] = s[i];
		}
		for(; i < fft.bufferSize; ++i)
		{
			data[i] = 0;
		}
		fft.Direct(data);
		fft.Spectrum(data);
		acfViewer.thickness = primaryData.result[sensor][offs];
		double max = 0;
		for(int i = 0; i < fft.bufferSize; ++i) if(data[i] > max) max = data[i];
		acfViewer.chart.maxAxesY = max;
		acfViewer.chart.maxAxesX = fft.bufferSize / 2 - 1;
		RepaintWindow(acfViewer.hWnd);
	}
}
//-----------------------------------------------------------------------------------------------
bool __sent_mess_(TMouseWell &l, HWND h)
{
	RECT r;
	GetWindowRect(h, &r);
	if(InRect(l.x, l.y, r))
	{
		l.hwnd = h; 
		SendMessage(MESSAGE(l));
		return false;
	}
    return true;
}
void ZonesWindow::operator()(TKeyDown &l)
{
	switch(l.VirtKey)
	{
	case VK_LEFT: 
	case VK_RIGHT:  
		{
		HWND h[] = {
			zonesViewer.hWnd
			, signalViewer.hWnd
			, correlationViewer.hWnd
			, acfViewer.hWnd
		};
		currentScreen = currentScreen < 0 ? dimention_of(h) - 1 : currentScreen >= dimention_of(h) ? 0  : currentScreen;
		l.hwnd = h[currentScreen];
		SendMessage(MESSAGE(l));
		}
		break;
	case VK_UP: --currentScreen; break;
	case VK_DOWN: ++currentScreen; break;
	}	
}

void ZonesWindow::operator()(TMouseWell &l)
{
	__sent_mess_(l, zonesViewer.hWnd) && __sent_mess_(l, signalViewer.hWnd) 
		&& __sent_mess_(l, correlationViewer.hWnd) &&__sent_mess_(l, acfViewer.hWnd);
}

void ZonesWindow::operator()(TSysKeyDown &l)
{
	dprint(__FUNCTION__"%x %d", l.KeyData, l.VirtKey);
	if(0 == ((1 << 30) & l.KeyData))
	{
		switch(l.VirtKey)
		{
		case VK_LEFT: LeftArrowHandler(l.hwnd);break;
		case VK_RIGHT: RightArrowHandler(l.hwnd);break;
		case VK_UP: UpArrowHandler(l.hwnd);break;
		case VK_DOWN: DownArrowHandler(l.hwnd);break;
		}
	}
}
