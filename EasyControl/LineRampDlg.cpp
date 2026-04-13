//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineRampDlg
///
/// @file   LineRampDlg.cpp
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
#include "LineRampDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLineRampDlg, _func)

IMPLEMENT_DYNAMIC(CLineRampDlg, CDialog)
//***********************************************************************************************
//***********************************************************************************************
CLineRampDlg::CLineRampDlg(CWnd* pParent)
	: CDialog(CLineRampDlg::IDD, pParent)
	, c_EditBrush(EDITBKCOLOR)
	, m_EditMap({
			EDITITEM(IDC_LINE_RAMP_STEP, OnNotifyEditRampStep),
			EDITITEM(IDC_LINE_RAMP_DELAY, OnNotifyEditRampDelay),

		})
	, m_RampStep(0)
	, m_RampDelay(0)
{
}
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LINE_RAMP_STEP_INFO, m_RampStepInfoButton);
	DDX_Control(pDX, IDC_LINE_RAMP_DELAY_INFO, m_RampDelayInfoButton);
	DDX_Text(pDX, IDC_LINE_RAMP_STEP, m_RampStep);
	DDX_Text(pDX, IDC_LINE_RAMP_DELAY, m_RampDelay);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineRampDlg, CDialog)

	ON_STN_CLICKED(IDC_LINE_RAMP_STEP, &CLineRampDlg::OnStnClickedRampStep)
	ON_STN_CLICKED(IDC_LINE_RAMP_DELAY, &CLineRampDlg::OnStnClickedRampDelay)
	ON_BN_CLICKED(IDC_LINE_RAMP_STEP_INFO, OnBnClickedLineRampStepInfo)
	ON_BN_CLICKED(IDC_LINE_RAMP_DELAY_INFO, OnBnClickedLineRampDelayInfo)

	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::OnStnClickedRampStep()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LINE_RAMP_STEP);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::OnStnClickedRampDelay()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LINE_RAMP_DELAY);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineRampDlg::OnNotifyEditRampStep()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_RampStep);
	if (bModified)
	{
		REMOTEREF.setLineRampStep(_F32(m_RampStep));
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineRampDlg::OnNotifyEditRampDelay()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_RampDelay);
	if (bModified)
	{
		REMOTEREF.setLineRampDelay(m_RampDelay);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineRampDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
BOOL CLineRampDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0);

	auto result = CDialog::OnInitDialog();
	if (result)
	{
		SaveParams();
		BOOL bLineEditPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);
		ENABLE_ID(IDC_LINE_RAMP_STEP, bLineEditPermission);
		ENABLE_ID(IDC_LINE_RAMP_DELAY, bLineEditPermission);
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
HBRUSH CLineRampDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int32_t lID = pWnd->GetDlgCtrlID();
	switch (lID)
	{
	case IDC_LINE_RAMP_STEP:
	case IDC_LINE_RAMP_DELAY:
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
void CLineRampDlg::OnBnClickedLineRampStepInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_RAMP_STEP, IDS_LINE_RAMPSTEP_KGH, IDS_INFO_LINE_RAMPSTEP_KGH);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::OnBnClickedLineRampDelayInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_RAMP_DELAY, IDS_LINE_RAMPDELAY_S, IDS_INFO_LINE_RAMPDELAY_S);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::SaveParams()
{
	uint32_t lValue = 0;
	REMOTEREF.getLineRampDelay(lValue);
	m_RampDelay = lValue;

	float32_t fValue = 0.0F;
	REMOTEREF.getLineRampStep(fValue);
	m_RampStep = _U32(fValue);

	backupParam.RampDelay = m_RampDelay;
	backupParam.RampStep  = m_RampStep;
	UpdateData(FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::RestoreParams()
{
	UpdateData(TRUE);
	if (m_RampDelay != backupParam.RampDelay)
	{
		REMOTEREF.setLineRampDelay(backupParam.RampDelay);
	}
	if (m_RampStep != backupParam.RampStep)
	{
		REMOTEREF.setLineRampStep(_F32(backupParam.RampStep));
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineRampDlg::OnCancel()
{
	RestoreParams();
	CDialog::OnCancel();
}
