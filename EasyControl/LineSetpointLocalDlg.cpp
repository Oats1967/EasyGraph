//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineSetpointLocalDlg
///
/// @file   LineSetpointLocalDlg.cpp
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
#include "LineSetpointLocalDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLineSetpointLocalDlg, _func)

IMPLEMENT_DYNAMIC(CLineSetpointLocalDlg, CDialog)
//***********************************************************************************************
//***********************************************************************************************
CLineSetpointLocalDlg::CLineSetpointLocalDlg(CWnd* pParent)
	: CDialog(CLineSetpointLocalDlg::IDD, pParent)
	, c_EditBrush(EDITBKCOLOR)
	, m_EditMap({
			EDITITEM(IDC_LINE_LOCALSETPOINT_MAXSETPOINT, OnNotifyEditMaxSetpoint),

		})
	, m_MaxSetpoint(0.0F)
{
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLocalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LINE_LOCALSETPOINT_MAXSETPOINT_INFO, m_MaxSetpointInfoButton);
	DDX_Text(pDX, IDC_LINE_LOCALSETPOINT_MAXSETPOINT, m_MaxSetpoint);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineSetpointLocalDlg, CDialog)

	ON_STN_CLICKED(IDC_LINE_LOCALSETPOINT_MAXSETPOINT, &CLineSetpointLocalDlg::OnStnClickedLineSetpointMaxSetpoint)
	ON_BN_CLICKED(IDC_LINE_LOCALSETPOINT_MAXSETPOINT_INFO, &CLineSetpointLocalDlg::OnBnClickedLineSetpointMaxSetpointInfo)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLocalDlg::OnStnClickedLineSetpointMaxSetpoint()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_LOCALSETPOINT_MAXSETPOINT);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineSetpointLocalDlg::OnNotifyEditMaxSetpoint()
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
LRESULT CLineSetpointLocalDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			LOGERROR("Error not found" << _S32(id));
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
BOOL CLineSetpointLocalDlg::OnInitDialog()
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
		ENABLE_SHOW_ID(IDC_LINE_LOCALSETPOINT_MAXSETPOINT, bLineGeneralEnable && bMaxSetpointPermission, bLineGeneralShow)
			SHOWW_ID(IDC_LINE_LOCALSETPOINT_MAXSETPOINT_STATIC, bLineGeneralShow)
			SHOWW_ID(IDC_LINE_LOCALSETPOINT_MAXSETPOINT_INFO, bLineGeneralShow)
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
HBRUSH CLineSetpointLocalDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int32_t lID = pWnd->GetDlgCtrlID();
	switch (lID)
	{
	case IDC_LINE_LOCALSETPOINT_MAXSETPOINT:
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
void CLineSetpointLocalDlg::OnBnClickedLineSetpointMaxSetpointInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_LOCALSETPOINT_MAXSETPOINT, IDS_LINE_MAXSETPOINT_KGH, IDS_INFO_LINE_MAXSETPOINT_KGH);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLocalDlg::SaveParams()
{
	float32_t fValue = 0.0F;
	REMOTEREF.getLineMaxSetpoint(fValue);
	m_MaxSetpoint = fValue;

	backupParam.MaxSetpoint = m_MaxSetpoint;
	UpdateData(FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLocalDlg::RestoreParams()
{
	UpdateData(TRUE);

	if (m_MaxSetpoint != backupParam.MaxSetpoint)
	{
		REMOTEREF.setLineMaxSetpoint(backupParam.MaxSetpoint);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineSetpointLocalDlg::OnCancel()
{
	RestoreParams();
	CDialog::OnCancel();
}


