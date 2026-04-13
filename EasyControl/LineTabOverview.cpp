//------------------------------------------------------------------------------------
///
///                           <<<   E A S Y C O N T R O L   >>>
///
///
/// @brief  Implementation of module LineTabOverview
///
/// @file   LineTabOverview.cpp
///
///
/// @coypright Ing.büro Hafer
///            Branderweg 8A
///            D-91058 Erlangen
///
/// @author    Detlef Hafer
///
//------------------------------------------------------------------------------------
#include "stdafx.h"
#include "EasyControl.h"
#include "RemoteControl.h"
#include "LineTabOverview.h"
#include "LineSettingsDlg.h"
#include "DataImage.h"
#include "FormatMetric.h"
#include "MFCMacros.h"
#include "EditCtrlEx.h"



#define EDITITEM(_a, _func) 	BINDFUNC(_a, CLineTabOverview, _func)

// CLineTabOverview-Dialogfeld
IMPLEMENT_DYNAMIC(CLineTabOverview, CDialog)
CLineTabOverview::CLineTabOverview(CWnd* pParent)
	: CDialog(CLineTabOverview::IDD, pParent)
	, c_EditMap({
		EDITITEM(IDC_LINETAB_OVERVIEW_SETPOINT_EDIT,	OnNotifyEditSetpoint)
		})
	, c_EditBrush{ EDITBKCOLOR }
	, c_GreenBrush{ COLORLIGHTGREEN }
	, c_YellowBrush{ COLORLIGHTYELLOW }
	, c_RedBrush{ COLORLIGHTRED }
	, m_aDescriptionFont()
	, m_aStaticFont()
	, m_wndOverviewGauge()
	, m_aRampUp()
	, m_aRampDown()
	, m_StartBt{ IDS_BT_LINESTART, IDS_BT_LINESTOP }
	, m_StSetpoint()
	, m_StMassflow()
	, m_fSetpoint { 0.0F }
	, m_fMassflow { 0.0F }
	, m_fMaxSetpoint{ 0.0F }
	, m_Redraw { FALSE }
	, m_bLineStart{ FALSE }
	, m_bSetpointEditable{ FALSE }
{
	VERIFY(m_aDescriptionFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	VERIFY(m_aStaticFont.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

}
//***********************************************************************************************
//***********************************************************************************************
BEGIN_MESSAGE_MAP(CLineTabOverview, CDialog)
	ON_MESSAGE(WM_TIMER_REFRESH, &CLineTabOverview::OnTimerRefresh)
	ON_MESSAGE(WM_NOTIFYEDIT, &CLineTabOverview::OnNotifyEdit)
	ON_STN_CLICKED(IDC_LINETAB_OVERVIEW_SETPOINT_EDIT, &CLineTabOverview::OnStnClickedLineTabOverviewSetpoint)
	ON_BN_CLICKED(IDC_LINETAB_OVERVIEW_RAMP_UP, &CLineTabOverview::OnBnClickedLineTabOverviewRampUp)
	ON_BN_CLICKED(IDC_LINETAB_OVERVIEW_RAMP_DOWN, &CLineTabOverview::OnBnClickedLineTabOverviewRampDown)
	ON_STN_DBLCLK(IDC_LINETAB_OVERVIEW_GRAPHIC, &CLineTabOverview::OnDblclkLineTabOverviewGauge)
	ON_BN_CLICKED(IDC_LINETAB_OVERVIEW_START, &CLineTabOverview::OnClickedLineTabOverviewLineStart)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//******************************************************************************************************
//******************************************************************************************************
const CBrush& CLineTabOverview::GetMassflowBrush() const
{
	if ((! m_bLineStart) || _F32(fabs(m_fSetpoint)) < 1e-03f)
	{
		return c_EditBrush;
	}
	else
	{
		float32_t fDiff = _F32(fabs(m_fSetpoint - m_fMassflow));
		if (fDiff <= m_fSetpoint * 0.01F)
		{
			return c_GreenBrush;
		}
		else
		{
			return (fDiff < m_fSetpoint * 0.1F) ? c_YellowBrush : c_RedBrush;
		}
	}
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLineTabOverview::IsSetpointEditable() const
{
	BOOL bLineEditEnable = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);
	if (bLineEditEnable)
	{
		const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
		bLineEditEnable = (!processstatus.flags.StateExtlineSetpoint) &&
			              (processstatus.flags.ModuleLineSetpoint);
	}
	return bLineEditEnable;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLineTabOverview::IsRampEnabled() const
{
	BOOL bLineRampEnabled = IsSetpointEditable();
	if (bLineRampEnabled)
	{
		const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
		bLineRampEnabled = BOOL(processstatus.flags.ModuleSetpointRamp);
	}
	return bLineRampEnabled;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLineTabOverview::IsRampShow() const
{
	BOOL bLineRampShow = IsRampEnabled();
	return bLineRampShow;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLineTabOverview::IsLineStartEnabled() const
{
	BOOL bLineStartEnable = (USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_DOSE_START));
	if (bLineStartEnable)
	{
		const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
		bLineStartEnable		  = ! processstatus.flags.IOlinestart;
	}
	return bLineStartEnable;
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLineTabOverview::IsLineStartShow() const
{
	BOOL bLineStartShow = IsLineStartEnabled();
	if (bLineStartShow)
	{
		const auto& processstatus = dataimage::getLineDataImage().m_Cache.m_ProcessStatus;
		bLineStartShow = processstatus.flags.LineStartEnable;
	}
	return bLineStartShow;
}
//******************************************************************************************************
//******************************************************************************************************
void CLineTabOverview::SetValue()
{
	const auto& aV = dataimage::getLineDataImage();
	const auto& cache = aV.m_Cache;

	SetMassflow(cache.m_fLineMassflow);
	SetSetpoint(cache.m_fLineSetpoint);
	SetMaxSetpoint(cache.m_fMaxLeistung);

	m_wndOverviewGauge.SetMaxSetpoint(m_fMaxSetpoint);
	m_wndOverviewGauge.SetSetpoint(m_fSetpoint);
	m_wndOverviewGauge.SetMassflow(m_fMassflow);


	const auto bLineRampEnabled		 = IsRampEnabled();
	ENABLE_SHOW_ID(IDC_LINETAB_OVERVIEW_RAMP_UP, bLineRampEnabled, bLineRampEnabled);
	ENABLE_SHOW_ID(IDC_LINETAB_OVERVIEW_RAMP_DOWN, bLineRampEnabled, bLineRampEnabled);

	const auto bLineStartEnable = IsLineStartEnabled();
	const auto bLineStartShow   = IsLineStartShow();
	ENABLE_SHOW_ID(IDC_LINETAB_OVERVIEW_START, bLineStartEnable, bLineStartShow);

	const auto& processstatus = cache.m_ProcessStatus;
	m_bLineStart = processstatus.flags.linecontrolstarted;
	if (bLineStartShow )
	{
		m_StartBt.SetState((m_bLineStart) ? CButtonStartStop::eState::eStarted : CButtonStartStop::eState::eStopped);
	}
	UpdateData(FALSE);

	auto bLineSetpointEditable = IsSetpointEditable();
	if (bLineSetpointEditable != m_bSetpointEditable)
	{
		m_bSetpointEditable = bLineSetpointEditable;
		Invalidate(FALSE);
	}
}
//******************************************************************************************************
//******************************************************************************************************
BOOL CLineTabOverview::OnNotifyEditSetpoint(void)
{
	auto bModified = CEditCtrlEx::GetFloatAbs(m_fSetpoint);
	if (bModified)
	{
		REMOTEREF.setLineSetpoint(m_fSetpoint);
	}
	return bModified;
}
//***************************************************************************************
//***************************************************************************************
LRESULT CLineTabOverview::OnNotifyEdit(WPARAM id, LPARAM bValue)
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
			PostMessage(WM_TIMER_REFRESH);
		}
	}
	return 0;
}

//******************************************************************************************************
//******************************************************************************************************
void CLineTabOverview::GetValue()
{
	UpdateData(TRUE);
}
//***********************************************************************************************
//***********************************************************************************************
void CLineTabOverview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINETAB_OVERVIEW_GRAPHIC, m_wndOverviewGauge);
	DDX_Control(pDX, IDC_LINETAB_OVERVIEW_RAMP_UP, m_aRampUp);
	DDX_Control(pDX, IDC_LINETAB_OVERVIEW_RAMP_DOWN, m_aRampDown);
	DDX_Control(pDX, IDC_LINETAB_OVERVIEW_START, m_StartBt);
	DDX_Control(pDX, IDC_LINETAB_OVERVIEW_SETPOINT_STATIC, m_StSetpoint);
	DDX_Control(pDX, IDC_LINETAB_OVERVIEW_MASSFLOW_STATIC, m_StMassflow);
	DDX_Float(pDX, IDC_LINETAB_OVERVIEW_SETPOINT_EDIT, m_fSetpoint);
	DDX_Float(pDX, IDC_LINETAB_OVERVIEW_MASSFLOW, m_fMassflow);

}
//***********************************************************************************************
//***********************************************************************************************
BOOL CLineTabOverview::OnInitDialog()
{
	auto result = CDialog::OnInitDialog();
	if (result)
	{
		m_aRampUp.SizeToContent();
		m_aRampDown.SizeToContent();
		SETFONT_ID(IDC_LINETAB_OVERVIEW_SETPOINT_EDIT, &m_aDescriptionFont);
		SETFONT_ID(IDC_LINETAB_OVERVIEW_MASSFLOW, &m_aDescriptionFont);
		m_StSetpoint.SetFont(&m_aStaticFont);
		m_StSetpoint.SizeToContent();
		m_StMassflow.SetFont(&m_aStaticFont);
		m_StMassflow.SizeToContent();

		auto bLogin = USERRIGHTSREF.IsAktUserPermitted(base::utils::eUserCategory::LOGIN_LINE_EDIT);
		ENABLE_ID(IDC_LINETAB_OVERVIEW_SETPOINT_EDIT, bLogin);

		SetValue();
	}
	return result;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
LRESULT CLineTabOverview::OnTimerRefresh(WPARAM, LPARAM)
{
	SetValue();
	return 0L;
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabOverview::OnDblclkLineTabOverviewGauge()
{
	const auto& aV = dataimage::getLineDataImage();
	const auto& cache = aV.m_Cache;
	const auto& processstatus = cache.m_ProcessStatus;

	BOOL bEnable = (processstatus.flags.ModuleLineSetpoint != FALSE) ||
		(processstatus.flags.ModuleSetpointRamp != FALSE) ||
		(processstatus.flags.ModuleExternalSetpoint != FALSE) ||
		(processstatus.flags.ModuleLocalSetpoint != FALSE);

	if (bEnable)
	{
		CLineSettingsDlg aDlg(this);

		aDlg.DoModal();
	}
}
//***************************************************************************************
//***************************************************************************************
void CLineTabOverview::OnStnClickedLineTabOverviewSetpoint()
{
	const BOOL bLineEditEnable = IsSetpointEditable();
	if (bLineEditEnable)
	{
		CEditCtrl::GetInput(this, E_TYPCTRL::E_FLOATCTRL, IDC_LINETAB_OVERVIEW_SETPOINT_EDIT);
	}
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabOverview::OnBnClickedLineTabOverviewRampUp()
{
	REMOTEREF.setLinePBRampUp();
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabOverview::OnBnClickedLineTabOverviewRampDown()
{
	REMOTEREF.setLinePBRampDown();
}
//*******************************************************************************************************************
//*******************************************************************************************************************
void CLineTabOverview::OnClickedLineTabOverviewLineStart()
{
	if (! m_StartBt.isStarted())
	{
		REMOTEREF.setLinePBExtStart();
		m_StartBt.SetState(CButtonStartStop::eState::eStarted);
	}
	else
	{
		REMOTEREF.setLinePBExtStop();
		m_StartBt.SetState(CButtonStartStop::eState::eStopped);
	}
}
//***************************************************************************************
//***************************************************************************************
HBRUSH CLineTabOverview::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_LINETAB_OVERVIEW_SETPOINT_EDIT:
		{
			if (pWnd->IsWindowEnabled())
			{
				pDC->SetTextColor(EDITTEXTCOLOR);
				pDC->SetBkMode(TRANSPARENT);
				if (IsSetpointEditable())
				{
					hbr = (HBRUSH)c_EditBrush;
				}
			}
		}
		break;

		case IDC_LINETAB_OVERVIEW_MASSFLOW:
		{
			if (pWnd->IsWindowEnabled())
			{
				pDC->SetTextColor(EDITTEXTCOLOR);
				pDC->SetBkMode(TRANSPARENT);
				hbr = (HBRUSH)GetMassflowBrush();
			}
		}
		break;

		default:
			break;
	}
	return hbr;
}

