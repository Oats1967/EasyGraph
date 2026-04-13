//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineTotalizerPulseDlg
///
/// @file   LineTotalizerPulseDlg.cpp
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
#include "LineTotalizerPulseDlg.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"
#include "HelpInfoDlgEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLineTotalizerPulseDlg, _func)

IMPLEMENT_DYNAMIC(CLineTotalizerPulseDlg, CDialog)
//***********************************************************************************************
//***********************************************************************************************
CLineTotalizerPulseDlg::CLineTotalizerPulseDlg(CWnd* pParent)
	: CDialog(CLineTotalizerPulseDlg::IDD, pParent)
	, c_EditBrush(EDITBKCOLOR)
	, m_EditMap({
			EDITITEM(IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT, OnNotifyEditStep),
			EDITITEM(IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT, OnNotifyEditImpulse),

		})
		, m_Step{ 0.0F }
	, m_Impulse{ 0 }
{
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LINE_TOTALIZERIMPULSE_STEP_INFO, m_StepInfoButton);
	DDX_Control(pDX, IDC_LINE_TOTALIZERIMPULSE_IMPULSE_INFO, m_ImpulseInfoButton);
	DDX_Text(pDX, IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT, m_Step);
	DDX_Text(pDX, IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT, m_Impulse);
}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineTotalizerPulseDlg, CDialog)

	ON_STN_CLICKED(IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT, &CLineTotalizerPulseDlg::OnStnClickedStep)
	ON_STN_CLICKED(IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT, &CLineTotalizerPulseDlg::OnStnClickedImpulse)
	ON_BN_CLICKED(IDC_LINE_TOTALIZERIMPULSE_STEP_INFO, OnBnClickedLineStepInfo)
	ON_BN_CLICKED(IDC_LINE_TOTALIZERIMPULSE_IMPULSE_INFO, OnBnClickedLineImpulseInfo)

	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_NOTIFYEDIT, OnNotifyEdit)
END_MESSAGE_MAP()
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::OnStnClickedStep()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::OnStnClickedImpulse()
{
	CEditCtrl::GetInput(this, E_TYPCTRL::E_INTCTRL, IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT);
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineTotalizerPulseDlg::OnNotifyEditStep()
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_Step);
	if (bModified)
	{
		REMOTEREF.setLineTotalizerPulseStep(m_Step);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineTotalizerPulseDlg::OnNotifyEditImpulse()
{
	auto bModified = CEditCtrlEx::GetLongAbs(m_Impulse);
	if (bModified)
	{
		REMOTEREF.setLineTotalizerPulseDuration(m_Impulse);
	}
	return bModified;
}
//***********************************************************************************************
//***********************************************************************************************
LRESULT CLineTotalizerPulseDlg::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			LOGERROR("Error in map" << _S32(id));
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
BOOL CLineTotalizerPulseDlg::OnInitDialog()
{
	ModifyStyle(WS_SYSMENU, 0);

	auto result = CDialog::OnInitDialog();
	if (result)
	{
		SaveParams();

		BOOL bLineEditPermission = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);
		ENABLE_ID(IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT, bLineEditPermission);
		ENABLE_ID(IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT, bLineEditPermission);
	}
	return result;
}
//***********************************************************************************************
//***********************************************************************************************
HBRUSH CLineTotalizerPulseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int32_t lID = pWnd->GetDlgCtrlID();
	switch (lID)
	{
	case IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT:
	case IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT:
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
void CLineTotalizerPulseDlg::OnBnClickedLineStepInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_TOTALIZERIMPULSE_STEP_EDIT, IDS_TOTALIZERIMPULSE_STEP, IDS_INFO_TOTALIZERIMPULSE_STEP);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::OnBnClickedLineImpulseInfo()
{
	CHelpInfoDlgEx::CreateHelpInfoBox(this, IDC_LINE_TOTALIZERIMPULSE_IMPULSE_EDIT, IDS_TOTALIZERIMPULSE_IMPULSE, IDS_INFO_TOTALIZERIMPULSE_IMPULSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::SaveParams()
{
	uint32_t lValue = 0;
	REMOTEREF.getLineTotalizerPulseDuration(lValue);
	m_Impulse = lValue;

	float32_t fValue = 0.0F;
	REMOTEREF.getLineTotalizerPulseStep(fValue);
	m_Step = fValue;

	backupParam.Impulse = m_Impulse;
	backupParam.Step = m_Step;

	UpdateData(FALSE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::RestoreParams()
{
	UpdateData(TRUE);

	if (m_Step != backupParam.Step)
	{
		REMOTEREF.setLineTotalizerPulseStep(backupParam.Step);
	}
	if (m_Impulse != backupParam.Impulse)
	{
		REMOTEREF.setLineTotalizerPulseDuration(backupParam.Impulse);
	}
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTotalizerPulseDlg::OnCancel()
{
	RestoreParams();
	CDialog::OnCancel();
}
