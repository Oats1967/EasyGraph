//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineSetpointLineDlg
///
/// @file   LineSetpointLineDlg.cpp
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
#include "EasyControl.h"
#include "LineSetpointLineDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLineSetpointLineDlg, _func)

IMPLEMENT_DYNAMIC(CLineSetpointLineDlg, CDialog)
//***********************************************************************************************
//***********************************************************************************************
CLineSetpointLineDlg::CLineSetpointLineDlg(CWnd* pParent)
	: CDialog(CLineSetpointLineDlg::IDD, pParent)
	, c_EditBrush(EDITBKCOLOR)
	, m_EditMap({
			EDITITEM(IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT, OnNotifyEditMaxSetpoint),
			EDITITEM(IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT, OnNotifyEditMinTotband),
			EDITITEM(IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT, OnNotifyEditHysteresis),
			EDITITEM(IDC_LINE_LINESETPOINT_FILTERTIME_EDIT, OnNotifyEditNominalFilterTime),

		})
	, m_MaxSetpoint(0.0F)
	, m_Hysteresis(0.0F)
	, m_MinTotBand(0.0F)
	, m_FilterTime(0)
{
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LINE_LINESETPOINT_MAXSETPOINT_INFO, m_MaxSetpointInfoButton);
	DDX_Control(pDX, IDC_LINE_LINESETPOINT_HYSTERESIS_INFO, m_HysteresisInfoButton);
	DDX_Control(pDX, IDC_LINE_LINESETPOINT_MINTOTBAND_INFO, m_MinTotBandInfoButton);
	DDX_Control(pDX, IDC_LINE_LINESETPOINT_FILTERTIME_INFO, m_FilterTimeInfoButton);


	DDX_Text(pDX, IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT, m_MaxSetpoint);
	DDX_Text(pDX, IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT, m_Hysteresis);
	DDX_Text(pDX, IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT, m_MinTotBand);
	DDX_Text(pDX, IDC_LINE_LINESETPOINT_FILTERTIME_EDIT, m_FilterTime);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineSetpointLineDlg, CDialog)

	ON_STN_CLICKED(IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT, &CLineSetpointLineDlg::OnStnClickedLineSetpointMaxSetpoint)
	ON_STN_CLICKED(IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT, &CLineSetpointLineDlg::OnStnClickedLineSetpointHysteresis)
	ON_STN_CLICKED(IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT, &CLineSetpointLineDlg::OnStnClickedLineSetpointMinTotband)
	ON_STN_CLICKED(IDC_LINE_LINESETPOINT_FILTERTIME_EDIT, &CLineSetpointLineDlg::OnStnClickedLineSetpointFilterTime)

	ON_BN_CLICKED(IDC_LINE_LINESETPOINT_MAXSETPOINT_INFO, &CLineSetpointLineDlg::OnBnClickedLineSetpointMaxSetpointInfo)
	ON_BN_CLICKED(IDC_LINE_LINESETPOINT_HYSTERESIS_INFO, &CLineSetpointLineDlg::OnBnClickedLineSetpointHysteresisInfo)
	ON_BN_CLICKED(IDC_LINE_LINESETPOINT_MINTOTBAND_INFO, &CLineSetpointLineDlg::OnBnClickedLineSetpointMinTotbandInfo)
	ON_BN_CLICKED(IDC_LINE_LINESETPOINT_FILTERTIME_INFO, &CLineSetpointLineDlg::OnBnClickedLineSetpointFilterTimeInfo)

	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnStnClickedLineSetpointMinTotband()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnStnClickedLineSetpointFilterTime()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LINE_LINESETPOINT_FILTERTIME_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnStnClickedLineSetpointHysteresis()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnStnClickedLineSetpointMaxSetpoint()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSetpointLineDlg::OnNotifyEditMaxSetpoint()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_MaxSetpoint);
	if (bModified)
	{
		REMOTEREF.setLineMaxSetpoint(m_MaxSetpoint);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSetpointLineDlg::OnNotifyEditMinTotband()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_MinTotBand);
	if (bModified)
	{
		REMOTEREF.setLineMinTotband(m_MinTotBand);
	}
	return bModified;

}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSetpointLineDlg::OnNotifyEditHysteresis()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_Hysteresis);
	if (bModified)
	{
		if (m_Hysteresis == 0.0F)
		{
			m_FilterTime = 0;
		}
		ENABLE_ID(IDC_LINE_LINESETPOINT_FILTERTIME_EDIT, BOOL(m_Hysteresis > 0.0F));
		REMOTEREF.setLineRampHysteresis(m_Hysteresis);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSetpointLineDlg::OnNotifyEditNominalFilterTime()
{
	auto bModified = CEditCtrlEx::GetLongAbsRange(m_FilterTime, 0, 100U);
	if (bModified)
	{
		REMOTEREF.setLineFilterTime(m_FilterTime);
	}
	return bModified;

}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineSetpointLineDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
{
	if (bValue)
	{
		BOOL bModified = FALSE;
		try
		{
			bModified = m_EditMap.at(_S32(id))();
		}
		catch (std::out_of_range)
		{
			ASSERT(FALSE);
			LOGERROR("Error in map: " << _S32(id));
		}
		if (bModified)
		{
			UpdateData(FALSE);
		}
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSetpointLineDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0);

	auto result = CDialog::OnInitDialog();
	if (result)
	{
		SaveParams();

		BOOL bMaxSetpointPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_ANLAGENMAXSETPOINT);
		BOOL bLineEditPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);

		const BOOL bLineGeneralEnable = bLineEditPermission;
		const BOOL bLineGeneralShow = TRUE;
		ENABLE_SHOW_ID(IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT, bLineGeneralEnable, bLineGeneralShow);
		SHOWW_ID(IDC_LINE_LINESETPOINT_HYSTERESIS_STATIC, bLineGeneralShow);
		SHOWW_ID(IDC_LINE_LINESETPOINT_HYSTERESIS_INFO, bLineGeneralShow);
		ENABLE_SHOW_ID(IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT, bLineGeneralEnable, bLineGeneralShow);
		SHOWW_ID(IDC_LINE_LINESETPOINT_MINTOTBAND_STATIC, bLineGeneralShow);
		SHOWW_ID(IDC_LINE_LINESETPOINT_MINTOTBAND_INFO, bLineGeneralShow);
		ENABLE_SHOW_ID(IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT, bLineGeneralEnable && bMaxSetpointPermission, bLineGeneralShow)
		SHOWW_ID(IDC_LINE_LINESETPOINT_MAXSETPOINT_STATIC, bLineGeneralShow)
		SHOWW_ID(IDC_LINE_LINESETPOINT_MAXSETPOINT_INFO, bLineGeneralShow)
		ENABLE_SHOW_ID(IDC_LINE_LINESETPOINT_FILTERTIME_EDIT, bLineGeneralEnable && (m_Hysteresis > 0.0F), bLineGeneralShow);
		SHOWW_ID(IDC_LINE_LINESETPOINT_FILTERTIME_STATIC, bLineGeneralShow);
		SHOWW_ID(IDC_LINE_LINESETPOINT_FILTERTIME_INFO, bLineGeneralShow);
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
HBRUSH CLineSetpointLineDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int32_t lID = pWnd->GetDlgCtrlID();
	switch (lID)
	{
	case IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT:
	case IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT:
	case IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT:
	case IDC_LINE_LINESETPOINT_FILTERTIME_EDIT:
		if (pWnd->IsWindowEnabled())
		{
			pDC->SetTextColor(EDITTEXTCOLOR);
			pDC->SetBkColor(EDITBKCOLOR);
			pDC->SetBkMode(OPAQUE);
			hbr = (HBRUSH)c_EditBrush;
		}
		break;
	default:
		break;

	}
	return hbr;
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnBnClickedLineSetpointMaxSetpointInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_LINESETPOINT_MAXSETPOINT_EDIT, IDS_LINE_MAXSETPOINT_KGH, IDS_INFO_LINE_MAXSETPOINT_KGH);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnBnClickedLineSetpointMinTotbandInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_LINESETPOINT_MINTOTBAND_EDIT, IDS_LINE_MINTOTBAND_KGH, IDS_INFO_LINE_MINTOTBAND_KGH);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnBnClickedLineSetpointFilterTimeInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_LINESETPOINT_FILTERTIME_EDIT, IDS_LINE_FILTERTIME_P, IDS_INFO_LINE_FILTERTIME_P);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnBnClickedLineSetpointHysteresisInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_LINESETPOINT_HYSTERESIS_EDIT, IDS_LINE_HYSTERESIS_KGH, IDS_INFO_LINE_HYSTERESIS_KGH);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::SaveParams()
{
	float32_t fValue = 0.0F;

	REMOTEREF.getLineMinTotband(fValue);
	m_MinTotBand = fValue;

	REMOTEREF.getLineRampHysteresis(fValue);
	m_Hysteresis = fValue;

	REMOTEREF.getLineMaxSetpoint(fValue);
	m_MaxSetpoint = fValue;

	uint32_t lValue = 0;
	REMOTEREF.getLineFilterTime(lValue);
	m_FilterTime = lValue;

	backupParam.FilterTime = m_FilterTime;
	backupParam.Hysteresis = m_Hysteresis;
	backupParam.MaxSetpoint = m_MaxSetpoint;
	backupParam.MinTotBand = m_MinTotBand;

	UpdateData(FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::RestoreParams()
{
	UpdateData(TRUE);

	if (m_MinTotBand != backupParam.MinTotBand)
	{
		REMOTEREF.setLineMinTotband(backupParam.MinTotBand);
	}
	if (m_Hysteresis != backupParam.Hysteresis)
	{
		REMOTEREF.setLineRampHysteresis(backupParam.Hysteresis);
	}
	if (m_MaxSetpoint != backupParam.MaxSetpoint)
	{
		REMOTEREF.setLineMaxSetpoint(backupParam.MaxSetpoint);
	}
	if (m_FilterTime != backupParam.FilterTime)
	{
		REMOTEREF.setLineFilterTime(backupParam.FilterTime);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLineDlg::OnCancel()
{
	RestoreParams();
	CDialog::OnCancel();
}


