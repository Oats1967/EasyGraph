//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module DiagramScaleDlg
///
/// @file   DiagramScaleDlg.cpp
///
///
/// @coypright(c)  Ing.büro Hafer
///                Branderweg 8A
///                D-91058 Erlangen
///
/// @author        Detlef Hafer
///
//------------------------------------------------------------------------------------

#include "stdafx.h"
#include "MFCMacros.h"
#include "DiagramScaleDlg.h"
#include "EditCtrlEx.h"


// CDiagramScaleDlg-Dialog
#define EDITITEM(_a, _func) 	BINDFUNC(_a, CDiagramScaleDlg, _func)

#define AUTO_ID base::CAxisScale::TupleId::eAuto
#define MIN_ID base::CAxisScale::TupleId::eMin
#define MAX_ID base::CAxisScale::TupleId::eMax

IMPLEMENT_DYNAMIC(CDiagramScaleDlg, CBaseClass)

//**************************************************************************************************************
//**************************************************************************************************************
BEGIN_MESSAGE_MAP(CDiagramScaleDlg, CBaseClass)
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MIN, &CDiagramScaleDlg::OnStnClickedDiagramScaleDosePerformanceMin)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MAX, &CDiagramScaleDlg::OnStnClickedDiagramScaleDosePerformanceMax)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_SETPOINT_MIN, &CDiagramScaleDlg::OnStnClickedDiagramScaleSetpointMin)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_SETPOINT_MAX, &CDiagramScaleDlg::OnStnClickedDiagramScaleSetpointMax)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_MASSFLOW_MIN, &CDiagramScaleDlg::OnStnClickedDiagramScaleMassflowMin)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_MASSFLOW_MAX, &CDiagramScaleDlg::OnStnClickedDiagramScaleMassflowMax)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_WEIGHT_MIN, &CDiagramScaleDlg::OnStnClickedDiagramScaleWeightMin)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_WEIGHT_MAX, &CDiagramScaleDlg::OnStnClickedDiagramScaleWeightMax)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_DRIVECOMMAND_MIN, &CDiagramScaleDlg::OnStnClickedDiagramScaleDriveCommandMin)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_DRIVECOMMAND_MAX, &CDiagramScaleDlg::OnStnClickedDiagramScaleDriveCommandMax)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_ROTSPEED_MIN, &CDiagramScaleDlg::OnStnClickedDiagramScaleRotSpeedMin)
	ON_STN_CLICKED(IDC_DIAGRAM_SCALE_ROTSPEED_MAX, &CDiagramScaleDlg::OnStnClickedDiagramScaleRotSpeedMax)
	ON_BN_CLICKED(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_AUTO, &CDiagramScaleDlg::OnBnClickedDosePerformanceAuto)
	ON_BN_CLICKED(IDC_DIAGRAM_SCALE_SETPOINT_AUTO, &CDiagramScaleDlg::OnBnClickedSetpointAuto)
	ON_BN_CLICKED(IDC_DIAGRAM_SCALE_MASSFLOW_AUTO, &CDiagramScaleDlg::OnBnClickedMassflowAuto)
	ON_BN_CLICKED(IDC_DIAGRAM_SCALE_WEIGHT_AUTO, &CDiagramScaleDlg::OnBnClickedWeightAuto)
	ON_BN_CLICKED(IDC_DIAGRAM_SCALE_DRIVECOMMAND_AUTO, &CDiagramScaleDlg::OnBnClickedDriveCommandAuto)
	ON_BN_CLICKED(IDC_DIAGRAM_SCALE_ROTSPEED_AUTO, &CDiagramScaleDlg::OnBnClickedRotSpeedAuto)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//**************************************************************************************************************
//**************************************************************************************************************
CDiagramScaleDlg::CDiagramScaleDlg(CWnd* pParent, base::CAxisScale& rScale)
	: CBaseClass(CDiagramScaleDlg::IDD, pParent)
	, c_EditBrush{ EDITBKCOLOR }
	, c_EditMap({
			EDITITEM(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MIN, OnNotifyDosePerformanceMin),
			EDITITEM(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MAX, OnNotifyDosePerformanceMax),
			EDITITEM(IDC_DIAGRAM_SCALE_SETPOINT_MIN, OnNotifySetpointMin),
			EDITITEM(IDC_DIAGRAM_SCALE_SETPOINT_MAX, OnNotifySetpointMax),
			EDITITEM(IDC_DIAGRAM_SCALE_MASSFLOW_MIN, OnNotifyMassflowMin),
			EDITITEM(IDC_DIAGRAM_SCALE_MASSFLOW_MAX, OnNotifyMassflowMax),
			EDITITEM(IDC_DIAGRAM_SCALE_WEIGHT_MIN, OnNotifyWeightMin),
			EDITITEM(IDC_DIAGRAM_SCALE_WEIGHT_MAX, OnNotifyWeightMax),
			EDITITEM(IDC_DIAGRAM_SCALE_DRIVECOMMAND_MIN, OnNotifyDriveCommandMin),
			EDITITEM(IDC_DIAGRAM_SCALE_DRIVECOMMAND_MAX, OnNotifyDriveCommandMax),
			EDITITEM(IDC_DIAGRAM_SCALE_ROTSPEED_MIN, OnNotifyRotSpeedMin),
			EDITITEM(IDC_DIAGRAM_SCALE_ROTSPEED_MAX, OnNotifyRotSpeedMax)
		})
	, r_AxisScale( rScale )
	, m_SaveAxisScale( rScale )
	, m_AutoSetpoint()
	, m_AutoWeight()
	, m_AutoDriveCommand()
	, m_AutoRotSpeed()
	, m_AutoDosePerformance()
	, m_AutoMassflow()
{
}
//**************************************************************************************************************
//**************************************************************************************************************
CDiagramScaleDlg::~CDiagramScaleDlg()
{
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseClass::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MIN,	r_AxisScale.GetRef<MIN_ID>(base::eMassflowSelect::eVIEWDOSEPERFORMANCE));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_SETPOINT_MIN,			r_AxisScale.GetRef<MIN_ID>(base::eMassflowSelect::eVIEWSETPOINT));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_MASSFLOW_MIN,			r_AxisScale.GetRef<MIN_ID>(base::eMassflowSelect::eVIEWMASSFLOW));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_DRIVECOMMAND_MIN,		r_AxisScale.GetRef<MIN_ID>(base::eMassflowSelect::eVIEWDRIVECOMMAND));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_ROTSPEED_MIN,			r_AxisScale.GetRef<MIN_ID>(base::eMassflowSelect::eVIEWROTSPEED));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_WEIGHT_MIN,				r_AxisScale.GetRef<MIN_ID>(base::eMassflowSelect::eVIEWWEIGHT));

	DDX_Text(pDX, IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MAX,	r_AxisScale.GetRef<MAX_ID>(base::eMassflowSelect::eVIEWDOSEPERFORMANCE));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_SETPOINT_MAX,			r_AxisScale.GetRef<MAX_ID>(base::eMassflowSelect::eVIEWSETPOINT));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_MASSFLOW_MAX,			r_AxisScale.GetRef<MAX_ID>(base::eMassflowSelect::eVIEWMASSFLOW));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_WEIGHT_MAX,				r_AxisScale.GetRef<MAX_ID>(base::eMassflowSelect::eVIEWWEIGHT));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_DRIVECOMMAND_MAX,		r_AxisScale.GetRef<MAX_ID>(base::eMassflowSelect::eVIEWDRIVECOMMAND));
	DDX_Text(pDX, IDC_DIAGRAM_SCALE_ROTSPEED_MAX,			r_AxisScale.GetRef<MAX_ID>(base::eMassflowSelect::eVIEWROTSPEED));

	DDX_Control(pDX, IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_AUTO, m_AutoDosePerformance);
	DDX_Control(pDX, IDC_DIAGRAM_SCALE_SETPOINT_AUTO, m_AutoSetpoint);
	DDX_Control(pDX, IDC_DIAGRAM_SCALE_MASSFLOW_AUTO, m_AutoMassflow);
	DDX_Control(pDX, IDC_DIAGRAM_SCALE_WEIGHT_AUTO, m_AutoWeight);
	DDX_Control(pDX, IDC_DIAGRAM_SCALE_DRIVECOMMAND_AUTO, m_AutoDriveCommand);
	DDX_Control(pDX, IDC_DIAGRAM_SCALE_ROTSPEED_AUTO, m_AutoRotSpeed);

}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnCancel()
{
	r_AxisScale = m_SaveAxisScale;
	CBaseClass::OnCancel();
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnInitDialog()
{
	auto result = CBaseClass::OnInitDialog();
	if (result)
	{
		m_AutoDosePerformance.SizeToContent();
		m_AutoSetpoint.SizeToContent();
		m_AutoMassflow.SizeToContent();
		m_AutoWeight.SizeToContent();
		m_AutoDriveCommand.SizeToContent();
		m_AutoRotSpeed.SizeToContent();
		SetValue();
	}
	return result;
}

//**************************************************************************************************************
//**************************************************************************************************************
LRESULT CDiagramScaleDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = c_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error");
		}
		if (bModified)
		{
			SetValue();
		}
	}
	return 0;
}
//******************************************************************************************************
//******************************************************************************************************
void CDiagramScaleDlg::SetValue(void)
{
	BOOL  bEnable = ! r_AxisScale.GetAuto(base::eMassflowSelect ::eVIEWDOSEPERFORMANCE);
	ENABLE_ID(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MIN, bEnable);
	ENABLE_ID(IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MAX, bEnable);
	m_AutoDosePerformance.SetAuto(!bEnable);


	bEnable = ! r_AxisScale.GetAuto(base::eMassflowSelect::eVIEWSETPOINT);
	ENABLE_ID(IDC_DIAGRAM_SCALE_SETPOINT_MIN, bEnable);
	ENABLE_ID(IDC_DIAGRAM_SCALE_SETPOINT_MAX, bEnable);
	m_AutoSetpoint.SetAuto(!bEnable);

	bEnable = ! r_AxisScale.GetAuto(base::eMassflowSelect::eVIEWMASSFLOW);
	ENABLE_ID(IDC_DIAGRAM_SCALE_MASSFLOW_MIN, bEnable);
	ENABLE_ID(IDC_DIAGRAM_SCALE_MASSFLOW_MAX, bEnable);
	m_AutoMassflow.SetAuto(!bEnable);

	bEnable = ! r_AxisScale.GetAuto(base::eMassflowSelect::eVIEWWEIGHT);
	ENABLE_ID(IDC_DIAGRAM_SCALE_WEIGHT_MIN, bEnable);
	ENABLE_ID(IDC_DIAGRAM_SCALE_WEIGHT_MAX, bEnable);
	m_AutoWeight.SetAuto(!bEnable);

	bEnable = ! r_AxisScale.GetAuto(base::eMassflowSelect::eVIEWDRIVECOMMAND);
	ENABLE_ID(IDC_DIAGRAM_SCALE_DRIVECOMMAND_MIN, bEnable);
	ENABLE_ID(IDC_DIAGRAM_SCALE_DRIVECOMMAND_MAX, bEnable);
	m_AutoDriveCommand.SetAuto(!bEnable);

	bEnable = ! r_AxisScale.GetAuto(base::eMassflowSelect::eVIEWROTSPEED);
	ENABLE_ID(IDC_DIAGRAM_SCALE_ROTSPEED_MIN, bEnable);
	ENABLE_ID(IDC_DIAGRAM_SCALE_ROTSPEED_MAX, bEnable);
	m_AutoRotSpeed.SetAuto(!bEnable);

	UpdateData(FALSE);
}
//******************************************************************************************************
//******************************************************************************************************
void CDiagramScaleDlg::GetValue(void)
{
	UpdateData(TRUE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleDosePerformanceMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleDosePerformanceMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleSetpointMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_SETPOINT_MIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleSetpointMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_SETPOINT_MAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleMassflowMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_MASSFLOW_MIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleMassflowMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_MASSFLOW_MAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleWeightMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_WEIGHT_MIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleWeightMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_WEIGHT_MAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleDriveCommandMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_DRIVECOMMAND_MIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleDriveCommandMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_DRIVECOMMAND_MAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleRotSpeedMin()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_ROTSPEED_MIN);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnStnClickedDiagramScaleRotSpeedMax()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_DIAGRAM_SCALE_ROTSPEED_MAX);
}
//**************************************************************************************************************
//**************************************************************************************************************
template <int32_t ix>
BOOL CDiagramScaleDlg::OnNotifyValue(const base::eMassflowSelect sel)
{
	auto newvalue = _S32(CEditCtrl::GetLongAbs());

	auto& currentvalue = r_AxisScale.GetRef<ix>(sel);
	BOOL bModified = (newvalue != currentvalue);
	if (bModified)
	{
		currentvalue = newvalue;
	}
	return bModified;
}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyDosePerformanceMin()
{	return OnNotifyValue<MIN_ID>(base::eMassflowSelect::eVIEWDOSEPERFORMANCE);}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyDosePerformanceMax()
{	return OnNotifyValue<MAX_ID>(base::eMassflowSelect::eVIEWDOSEPERFORMANCE); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifySetpointMin()
{	return OnNotifyValue<MIN_ID>(base::eMassflowSelect::eVIEWSETPOINT); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifySetpointMax()
{ return OnNotifyValue<MAX_ID>(base::eMassflowSelect::eVIEWSETPOINT); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyMassflowMin()
{	return OnNotifyValue<MIN_ID>(base::eMassflowSelect::eVIEWMASSFLOW);}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyMassflowMax()
{ return OnNotifyValue<MAX_ID>(base::eMassflowSelect::eVIEWMASSFLOW); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyWeightMin()
{return OnNotifyValue<MIN_ID>(base::eMassflowSelect::eVIEWWEIGHT); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyWeightMax()
{	return OnNotifyValue<MAX_ID>(base::eMassflowSelect::eVIEWWEIGHT); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyDriveCommandMin()
{	return OnNotifyValue<MIN_ID>(base::eMassflowSelect::eVIEWDRIVECOMMAND); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyDriveCommandMax()
{	return OnNotifyValue<MAX_ID>(base::eMassflowSelect::eVIEWDRIVECOMMAND); }
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyRotSpeedMin()
{	return OnNotifyValue<MIN_ID>(base::eMassflowSelect::eVIEWROTSPEED);}
//**************************************************************************************************************
//**************************************************************************************************************
BOOL CDiagramScaleDlg::OnNotifyRotSpeedMax()
{	return OnNotifyValue<MAX_ID>(base::eMassflowSelect::eVIEWROTSPEED);}

//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::ToggleAuto(const base::eMassflowSelect select)
{
	r_AxisScale.SetAuto(select, !r_AxisScale.GetAuto(select));
	SetValue();
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnBnClickedDosePerformanceAuto()
{
	ToggleAuto(base::eMassflowSelect::eVIEWDOSEPERFORMANCE);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnBnClickedSetpointAuto()
{
	ToggleAuto(base::eMassflowSelect::eVIEWSETPOINT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnBnClickedMassflowAuto()
{
	ToggleAuto(base::eMassflowSelect::eVIEWMASSFLOW);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnBnClickedWeightAuto()
{
	ToggleAuto(base::eMassflowSelect::eVIEWWEIGHT);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnBnClickedDriveCommandAuto()
{
	ToggleAuto(base::eMassflowSelect::eVIEWDRIVECOMMAND);
}
//**************************************************************************************************************
//**************************************************************************************************************
void CDiagramScaleDlg::OnBnClickedRotSpeedAuto()
{
	ToggleAuto(base::eMassflowSelect::eVIEWROTSPEED);
}
//**************************************************************************************************************
//**************************************************************************************************************
HBRUSH CDiagramScaleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseClass::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MIN:
	case IDC_DIAGRAM_SCALE_DOSEPERFORMANCE_MAX:
	case IDC_DIAGRAM_SCALE_SETPOINT_MIN:
	case IDC_DIAGRAM_SCALE_SETPOINT_MAX:
	case IDC_DIAGRAM_SCALE_MASSFLOW_MIN:
	case IDC_DIAGRAM_SCALE_MASSFLOW_MAX:
	case IDC_DIAGRAM_SCALE_WEIGHT_MIN:
	case IDC_DIAGRAM_SCALE_WEIGHT_MAX:
	case IDC_DIAGRAM_SCALE_DRIVECOMMAND_MIN:
	case IDC_DIAGRAM_SCALE_DRIVECOMMAND_MAX:
	case IDC_DIAGRAM_SCALE_ROTSPEED_MIN:
	case IDC_DIAGRAM_SCALE_ROTSPEED_MAX:
	{
		if (pWnd->IsWindowEnabled())
		{
			pDC->SetBkColor(EDITBKCOLOR);
			pDC->SetBkMode(OPAQUE);
			hbr = (HBRUSH)c_EditBrush;
		}
		pDC->SetTextColor(EDITTEXTCOLOR);
	}
	break;

	default:
		break;
	}
	return hbr;
}


