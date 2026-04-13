//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineExtSetpointDlg
///
/// @file   LineExtSetpointDlg.cpp
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
#include "LineExtSetpointDlg.h"
#include "DataImage.h"
#include "MFCMacros.h"
#include "FormatMetric.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"


#define MAXUPDATESWITCHES 2U

#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLineExtSetpointDlg, _func)

IMPLEMENT_DYNAMIC(CLineExtSetpointDlg, CDialog)
//***********************************************************************************************
//***********************************************************************************************
CLineExtSetpointDlg::CLineExtSetpointDlg(CWnd* pParent)
	: CDialog(CLineExtSetpointDlg::IDD, pParent)
	, c_EditBrush(EDITBKCOLOR)
	, c_GreenBrush{ COLORLIGHTGREEN }
	, m_chkUserInvertSetpoint(IDC_LINE_EXTSETPOINT_SETPOINT_INVERS, TRUE)
	, m_chkUserExtSetpoint(IDC_LINE_EXTSETPOINT_SETPOINT_ENABLE, TRUE)
	, m_EditMap({
			EDITITEM(IDC_LINE_EXTSETPOINT_SCALE_EDIT, OnNotifyEditScale),
			EDITITEM(IDC_LINE_EXTSETPOINT_OFFSET_EDIT, OnNotifyEditOffset)
		})
	, m_Scale(0.0F)
	, m_Offset(0.0F)
	, m_ActExtSetpoint(0.0F)
	, m_lUpdateSwitches{ 0 }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LINE_EXTSETPOINT_SETPOINT_INVERS, m_chkUserInvertSetpoint);
	DDX_Control(pDX, IDC_LINE_EXTSETPOINT_SETPOINT_ENABLE, m_chkUserExtSetpoint);
	DDX_Control(pDX, IDC_LINE_EXTSETPOINT_SCALE_INFO, m_ScaleInfoButton);
	DDX_Control(pDX, IDC_LINE_EXTSETPOINT_OFFSET_INFO, m_OffsetInfoButton);
	DDX_Control(pDX, IDC_LINE_EXTSETPOINT_ACTSETPOINT_INFO, m_ActExtSetpointInfoButton);
	DDX_Text(pDX, IDC_LINE_EXTSETPOINT_SCALE_EDIT, m_Scale);
	DDX_Text(pDX, IDC_LINE_EXTSETPOINT_OFFSET_EDIT, m_Offset);
	DDX_Float(pDX, IDC_LINE_EXTSETPOINT_ACTSETPOINT_EDIT, m_ActExtSetpoint);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineExtSetpointDlg, CDialog)

	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_LINE_EXTSETPOINT_SCALE_EDIT, &CLineExtSetpointDlg::OnStnClickedLineExtSetpointScaleEdit)
	ON_STN_CLICKED(IDC_LINE_EXTSETPOINT_OFFSET_EDIT, &CLineExtSetpointDlg::OnStnClickedLineExtSetpointOffsetEdit)
	ON_BN_CLICKED(IDC_LINE_EXTSETPOINT_SCALE_INFO, &CLineExtSetpointDlg::OnBnClickedLineExtSetpointScaleInfo)
	ON_BN_CLICKED(IDC_LINE_EXTSETPOINT_OFFSET_INFO, &CLineExtSetpointDlg::OnBnClickedLineExtSetpointOffsetInfo)
	ON_BN_CLICKED(IDC_LINE_EXTSETPOINT_ACTSETPOINT_INFO, &CLineExtSetpointDlg::OnBnClickedLineExtSetpointActSetpointInfo)
	ON_REGISTERED_MESSAGE(BCGM_ON_GAUGE_CLICK, &CLineExtSetpointDlg::OnGaugeClick)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnStnClickedLineExtSetpointScaleEdit()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_EXTSETPOINT_SCALE_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnStnClickedLineExtSetpointOffsetEdit()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_EXTSETPOINT_OFFSET_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnBnClickedLineExtSetpointInversUser()
{
	const auto& linestatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
	const BOOL bActive = m_chkUserInvertSetpoint.IsOn();
	const BOOL bInvers = linestatus.flags.StateExtLineInvSetpoint;
	ASSERT(bActive != bInvers);
	if (bActive != bInvers)
	{
		REMOTEREF.setLineExtSetpointInversEnable(bActive);
		m_lUpdateSwitches = MAXUPDATESWITCHES;
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnBnClickedLineExtSetpointUser()
{
	const auto& linestatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
	const BOOL bExtSetpoint = linestatus.flags.StateExtlineSetpoint;
	const BOOL bActive = m_chkUserExtSetpoint.IsOn();
	ASSERT(bActive != bExtSetpoint);
	if (bActive != bExtSetpoint)
	{
		REMOTEREF.setLineExtSetpointEnable(bActive);
		m_lUpdateSwitches = MAXUPDATESWITCHES;
	}
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineExtSetpointDlg::OnNotifyEditScale()
{
	auto modified = CEditCtrlEx::GetFloatAbs(m_Scale);
	if (modified)
	{
		REMOTEREF.setLineExtSetpointScale(m_Scale);
	}
	return modified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineExtSetpointDlg::OnNotifyEditOffset()
{
	auto modified = CEditCtrlEx::GetFloatRange(m_Offset, -10.0f, 10.0f);
	if (modified)
	{
		REMOTEREF.setLineExtSetpointOffset(m_Offset);
	}
	return modified;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineExtSetpointDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			LOGERROR("Error not found : " << _S32(id));
		}
		if (bModified)
		{
			UpdateData(FALSE);
		}
	}
	return 0L;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineExtSetpointDlg::UpdateSwitches()
{
	const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;

	BOOL bLineEditPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);
	const BOOL bLineExternalSetpointEnable = (processstatus.flags.StateExtlineSetpoint != FALSE);
	ENABLE_ID(IDC_LINE_EXTSETPOINT_SCALE_EDIT, bLineEditPermission);

	const BOOL bUpdateSwitches = bLineEditPermission && (m_lUpdateSwitches == 0);
	ENABLE_ID(IDC_LINE_EXTSETPOINT_SETPOINT_ENABLE, bUpdateSwitches);
	if (bUpdateSwitches)
	{
		m_chkUserExtSetpoint.SetOn(bLineExternalSetpointEnable);
	}
	ENABLE_SHOW_ID(IDC_LINE_EXTSETPOINT_SETPOINT_INVERS, bUpdateSwitches, bLineExternalSetpointEnable);
	SHOWW_ID(IDC_LINE_EXTSETPOINT_SETPOINT_INVERS_STATIC, bLineExternalSetpointEnable);
	if (bLineExternalSetpointEnable && bUpdateSwitches)
	{
		const BOOL bLineExternalSetpointInverseEnable = (processstatus.flags.StateExtLineInvSetpoint != FALSE);
		m_chkUserInvertSetpoint.SetOn(bLineExternalSetpointInverseEnable);
	}
}

//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineExtSetpointDlg::UpdateControl()
{
	REMOTEREF.getLineExtSetpoint(m_ActExtSetpoint);
	UpdateData(FALSE);
}

//***********************************************************************************************
//***********************************************************************************************
BOOL CLineExtSetpointDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0);

	auto result = CDialog::OnInitDialog();
	if (result)
	{
		SaveParams();
		m_lUpdateSwitches = 0L;
		UpdateSwitches();
		UpdateControl();
		SetTimer(0, 1000U, NULL);
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
HBRUSH CLineExtSetpointDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int32_t lID = pWnd->GetDlgCtrlID();
	switch (lID)
	{
	case IDC_LINE_EXTSETPOINT_SCALE_EDIT:
	case IDC_LINE_EXTSETPOINT_OFFSET_EDIT:
		if (pWnd->IsWindowEnabled())
		{
			pDC->SetTextColor(EDITTEXTCOLOR);
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)c_EditBrush;
		}
		break;

	case IDC_LINE_EXTSETPOINT_ACTSETPOINT_EDIT:
		if (pWnd->IsWindowEnabled())
		{
			pDC->SetTextColor(EDITTEXTCOLOR);
			pDC->SetBkMode(TRANSPARENT);
			if (m_chkUserExtSetpoint.IsOn())
			{
				hbr = c_GreenBrush;
			}
		}
		break;
	default:
		break;

	}
	return hbr;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineExtSetpointDlg::OnGaugeClick(WPARAM wp, LPARAM)
{
	int32_t id = _S32(wp);
	switch (id)
	{
	case  IDC_LINE_EXTSETPOINT_SETPOINT_INVERS:
	{
		UpdateData(TRUE);
		OnBnClickedLineExtSetpointInversUser();
		UpdateData(FALSE);
		Invalidate(FALSE);
	}
	break;
	case  IDC_LINE_EXTSETPOINT_SETPOINT_ENABLE:
	{
		UpdateData(TRUE);
		OnBnClickedLineExtSetpointUser();
		UpdateData(FALSE);
		Invalidate(FALSE);
	}
	break;

	default:
		ASSERT(FALSE);
		break;
	}
	return 0L;
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnBnClickedLineExtSetpointScaleInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_EXTSETPOINT_SCALE_EDIT, IDS_LINE_SCALE_P, IDS_INFO_LINE_SCALE_P);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnBnClickedLineExtSetpointOffsetInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_EXTSETPOINT_OFFSET_EDIT, IDS_LINE_EXTSETPOINT_OFFSET, IDS_INFO_EXTSETPOINT_OFFSET);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::OnBnClickedLineExtSetpointActSetpointInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_EXTSETPOINT_ACTSETPOINT_EDIT, IDS_EXTSETPOINT_ACTSETPOINT_KGH, IDS_INFO_EXTSETPOINT_ACTSETPOINT);
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineExtSetpointDlg::OnTimer(UINT_PTR )
{
	if ( m_lUpdateSwitches )
	{
		m_lUpdateSwitches--;
	}
	UpdateSwitches();
	UpdateControl();
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineExtSetpointDlg::OnCancel()
{
	RestoreParams();
	return CDialog::OnCancel();
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::SaveParams()
{
	const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
	const BOOL bLineExternalSetpointEnable		   = (processstatus.flags.StateExtlineSetpoint != FALSE);
	const BOOL bLineExternalSetpointInverseEnable  = (processstatus.flags.StateExtLineInvSetpoint != FALSE);
	REMOTEREF.getLineExtSetpointOffset(m_Offset);
	REMOTEREF.getLineExtSetpointScale(m_Scale);
	backupParam.Scale = m_Scale;
	backupParam.Offset = m_Offset;
	backupParam.bLineExternalSetpointEnable			= bLineExternalSetpointEnable;
	backupParam.bLineExternalSetpointInverseEnable  = bLineExternalSetpointInverseEnable;
	UpdateData(FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineExtSetpointDlg::RestoreParams()
{
	UpdateData(TRUE);
	const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
	const BOOL bLineExternalSetpointEnable = (processstatus.flags.StateExtlineSetpoint != FALSE);
	const BOOL bLineExternalSetpointInverseEnable = (processstatus.flags.StateExtLineInvSetpoint != FALSE);

	if (bLineExternalSetpointEnable != backupParam.bLineExternalSetpointEnable)
	{
		REMOTEREF.setLineExtSetpointEnable(backupParam.bLineExternalSetpointEnable);
	}
	if (bLineExternalSetpointInverseEnable != backupParam.bLineExternalSetpointInverseEnable)
	{
		REMOTEREF.setLineExtSetpointInversEnable(backupParam.bLineExternalSetpointInverseEnable);
	}
	if (backupParam.Offset != m_Offset)
	{
		REMOTEREF.setLineExtSetpointOffset(backupParam.Offset);
	}
	if (backupParam.Scale != m_Scale)
	{
		REMOTEREF.setLineExtSetpointScale(backupParam.Scale);
	}
}




