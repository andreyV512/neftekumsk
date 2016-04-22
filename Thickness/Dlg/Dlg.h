#pragma once

#define DLG_DO(name)struct name{static void Do(HWND);};

DLG_DO(StoredData)
DLG_DO(LoadData)
DLG_DO(Recalculation)
DLG_DO(ThicknessDlg)
DLG_DO(DeadAreaDlg)
DLG_DO(BorderCredibilityDlg)
DLG_DO(RotationalSpeedDlg)
DLG_DO(SelectTypeSizeNewDlg)
DLG_DO(SelectTypeSizeDeleteDlg)
DLG_DO(DiscretePlateInputsDlg)
DLG_DO(DiscretePlateOutputsDlg)
DLG_DO(DiscretePlateDescriptorDlg)
DLG_DO(AnalogPlateDlg)
DLG_DO(CoefficientDlg)
DLG_DO(MedianFiltreDlg)
DLG_DO(SignalDlg)
DLG_DO(ACFBorderDlg)
DLG_DO(AboutWindowDlg)
DLG_DO(TcpDlg)
DLG_DO(ColorItemsDlg)
DLG_DO(AxesTableDlg)
#undef DLG_DO